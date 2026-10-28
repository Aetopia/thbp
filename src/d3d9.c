#include <d3d9.h>
#include <realtimeapiset.h>

#include "detour.c"
#include "window.c"

HRESULT WINAPI (*g_Reset)(PVOID, PVOID) = {};
HRESULT WINAPI (*g_Present)(PVOID, PVOID, PVOID, HWND, PVOID) = {};
HRESULT WINAPI (*g_CreateDevice)(PVOID, UINT, D3DDEVTYPE, HWND, DWORD, PVOID, PVOID) = {};

HRESULT WINAPI Reset(PVOID this, D3DPRESENT_PARAMETERS *pp)
{
    pp->Windowed = TRUE;
    pp->FullScreen_RefreshRateInHz = 0;

    pp->BackBufferCount = 0;
    pp->SwapEffect = D3DSWAPEFFECT_COPY;

    pp->PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

    return g_Reset(this, pp);
}

HRESULT WINAPI Present(LPDIRECT3DDEVICE9 this, PVOID src, PVOID dst, HWND wnd, PVOID rgn)
{
    static ULONG64 s_last = {};

    ULONG64 now = {};
    QueryInterruptTimePrecise(&now);

    s_last = (s_last ?: now) + 1E7 / 60;
    s_last = s_last < now ? now : s_last;

    while (now < s_last)
        QueryInterruptTimePrecise(&now);

    D3DVIEWPORT9 vp = {};
    this->lpVtbl->GetViewport(this, &vp);

    RECT rc = {vp.X, vp.Y, vp.X + vp.Width, vp.Y + vp.Height};
    return g_Present(this, &rc, NULL, g_Child, NULL);
}

HRESULT WINAPI CreateDevice(PVOID this, UINT adapter, D3DDEVTYPE type, HWND wnd, DWORD flags, D3DPRESENT_PARAMETERS *pp,
                            LPDIRECT3DDEVICE9 *device)
{
    if (!IsWindow(g_Child))
    {
        g_WndProc = (PVOID)SetWindowLongW(wnd, GWL_WNDPROC, (LONG)WndProc);
        g_Child = CreateWindowExW(0, L" ", NULL, WS_CHILD, 0, 0, 0, 0, wnd, NULL, NULL, NULL);

        SetWindowLongW(wnd, GWL_STYLE, 0);
        SetWindowLongW(wnd, GWL_EXSTYLE, 0);

        HBRUSH hbr = GetStockObject(BLACK_BRUSH);
        SetClassLongW(wnd, GCL_HBRBACKGROUND, (LONG)hbr);

        SetWindowPos(wnd, NULL, 0, 0, 0, 0, SWP_FRAMECHANGED);
        RedrawWindow(wnd, NULL, NULL, RDW_ERASE | RDW_ERASENOW | RDW_INVALIDATE);
    }

    pp->Windowed = TRUE;
    pp->FullScreen_RefreshRateInHz = 0;

    pp->BackBufferCount = 0;
    pp->SwapEffect = D3DSWAPEFFECT_COPY;

    pp->PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

    flags |= D3DCREATE_NOWINDOWCHANGES;
    HRESULT hr = g_CreateDevice(this, adapter, type, wnd, flags, pp, device);

    if (SUCCEEDED(hr) && !g_Present && !g_Reset)
    {
        g_Reset = CreateDetour((*device)->lpVtbl->Reset, Reset);
        g_Present = CreateDetour((*device)->lpVtbl->Present, Present);
    }

    return hr;
}