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
    case WM_ERASEBKGND:
        return DefWindowProcW(hWnd, uMsg, wParam, lParam);

    case WM_SIZE:
    case WM_SYSCOMMAND:
        if (uMsg == WM_SIZE || GET_SC_WPARAM(wParam) != SC_CLOSE)
            return 0;
        break;

    case WM_SETCURSOR:
    case WM_NCACTIVATE:
        if (g_FullScreen && hWnd == GetForegroundWindow())
        {
            SetCursor(NULL);
            if (uMsg == WM_SETCURSOR)
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

            ss->styleNew |= !g_FullScreen * WS_CAPTION;
            ss->styleNew |= !g_FullScreen * WS_SYSMENU;
            break;
        }
        return 0;

    case WM_WINDOWPOSCHANGED:
        RECT cr = {};
        GetClientRect(hWnd, &cr);

        INT cx = cr.right;
        INT cy = MulDiv(cx, 3, 4);

        if (cy > cr.bottom)
        {
            cy = cr.bottom;
            cx = MulDiv(cy, 4, 3);
        }

        INT x = (cr.right - cx) / 2;
        INT y = (cr.bottom - cy) / 2;

        SetWindowPos(g_Window, NULL, x, y, cx, cy, SWP_SHOWWINDOW);
        return 0;

    case WM_WINDOWPOSCHANGING:
        MONITORINFO mi = {sizeof(MONITORINFO)};
        GetMonitorInfoW(MonitorFromPoint((POINT){}, MONITOR_DEFAULTTOPRIMARY), &mi);

        PWINDOWPOS wp = (PWINDOWPOS)lParam;
        PRECT wr = g_FullScreen ? &mi.rcMonitor : &mi.rcWork;

        wp->flags &= ~SWP_NOMOVE;
        wp->flags &= ~SWP_NOSIZE;
        wp->flags |= SWP_DRAWFRAME;

        wp->x = wr->left, wp->cx = wr->right - wp->x;
        wp->y = wr->top, wp->cy = wr->bottom - wp->y;
        return 0;
    }
    return CallWindowProcW(g_WndProc, hWnd, uMsg, wParam, lParam);
}