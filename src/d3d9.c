#pragma once
#include "wnd.c"
#include "hook.c"
#include <d3d9.h>

HRESULT WINAPI (*g_Reset)(PVOID, PVOID) = {};
HRESULT WINAPI (*g_Present)(PVOID, PVOID, PVOID, HWND, PVOID) = {};
HRESULT WINAPI (*g_CreateDevice)(PVOID, UINT, D3DDEVTYPE, HWND, DWORD, PVOID, PVOID) = {};

HRESULT WINAPI Present(PVOID this, PVOID src, PVOID dst, HWND wnd, PVOID rgn)
{
    return g_Present(this, NULL, NULL, g_Wnd, NULL);
}

HRESULT WINAPI Reset(PVOID this, D3DPRESENT_PARAMETERS *params)
{
    params->PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
    return params->Windowed ? g_Reset(this, params) : E_FAIL;
}

HRESULT WINAPI CreateDevice(PVOID this, UINT adapter, D3DDEVTYPE type, HWND wnd, DWORD flags,
                            D3DPRESENT_PARAMETERS *params, LPDIRECT3DDEVICE9 *device)
{
    static BOOL s_flag = {};

    flags |= D3DCREATE_NOWINDOWCHANGES;
    params->PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
    HRESULT hr = params->Windowed ? g_CreateDevice(this, adapter, type, wnd, flags, params, device) : E_FAIL;

    if (SUCCEEDED(hr) && !s_flag)
    {
        s_flag = TRUE;

        g_WndProc = (PVOID)SetWindowLongW(wnd, GWLP_WNDPROC, (LONG_PTR)WndProc);
        g_Wnd = CreateWindowExW(WS_EX_LEFT, L" ", NULL, WS_VISIBLE | WS_CHILD, 0, 0, 0, 0, wnd, NULL, NULL, NULL);

        SetWindowLongW(wnd, GWL_EXSTYLE, WS_EX_APPWINDOW);
        SetWindowLongW(wnd, GWL_STYLE, WS_CLIPCHILDREN | WS_POPUP | (IsWindowVisible(wnd) * WS_VISIBLE));

        SetWindowPos(wnd, NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);

        g_Reset = CreateHook((*device)->lpVtbl->Reset, Reset);
        g_Present = CreateHook((*device)->lpVtbl->Present, Present);
    }

    return hr;
}