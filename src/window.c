#include <windef.h>
#include <winuser.h>
#include <winbase.h>

HWND g_Window = {};
BOOL g_Windowed = {};
WNDPROC g_WndProc = {};

LRESULT WINAPI WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_SIZE: {
        return 0;
    }
    case WM_ERASEBKGND: {
        return DefWindowProcW(hWnd, uMsg, wParam, lParam);
    }
    case WM_SETCURSOR: {
        if (g_Windowed || hWnd != GetForegroundWindow())
            return DefWindowProcW(hWnd, uMsg, wParam, lParam);
        return SetCursor(NULL), TRUE;
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
    case WM_STYLECHANGING: {
        LPSTYLESTRUCT ss = (LPSTYLESTRUCT)lParam;
        switch (wParam)
        {
        case GWL_EXSTYLE: {
            ss->styleNew = WS_EX_LEFT;
            break;
        }
        case GWL_STYLE: {
            ss->styleNew = g_Windowed ? WS_OVERLAPPEDWINDOW : WS_POPUP;
            ss->styleNew |= (WS_VISIBLE & ss->styleOld) | WS_CLIPCHILDREN;
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
        wp->flags |= SWP_FRAMECHANGED;

        if (g_Windowed || (wp->flags & SWP_NOMOVE && wp->flags & SWP_NOSIZE))
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