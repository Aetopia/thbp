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
    case WM_ERASEBKGND:
        return DefWindowProcW(hWnd, uMsg, wParam, lParam);

    case WM_NCACTIVATE:
        if (hWnd == GetForegroundWindow())
            SetCursor(NULL);
        break;

    case WM_SETCURSOR:
        if (hWnd == GetForegroundWindow())
        {
            SetCursor(NULL);
            return TRUE;
        }
        return DefWindowProcW(hWnd, uMsg, wParam, lParam);

    case WM_STYLECHANGING:
        LPSTYLESTRUCT ss = (LPSTYLESTRUCT)lParam;
        switch (wParam)
        {
        case GWL_EXSTYLE:
            ss->styleNew = WS_EX_LEFT;
            break;

        case GWL_STYLE:
            ss->styleNew = WS_VISIBLE & ss->styleOld;
            ss->styleNew |= WS_CLIPCHILDREN | WS_POPUP;
            break;
        }
        return 0;

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

        SetWindowPos(g_Window, NULL, x, y, cx, cy, SWP_SHOWWINDOW);
        return 0;

    case WM_WINDOWPOSCHANGING:
        MONITORINFO mi = {sizeof(MONITORINFO)};
        GetMonitorInfoW(MonitorFromPoint((POINT){}, MONITOR_DEFAULTTOPRIMARY), &mi);

        PWINDOWPOS wp = (PWINDOWPOS)lParam;

        wp->flags &= ~SWP_NOMOVE;
        wp->flags &= ~SWP_NOSIZE;

        wp->x = mi.rcMonitor.left;
        wp->y = mi.rcMonitor.top;

        wp->cx = mi.rcMonitor.right - wp->x;
        wp->cy = mi.rcMonitor.bottom - wp->y;
        return 0;
    }
    return CallWindowProcW(g_WndProc, hWnd, uMsg, wParam, lParam);
}