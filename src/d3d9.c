#include "wnd.c"
#include "hook.c"

#include <d3d9.h>
#include <realtimeapiset.h>

HRESULT WINAPI (*g_Reset)(PVOID, PVOID) = {};
HRESULT WINAPI (*g_Present)(PVOID, PVOID, PVOID, HWND, PVOID) = {};
HRESULT WINAPI (*g_CreateDevice)(PVOID, UINT, D3DDEVTYPE, HWND, DWORD, PVOID, PVOID) = {};

HRESULT WINAPI Reset(PVOID this, D3DPRESENT_PARAMETERS *params)
{
    g_FullScreen = !params->Windowed;

    D3DPRESENT_PARAMETERS pp = *params;
    pp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

    pp.Windowed = TRUE;
    pp.FullScreen_RefreshRateInHz = 0;

    pp.BackBufferCount = 0;
    pp.SwapEffect = D3DSWAPEFFECT_DISCARD;

    return g_Reset(this, &pp);
}

HRESULT WINAPI Present(LPDIRECT3DDEVICE9 this, PVOID src, PVOID dst, HWND wnd, PVOID rgn)
{
    static HANDLE timer = {};
    static ULONG64 last = {};

    if (!timer)
    {
        DWORD access = TIMER_ALL_ACCESS;
        DWORD flags = CREATE_WAITABLE_TIMER_HIGH_RESOLUTION;
        timer = CreateWaitableTimerExW(NULL, NULL, flags, access);
    }

    ULONG64 now = {};
    QueryInterruptTimePrecise(&now);

    last = last ? last : now;
    last += 10'000'000 / 60;

    last = last < now ? now : last;
    LONG64 wait = -(last - now);

    SetWaitableTimer(timer, (PVOID)&wait, 0, NULL, NULL, FALSE);
    WaitForSingleObject(timer, INFINITE);

    D3DVIEWPORT9 vp = {};
    this->lpVtbl->GetViewport(this, &vp);

    RECT rc = {vp.X, vp.Y, vp.X + vp.Width, vp.Y + vp.Height};
    return g_Present(this, &rc, NULL, g_Window, NULL);
}

HRESULT WINAPI CreateDevice(PVOID this, UINT adapter, D3DDEVTYPE type, HWND wnd, DWORD flags,
                            D3DPRESENT_PARAMETERS *params, LPDIRECT3DDEVICE9 *device)
{
    g_FullScreen = !params->Windowed;

    D3DPRESENT_PARAMETERS pp = *params;
    pp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

    pp.Windowed = TRUE;
    pp.FullScreen_RefreshRateInHz = 0;

    pp.BackBufferCount = 0;
    pp.SwapEffect = D3DSWAPEFFECT_DISCARD;

    if (!IsWindow(g_Window))
    {
        g_WndProc = (PVOID)SetWindowLongW(wnd, GWL_WNDPROC, (LONG)WndProc);
        g_Window = CreateWindowExW(WS_EX_LEFT, L" ", NULL, WS_CHILD, 0, 0, 0, 0, wnd, NULL, NULL, NULL);

        SetWindowLongW(wnd, GWL_EXSTYLE, WS_EX_LEFT);
        SetWindowLongW(wnd, GWL_STYLE, WS_OVERLAPPED);

        HBRUSH hbr = GetStockObject(BLACK_BRUSH);
        SetClassLongW(wnd, GCL_HBRBACKGROUND, (LONG)hbr);

        UINT flags = SWP_FRAMECHANGED | SWP_SHOWWINDOW;
        flags |= params->Windowed * (SWP_NOMOVE | SWP_NOSIZE);

        SetWindowPos(wnd, NULL, 0, 0, 0, 0, flags);
    }

    flags |= D3DCREATE_NOWINDOWCHANGES;
    HRESULT hr = g_CreateDevice(this, adapter, type, wnd, flags, &pp, device);

    if (SUCCEEDED(hr) && !g_Present && !g_Reset)
    {
        g_Reset = CreateHook((*device)->lpVtbl->Reset, Reset);
        g_Present = CreateHook((*device)->lpVtbl->Present, Present);
    }

    return hr;
}