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
    case WM_ERASEBKGND: {
        return DefWindowProcW(hWnd, uMsg, wParam, lParam);
    }
    case WM_SETCURSOR: {
        if (GetWindowLongW(hWnd, GWL_USERDATA))
            return DefWindowProcW(hWnd, uMsg, wParam, lParam);

        if (hWnd != GetForegroundWindow())
            return DefWindowProcW(hWnd, uMsg, wParam, lParam);

        return SetCursor(NULL), TRUE;
    }
    case WM_STYLECHANGING: {
        LPSTYLESTRUCT ws = (LPSTYLESTRUCT)lParam;
        switch (wParam)
        {
        case GWL_EXSTYLE: {
            ws->styleNew = WS_EX_APPWINDOW;
            break;
        }
        case GWL_STYLE: {
            ws->styleNew = (ws->styleNew & WS_VISIBLE) | WS_CLIPCHILDREN;
            ws->styleNew |= GetWindowLongW(hWnd, GWL_USERDATA) ? WS_TILEDWINDOW : WS_POPUP;
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

        SetWindowPos(g_Window, NULL, x, y, cx, cy, 0);
        return 0;
    }
    case WM_WINDOWPOSCHANGING: {
        if (GetWindowLongW(hWnd, GWL_USERDATA))
            return 0;

        PWINDOWPOS wp = (PWINDOWPOS)lParam;
        if (wp->flags & SWP_NOMOVE && wp->flags & SWP_NOSIZE)
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