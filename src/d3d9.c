#include <d3d9.h>
#include <realtimeapiset.h>

#include "hook.c"
#include "window.c"

HRESULT WINAPI (*g_Reset)(PVOID, PVOID) = {};
HRESULT WINAPI (*g_Present)(PVOID, PVOID, PVOID, HWND, PVOID) = {};
HRESULT WINAPI (*g_CreateDevice)(PVOID, UINT, D3DDEVTYPE, HWND, DWORD, PVOID, PVOID) = {};

HRESULT WINAPI Reset(PVOID this, D3DPRESENT_PARAMETERS *params)
{
    g_Windowed = params->Windowed;

    D3DPRESENT_PARAMETERS pp = *params;
    pp.Windowed = pp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    pp.BackBufferCount = pp.FullScreen_RefreshRateInHz = 0;
    pp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

    return g_Reset(this, &pp);
}

HRESULT WINAPI Present(LPDIRECT3DDEVICE9 this, PVOID src, PVOID dst, HWND wnd, PVOID rgn)
{
    static ULONG64 s_last = {};

    ULONG64 now = {};
    QueryInterruptTimePrecise(&now);

    s_last = (s_last ?: now) + 1E7 / 60;
    s_last = s_last < now ? now : s_last;

    do
    {
        _mm_pause();
        QueryInterruptTimePrecise(&now);
    } while (now < s_last);

    D3DVIEWPORT9 vp = {};
    this->lpVtbl->GetViewport(this, &vp);

    RECT rc = {vp.X, vp.Y, vp.X + vp.Width, vp.Y + vp.Height};
    return g_Present(this, &rc, NULL, g_Window, NULL);
}

HRESULT WINAPI CreateDevice(PVOID this, UINT adapter, D3DDEVTYPE type, HWND wnd, DWORD flags,
                            D3DPRESENT_PARAMETERS *params, LPDIRECT3DDEVICE9 *device)
{
    g_Windowed = params->Windowed;

    D3DPRESENT_PARAMETERS pp = *params;
    pp.Windowed = pp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    pp.BackBufferCount = pp.FullScreen_RefreshRateInHz = 0;
    pp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

    if (!IsWindow(g_Window))
    {
        g_WndProc = (PVOID)SetWindowLongW(wnd, GWL_WNDPROC, (LONG)WndProc);
        g_Window = CreateWindowExW(WS_EX_LEFT, L" ", NULL, WS_CHILD, 0, 0, 0, 0, wnd, NULL, NULL, NULL);

        SetWindowLongW(wnd, GWL_EXSTYLE, WS_EX_LEFT);
        SetWindowLongW(wnd, GWL_STYLE, WS_OVERLAPPED);

        SetClassLongW(wnd, GCL_HBRBACKGROUND, (LONG)GetStockObject(BLACK_BRUSH));
        SetWindowPos(wnd, NULL, 0, 0, 0, 0, g_Windowed * (SWP_NOMOVE | SWP_NOSIZE) | SWP_SHOWWINDOW);
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