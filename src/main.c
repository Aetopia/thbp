#include <dwmapi.h>

#include "d3d9.c"
#include "hooks.c"
#include "dinput8.c"

VOID MAIN_DllMain(VOID)
{
    WNDCLASSW wc = {
        .lpszClassName = L" ",
        .lpfnWndProc = DefWindowProcW,
        .hCursor = LoadCursorW(NULL, IDC_ARROW),
        .hbrBackground = GetStockObject(BLACK_BRUSH),
    };

    RegisterClassW(&wc);
    DwmEnableMMCSS(TRUE);

    SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);
}

BOOL WINAPI DllMain(PVOID lib, DWORD rsn, PVOID pv)
{
    if (rsn == DLL_PROCESS_ATTACH)
    {
        DisableThreadLibraryCalls(lib);
        MAIN_DllMain(), HOOKS_DllMain();
        D3D9_DllMain(), DINPUT8_DllMain();
    }
    return TRUE;
}