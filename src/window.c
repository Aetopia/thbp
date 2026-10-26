#include <windef.h>
#include <winuser.h>
#include <winbase.h>

HWND g_Window = {};
WNDPROC g_WndProc = {};

LRESULT WINAPI WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_SIZE:
    case WM_PAINT:
    case WM_SETCURSOR:
    case WM_ERASEBKGND: {
        if (uMsg == WM_SETCURSOR && hWnd == GetForegroundWindow())
        {
            SetCursor(NULL);
            return TRUE;
        }
        return DefWindowProcW(hWnd, uMsg, wParam, lParam);
    }
    case WM_SYSCOMMAND: {
        if (GET_SC_WPARAM(wParam) != SC_CLOSE)
            return 0;
        break;
    }
    case WM_STYLECHANGING: {
        LPSTYLESTRUCT ss = (LPSTYLESTRUCT)lParam;
        switch (wParam)
        {
        case GWL_EXSTYLE: {
            ss->styleNew = 0;
            break;
        }
        case GWL_STYLE: {
            ss->styleNew = WS_POPUP | WS_VISIBLE | WS_CLIPCHILDREN;
            break;
        }
        }
        return 0;
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

        SetWindowPos(g_Window, NULL, x, y, cx, cy, SWP_SHOWWINDOW);
        return 0;
    }
    case WM_WINDOWPOSCHANGING: {
        PWINDOWPOS wp = (PWINDOWPOS)lParam;
        if ((wp->flags & SWP_NOMOVE && wp->flags & SWP_NOSIZE))
            return 0;

        MONITORINFO mi = {sizeof(MONITORINFO)};
        RECT rc = {wp->x, wp->y, wp->x + wp->cx, wp->y + wp->cy};
        GetMonitorInfoW(MonitorFromRect(&rc, MONITOR_DEFAULTTONEAREST), &mi);

        wp->flags &= ~SWP_NOMOVE;
        wp->flags &= ~SWP_NOSIZE;

        wp->x = mi.rcMonitor.left;
        wp->y = mi.rcMonitor.top;

        wp->cx = mi.rcMonitor.right - wp->x;
        wp->cy = mi.rcMonitor.bottom - wp->y;
        return 0;
    }
    }
    return CallWindowProcW(g_WndProc, hWnd, uMsg, wParam, lParam);
}