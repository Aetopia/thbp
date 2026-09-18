#pragma once
#include <windef.h>
#include <wingdi.h>
#include <winuser.h>
#include <winbase.h>

HWND g_Wnd = {};
WNDPROC g_WndProc = {};

LRESULT WINAPI WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    static BOOL s_flag = {};

    switch (uMsg)
    {
    case WM_PAINT: {
        PAINTSTRUCT paint = {};
        BeginPaint(hWnd, &paint);

        HBRUSH hbr = GetStockObject(BLACK_BRUSH);
        FillRect(paint.hdc, &paint.rcPaint, hbr);

        EndPaint(hWnd, &paint);
        break;
    }
    case WM_WINDOWPOSCHANGED:
        if (!s_flag)
        {
            s_flag = TRUE;

            MONITORINFO mi = {.cbSize = sizeof(MONITORINFO)};
            GetMonitorInfoW(MonitorFromWindow(hWnd, MONITOR_DEFAULTTONEAREST), &mi);

            INT px = mi.rcMonitor.right - mi.rcMonitor.left;
            INT py = mi.rcMonitor.bottom - mi.rcMonitor.top;
            SetWindowPos(hWnd, NULL, mi.rcMonitor.left, mi.rcMonitor.top, px, py, SWP_NOZORDER);

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
            SetWindowPos(g_Wnd, NULL, x, y, cx, cy, SWP_NOZORDER);

            s_flag = FALSE;
        }
        return 0;
    }
    
    return CallWindowProcW(g_WndProc, hWnd, uMsg, wParam, lParam);
}