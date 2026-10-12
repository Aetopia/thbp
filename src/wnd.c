#include <windows.h>

BOOL g_Windowed = {};

HWND g_Wnd = {};
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
        if (!g_Windowed && hWnd == GetForegroundWindow())
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
            ss->styleNew |= WS_POPUP | WS_CLIPCHILDREN;

            if (g_Windowed)
                ss->styleNew |= WS_CAPTION | WS_SYSMENU;
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

        SetWindowPos(g_Wnd, NULL, x, y, cx, cy, 0);
        return 0;

    case WM_WINDOWPOSCHANGING:
        MONITORINFO mi = {sizeof(MONITORINFO)};
        GetMonitorInfoW(MonitorFromPoint((POINT){}, MONITOR_DEFAULTTOPRIMARY), &mi);

        PWINDOWPOS wp = (PWINDOWPOS)lParam;
        PRECT wr = g_Windowed ? &mi.rcWork : &mi.rcMonitor;

        wp->flags &= ~SWP_NOMOVE;
        wp->flags &= ~SWP_NOSIZE;
        wp->flags |= SWP_DRAWFRAME;

        wp->x = wr->left, wp->cx = wr->right - wp->x;
        wp->y = wr->top, wp->cy = wr->bottom - wp->y;
        return 0;
    }
    return CallWindowProcW(g_WndProc, hWnd, uMsg, wParam, lParam);
}