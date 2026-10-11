#include <windef.h>
#include <wingdi.h>
#include <winuser.h>
#include <winbase.h>

BOOL g_Windowed = {};

HWND g_Wnd = {};
WNDPROC g_WndProc = {};

LRESULT WINAPI WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
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
        if (!g_Windowed)
        {
            LPSTYLESTRUCT ss = (LPSTYLESTRUCT)lParam;

            if (wParam == GWL_EXSTYLE)
                ss->styleNew = WS_EX_LEFT;

            if (wParam == GWL_STYLE)
                ss->styleNew = WS_POPUP | (WS_VISIBLE * IsWindowVisible(hWnd));
        }
        break;

    case WM_WINDOWPOSCHANGED:
        RECT rc = {};
        GetClientRect(hWnd, &rc);

        INT cx = rc.right;
        INT cy = rc.bottom;

        if (!g_Windowed && (cy = MulDiv(cx, 3, 4)) > rc.bottom)
        {
            cy = rc.bottom;
            cx = MulDiv(cy, 4, 3);
        }

        INT x = (rc.right - cx) / 2;
        INT y = (rc.bottom - cy) / 2;

        SetWindowPos(g_Wnd, NULL, x, y, cx, cy, 0);
        break;

    case WM_WINDOWPOSCHANGING:
        PWINDOWPOS wp = (PWINDOWPOS)lParam;
        wp->flags |= SWP_DRAWFRAME;

        if (!g_Windowed)
        {
            MONITORINFO mi = {sizeof(MONITORINFO)};
            GetMonitorInfoW(MonitorFromPoint((POINT){}, MONITOR_DEFAULTTOPRIMARY), &mi);

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