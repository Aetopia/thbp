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
    case WM_SIZE:
    case WM_SETCURSOR:
    case WM_ERASEBKGND: {
        (uMsg == WM_SETCURSOR) && (ShowCursor(TRUE));
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

        SetWindowPos(g_Wnd, NULL, x, y, cx, cy, 0);
        break;
    }
    }
    return CallWindowProcW(g_WndProc, hWnd, uMsg, wParam, lParam);
}

LRESULT WINAPI FullScreenWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    if (uMsg == WM_WINDOWPOSCHANGING)
    {
        MONITORINFO mi = {sizeof(MONITORINFO)};
        GetMonitorInfoW(MonitorFromPoint((POINT){}, MONITOR_DEFAULTTOPRIMARY), &mi);

        PWINDOWPOS wp = (PWINDOWPOS)lParam;

        wp->flags &= ~SWP_NOMOVE;
        wp->flags &= ~SWP_NOSIZE;

        wp->x = mi.rcMonitor.left;
        wp->y = mi.rcMonitor.top;

        wp->cx = mi.rcMonitor.right - wp->x;
        wp->cy = mi.rcMonitor.bottom - wp->y;
    }
    return WindowedWndProc(hWnd, uMsg, wParam, lParam);
}