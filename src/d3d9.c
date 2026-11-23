#pragma once

#include <d3d9.h>
#include <realtimeapiset.h>

#include "detour.c"
#include "window.c"

HRESULT WINAPI (*g_Reset)(PVOID, PVOID) = {};
HRESULT WINAPI (*g_Present)(PVOID, PVOID, PVOID, HWND, PVOID) = {};
HRESULT WINAPI (*g_CreateDevice)(PVOID, UINT, D3DDEVTYPE, HWND, DWORD, PVOID, PVOID) = {};

HRESULT WINAPI Reset(LPDIRECT3DDEVICE9 this, D3DPRESENT_PARAMETERS *pp)
{
    g_Windowed = pp->Windowed;

    D3DPRESENT_PARAMETERS rp = *pp;
    rp.Windowed = TRUE, rp.SwapEffect = D3DSWAPEFFECT_COPY;
    rp.BackBufferCount = rp.FullScreen_RefreshRateInHz = 0;
    rp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

    return g_Reset(this, &rp);
}

HRESULT WINAPI Present(LPDIRECT3DDEVICE9 this, PVOID src, PVOID dst, HWND hwnd, PVOID rgn)
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

HRESULT WINAPI CreateDevice(PVOID this, UINT idx, D3DDEVTYPE type, HWND hwnd, DWORD flags, D3DPRESENT_PARAMETERS *pp,
                            LPDIRECT3DDEVICE9 *dev)
{
    if (!IsWindow(g_Child))
    {
        g_Windowed = pp->Windowed, g_WndProc = (PVOID)SetWindowLongW(hwnd, GWL_WNDPROC, (LONG)WndProc);
        g_Child = CreateWindowExW(0, L" ", NULL, WS_CHILD | WS_VISIBLE, 0, 0, 0, 0, hwnd, NULL, NULL, NULL);

        LONG ws = WS_VISIBLE * IsWindowVisible(hwnd);
        SetWindowLongW(hwnd, GWL_EXSTYLE, 0), SetWindowLongW(hwnd, GWL_STYLE, ws);
        SetClassLongW(hwnd, GCL_HBRBACKGROUND, (LONG)GetStockObject(BLACK_BRUSH));

        UINT wp = SWP_SHOWWINDOW | SWP_FRAMECHANGED;
        wp |= g_Windowed * (SWP_NOMOVE | SWP_NOSIZE);

        SetWindowPos(hwnd, HWND_TOP, 0, 0, 0, 0, wp);
        SendMessageW(hwnd, WM_NCACTIVATE, FALSE, -1);

        RedrawWindow(hwnd, NULL, NULL, RDW_ERASE | RDW_ERASENOW | RDW_INVALIDATE);
    }

    D3DPRESENT_PARAMETERS cp = *pp;
    cp.Windowed = TRUE, cp.SwapEffect = D3DSWAPEFFECT_COPY;
    cp.BackBufferCount = cp.FullScreen_RefreshRateInHz = 0;
    cp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

    HRESULT hr = g_CreateDevice(this, idx, type, hwnd, flags, &cp, dev);

    if (SUCCEEDED(hr) && !g_Present && !g_Reset)
    {
        g_Reset = CreateDetour((*dev)->lpVtbl->Reset, Reset);
        g_Present = CreateDetour((*dev)->lpVtbl->Present, Present);
    }

    return hr;
}

VOID D3D9_DllMain(VOID)
{
    LPDIRECT3D9 dx9 = Direct3DCreate9(D3D_SDK_VERSION);
    g_CreateDevice = CreateDetour(dx9->lpVtbl->CreateDevice, CreateDevice);
    dx9->lpVtbl->Release(dx9);
}