#include <windef.h>
#include <wingdi.h>
#include <winuser.h>
#include <winbase.h>

HWND g_Wnd = {};
BOOL g_FullScreen = {};
WNDPROC g_WndProc = {};

LRESULT WINAPI WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_SETCURSOR:
    case WM_NCACTIVATE:
        while (ShowCursor(TRUE) < 0)
            continue;

        if (g_FullScreen && hWnd == GetForegroundWindow())
        {
            SetCursor(NULL);

            if (uMsg == WM_SETCURSOR)
                return TRUE;

            if (uMsg == WM_NCACTIVATE)
                break;
        }

        return DefWindowProcW(hWnd, uMsg, wParam, lParam);

    case WM_SIZE:
    case WM_ERASEBKGND:
        return DefWindowProcW(hWnd, uMsg, wParam, lParam);

    case WM_SYSCOMMAND:
        switch (GET_SC_WPARAM(wParam))
        {
        case SC_KEYMENU:
        case SC_MOUSEMENU:
            return 0;
        }
        break;

    case WM_STYLECHANGING:
        LPSTYLESTRUCT ss = (LPSTYLESTRUCT)lParam;
        switch (wParam)
        {
        case GWL_EXSTYLE:
            ss->styleNew = WS_EX_LEFT;
            break;
        
        case GWL_STYLE: 
            ss->styleNew = WS_VISIBLE * IsWindowVisible(hWnd);
            ss->styleNew |= g_FullScreen ? WS_POPUP : WS_OVERLAPPEDWINDOW;
            break;
        }
        break;

    case WM_WINDOWPOSCHANGED:
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

    case WM_WINDOWPOSCHANGING:
        if (g_FullScreen)
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
        break;
    }
    return CallWindowProcW(g_WndProc, hWnd, uMsg, wParam, lParam);
}