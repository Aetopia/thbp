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
    case WM_ERASEBKGND:
        return DefWindowProcW(hWnd, uMsg, wParam, lParam);

    case WM_SETCURSOR:
        while (ShowCursor(TRUE) < 0)
            _mm_pause();

        if (g_FullScreen && hWnd == GetForegroundWindow())
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
            ss->styleNew = WS_VISIBLE & ss->styleOld | WS_CLIPCHILDREN;
            ss->styleNew |= g_FullScreen ? WS_POPUP : WS_OVERLAPPEDWINDOW;
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

        if (g_FullScreen)
        {
            MONITORINFO mi = {sizeof(MONITORINFO)};
            GetMonitorInfoW(MonitorFromWindow(hWnd, MONITOR_DEFAULTTONEAREST), &mi);

            INT x = mi.rcMonitor.left;
            INT y = mi.rcMonitor.top;

            INT cx = mi.rcMonitor.right - x;
            INT cy = mi.rcMonitor.bottom - y;

            SetWindowPos(hWnd, NULL, x, y, cx, cy, SWP_NOZORDER);
        }
        return 0;
    }
    return CallWindowProcW(g_WndProc, hWnd, uMsg, wParam, lParam);
}