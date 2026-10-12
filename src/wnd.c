#include <windef.h>
#include <winuser.h>
#include <winbase.h>

UINT g_Width = {};
UINT g_Height = {};
BOOL g_Windowed = {};

HWND g_Wnd = {};
WNDPROC g_WndProc = {};

LRESULT WINAPI WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
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
            ss->styleNew = WS_VISIBLE * IsWindowVisible(hWnd);
            ss->styleNew |= g_Windowed ? (WS_CAPTION | WS_SYSMENU) : WS_POPUP;
            break;
        }
        break;

    case WM_WINDOWPOSCHANGED:
        RECT cr = {};
        GetClientRect(hWnd, &cr);

        INT cx = cr.right;
        INT cy = MulDiv(cx, g_Height, g_Width);

        if (cy > cr.bottom)
        {
            cy = cr.bottom;
            cx = MulDiv(cy, g_Width, g_Height);
        }

        INT x = (cr.right - cx) / 2;
        INT y = (cr.bottom - cy) / 2;

        SetWindowPos(g_Wnd, NULL, x, y, cx, cy, 0);
        break;

    case WM_WINDOWPOSCHANGING:
        PWINDOWPOS wp = (PWINDOWPOS)lParam;
        RECT wr = {wp->x, wp->y, wp->x + wp->cx, wp->y + wp->cy};

        wr.left *= g_Windowed, wr.top *= g_Windowed;
        wr.right *= g_Windowed, wr.bottom *= g_Windowed;

        MONITORINFO mi = {sizeof(MONITORINFO)};
        GetMonitorInfoW(MonitorFromRect(&wr, MONITOR_DEFAULTTOPRIMARY), &mi);

        wp->flags &= ~SWP_NOMOVE;
        wp->flags &= ~SWP_NOSIZE;
        wp->flags |= SWP_DRAWFRAME;

        wp->y = mi.rcMonitor.top;
        wp->x = mi.rcMonitor.left;

        wp->cx = mi.rcMonitor.right - wp->x;
        wp->cy = mi.rcMonitor.bottom - wp->y;
        break;
    }
    return CallWindowProcW(g_WndProc, hWnd, uMsg, wParam, lParam);
}