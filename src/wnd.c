#include <windef.h>
#include <wingdi.h>
#include <winuser.h>
#include <winbase.h>

HWND g_Wnd = {};
WNDPROC g_WndProc = {};

LRESULT WINAPI WindowedWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_SIZE: {
        return 0;
    }
    case WM_WINDOWPOSCHANGING: {
        ((PWINDOWPOS)lParam)->flags |= SWP_SHOWWINDOW;
        break;
    }
    case WM_SETCURSOR:
    case WM_ERASEBKGND: {
        return DefWindowProcW(hWnd, uMsg, wParam, lParam);
    }
    case WM_SYSCOMMAND: {
        switch (GET_SC_WPARAM(wParam))
        {
        case SC_KEYMENU:
        case SC_MOUSEMENU:
            return 0;
        }
        break;
    }
    case WM_WINDOWPOSCHANGED: {
        RECT rc = {};
        GetClientRect(hWnd, &rc);

        INT cx = rc.right;
        INT cy = MulDiv(cx, 3, 4);

        if (cy > rc.bottom)
        {
            cy = rc.bottom;
            cx = MulDiv(cy, 4, 3);
        }

        INT x = (rc.right - cx) / 2;
        INT y = (rc.bottom - cy) / 2;

        SetWindowPos(g_Wnd, NULL, x, y, cx, cy, SWP_NOZORDER);
        return 0;
    }
    }
    return CallWindowProcW(g_WndProc, hWnd, uMsg, wParam, lParam);
}

LRESULT WINAPI FullScreenWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    if (uMsg == WM_WINDOWPOSCHANGED)
    {
        MONITORINFO mi = {.cbSize = sizeof(MONITORINFO)};
        GetMonitorInfoW(MonitorFromWindow(hWnd, MONITOR_DEFAULTTONEAREST), &mi);

        INT x = mi.rcMonitor.left;
        INT y = mi.rcMonitor.top;

        INT cx = mi.rcMonitor.right - x;
        INT cy = mi.rcMonitor.bottom - y;

        SetWindowPos(hWnd, NULL, x, y, cx, cy, SWP_NOZORDER);
    }
    return WindowedWndProc(hWnd, uMsg, wParam, lParam);
}