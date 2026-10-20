#include <d3d9.h>
#include <realtimeapiset.h>

#include "detour.c"
#include "window.c"

HRESULT WINAPI (*g_Reset)(PVOID, PVOID) = {};
HRESULT WINAPI (*g_Present)(PVOID, PVOID, PVOID, HWND, PVOID) = {};
HRESULT WINAPI (*g_CreateDevice)(PVOID, UINT, D3DDEVTYPE, HWND, DWORD, PVOID, PVOID) = {};

HRESULT WINAPI Reset(PVOID this, D3DPRESENT_PARAMETERS *params)
{
    t_Windowed = params->Windowed;

    D3DPRESENT_PARAMETERS pp = *params;
    pp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

    pp.Windowed = TRUE;
    pp.FullScreen_RefreshRateInHz = 0;

    pp.BackBufferCount = 0;
    pp.SwapEffect = D3DSWAPEFFECT_COPY;

    return g_Reset(this, &pp);
}

HRESULT WINAPI Present(LPDIRECT3DDEVICE9 this, PVOID src, PVOID dst, HWND wnd, PVOID rgn)
{
    static __thread ULONG64 t_last = {};

    ULONG64 now = {};
    QueryInterruptTimePrecise(&now);

    t_last = (t_last ?: now) + 1E7 / 60;
    t_last = t_last < now ? now : t_last;

    do
    {
        _mm_pause();
        QueryInterruptTimePrecise(&now);
    } while (now < t_last);

    D3DVIEWPORT9 vp = {};
    this->lpVtbl->GetViewport(this, &vp);

    RECT rc = {vp.X, vp.Y, vp.X + vp.Width, vp.Y + vp.Height};
    return g_Present(this, &rc, NULL, t_Child, NULL);
}

HRESULT WINAPI CreateDevice(PVOID this, UINT adapter, D3DDEVTYPE type, HWND wnd, DWORD flags,
                            D3DPRESENT_PARAMETERS *params, LPDIRECT3DDEVICE9 *device)
{
    t_Windowed = params->Windowed;

    D3DPRESENT_PARAMETERS pp = *params;
    pp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

    pp.Windowed = TRUE;
    pp.FullScreen_RefreshRateInHz = 0;

    pp.BackBufferCount = 0;
    pp.SwapEffect = D3DSWAPEFFECT_COPY;

    if (!IsWindow(t_Child))
    {
        t_WndProc = (PVOID)SetWindowLongW(wnd, GWL_WNDPROC, (LONG)WndProc);
        t_Child = CreateWindowExW(WS_EX_LEFT, L" ", NULL, WS_CHILD, 0, 0, 0, 0, wnd, NULL, NULL, NULL);

        SetWindowLongW(wnd, GWL_EXSTYLE, WS_EX_LEFT);
        SetWindowLongW(wnd, GWL_STYLE, WS_OVERLAPPED);

        HBRUSH hbr = GetStockObject(BLACK_BRUSH);
        SetClassLongW(wnd, GCL_HBRBACKGROUND, (LONG)hbr);

        UINT flags = SWP_SHOWWINDOW;
        flags |= t_Windowed * SWP_NOMOVE;
        flags |= t_Windowed * SWP_NOSIZE;

        SetWindowPos(wnd, NULL, 0, 0, 0, 0, flags);
    }

    flags |= D3DCREATE_MULTITHREADED | D3DCREATE_NOWINDOWCHANGES;
    HRESULT hr = g_CreateDevice(this, adapter, type, wnd, flags, &pp, device);

    if (SUCCEEDED(hr) && !g_Present && !g_Reset)
    {
        g_Reset = CreateDetour((*device)->lpVtbl->Reset, Reset);
        g_Present = CreateDetour((*device)->lpVtbl->Present, Present);
    }

    return hr;
}