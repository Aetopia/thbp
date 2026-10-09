#include "wnd.c"
#include "hook.c"
#include <d3d9.h>

LARGE_INTEGER g_Ticks = {};
HRESULT WINAPI (*g_Reset)(PVOID, PVOID) = {};
HRESULT WINAPI (*g_Present)(PVOID, PVOID, PVOID, HWND, PVOID) = {};
HRESULT WINAPI (*g_CreateDevice)(PVOID, UINT, D3DDEVTYPE, HWND, DWORD, PVOID, PVOID) = {};

HRESULT WINAPI Reset(PVOID this, D3DPRESENT_PARAMETERS *params)
{
    D3DPRESENT_PARAMETERS d3dpp = *params;

    d3dpp.Windowed = TRUE;
    d3dpp.FullScreen_RefreshRateInHz = 0;
    d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

    return g_Reset(this, &d3dpp);
}

HRESULT WINAPI Present(PVOID this, PVOID src, PVOID dst, HWND wnd, PVOID rgn)
{
    static LARGE_INTEGER last = {};

    LARGE_INTEGER now = {};
    QueryPerformanceCounter(&now);

    last = last.QuadPart ? last : now;
    last.QuadPart += g_Ticks.QuadPart;

    if (last.QuadPart < now.QuadPart)
        last = now;

    while (now.QuadPart < last.QuadPart)
        QueryPerformanceCounter(&now);

    return g_Present(this, NULL, NULL, g_Wnd, NULL);
}

HRESULT WINAPI CreateDevice(PVOID this, UINT adapter, D3DDEVTYPE type, HWND wnd, DWORD flags,
                            D3DPRESENT_PARAMETERS *params, LPDIRECT3DDEVICE9 *device)
{
    D3DPRESENT_PARAMETERS d3dpp = *params;

    if (!IsWindow(g_Wnd))
    {
        DWORD style = d3dpp.Windowed ? WS_OVERLAPPEDWINDOW : WS_POPUP;
        WNDPROC procedure = d3dpp.Windowed ? WindowedWndProc : FullScreenWndProc;

        g_WndProc = (PVOID)SetWindowLongW(wnd, GWLP_WNDPROC, (LONG_PTR)procedure);
        g_Wnd = CreateWindowExW(WS_EX_LEFT, L" ", NULL, WS_CHILD | WS_VISIBLE, 0, 0, 0, 0, wnd, NULL, NULL, NULL);

        SetWindowLongW(wnd, GWL_EXSTYLE, WS_EX_LEFT);
        SetWindowLongW(wnd, GWL_STYLE, style | (WS_VISIBLE * IsWindowVisible(wnd)));

        SetClassLongW(wnd, GCLP_HBRBACKGROUND, (LONG)GetStockObject(BLACK_BRUSH));
        SetWindowPos(wnd, NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_DRAWFRAME | SWP_SHOWWINDOW);
    }

    d3dpp.Windowed = TRUE;
    d3dpp.FullScreen_RefreshRateInHz = 0;
    d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
    
    HRESULT hr = g_CreateDevice(this, adapter, type, wnd, flags, &d3dpp, device);

    if (SUCCEEDED(hr) && !g_Present && !g_Reset)
    {
        g_Reset = CreateHook((*device)->lpVtbl->Reset, Reset);
        g_Present = CreateHook((*device)->lpVtbl->Present, Present);
    }

    return hr;
}