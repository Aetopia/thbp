#include <windef.h>
#include <winuser.h>
#include <winbase.h>

HWND g_Window = {};
BOOL g_FullScreen = {};
WNDPROC g_WndProc = {};

LRESULT WINAPI WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_SIZE:
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
    case WM_SETCURSOR: {
        while (ShowCursor(TRUE) < 0)
            _mm_pause();

        if (!g_FullScreen || hWnd != GetForegroundWindow())
            return DefWindowProcW(hWnd, uMsg, wParam, lParam);

        SetCursor(NULL);
        return TRUE;
    }
    case WM_STYLECHANGING: {
        LPSTYLESTRUCT ss = (LPSTYLESTRUCT)lParam;

        if (wParam == GWL_EXSTYLE)
        {
            ss->styleNew = WS_EX_LEFT;
            return 0;
        }

        if (wParam == GWL_STYLE)
        {
            ss->styleNew = (WS_VISIBLE & ss->styleOld) | WS_CLIPCHILDREN;
            ss->styleNew |= g_FullScreen ? WS_POPUP : WS_OVERLAPPEDWINDOW;
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
        UINT flags = SWP_NOMOVE | SWP_NOSIZE;

        if (!g_FullScreen || (wp->flags & flags) == flags)
            return 0;

        MONITORINFO mi = {sizeof(MONITORINFO)};
        RECT rc = {wp->x, wp->y, wp->x + wp->cx, wp->y + wp->cy};
        GetMonitorInfoW(MonitorFromRect(&rc, MONITOR_DEFAULTTONEAREST), &mi);

        wp->x = mi.rcMonitor.left;
        wp->y = mi.rcMonitor.top;

        wp->cx = mi.rcMonitor.right - wp->x;
        wp->cy = mi.rcMonitor.bottom - wp->y;

        wp->flags &= ~flags;
        return 0;
    }
    }
    return CallWindowProcW(g_WndProc, hWnd, uMsg, wParam, lParam);
}