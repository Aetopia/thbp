#pragma once
#include "wnd.c"
#include "hook.c"
#include <d3d9.h>

HRESULT WINAPI (*g_Reset)(PVOID, PVOID) = {};
HRESULT WINAPI (*g_CreateDevice)(PVOID, UINT, D3DDEVTYPE, HWND, DWORD, PVOID, PVOID) = {};

HRESULT WINAPI Reset(PVOID this, D3DPRESENT_PARAMETERS *params)
{
    D3DPRESENT_PARAMETERS d3dpp = *params;

    if (d3dpp.Windowed)
    {
        d3dpp.Windowed = TRUE;
        d3dpp.FullScreen_RefreshRateInHz = 0;

        d3dpp.hDeviceWindow = g_Wnd;
        d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

        return g_Reset(this, &d3dpp);
    }

    return E_FAIL;
}

HRESULT WINAPI CreateDevice(PVOID this, UINT adapter, D3DDEVTYPE type, HWND wnd, DWORD flags,
                            D3DPRESENT_PARAMETERS *params, LPDIRECT3DDEVICE9 *device)
{
    D3DPRESENT_PARAMETERS d3dpp = *params;

    if (d3dpp.Windowed)
    {
        if (!g_Wnd)
            g_Wnd = CreateWindowExW(WS_EX_LEFT, L" ", NULL, WS_VISIBLE | WS_CHILD, 0, 0, 0, 0, wnd, NULL, NULL, NULL);

        if (!g_WndProc)
        {
            g_WndProc = (PVOID)SetWindowLongW(wnd, GWLP_WNDPROC, (LONG_PTR)WndProc);

            SetWindowLongW(wnd, GWL_EXSTYLE, WS_EX_APPWINDOW);
            SetWindowLongW(wnd, GWL_STYLE, WS_POPUP | WS_CLIPCHILDREN | (IsWindowVisible(wnd) * WS_VISIBLE));

            SetWindowPos(wnd, NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);
        }

        d3dpp.Windowed = TRUE;
        d3dpp.FullScreen_RefreshRateInHz = 0;

        d3dpp.hDeviceWindow = g_Wnd;
        d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

        flags |= D3DCREATE_NOWINDOWCHANGES;
        HRESULT hr = g_CreateDevice(this, adapter, type, wnd, flags, &d3dpp, device);

        if (SUCCEEDED(hr) && !g_Reset)
            g_Reset = CreateHook((*device)->lpVtbl->Reset, Reset);

        return hr;
    }

    return E_FAIL;
}