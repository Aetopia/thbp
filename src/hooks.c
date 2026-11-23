#pragma once

#include <windef.h>
#include <wingdi.h>
#include <winuser.h>
#include <synchapi.h>

#include "detour.c"

VOID WINAPI (*g_Sleep)(DWORD) = {};
BOOL WINAPI (*g_EnumDisplaySettingsW)(PVOID, DWORD, PVOID) = {};
BOOL WINAPI (*g_EnumDisplaySettingsA)(PVOID, DWORD, PVOID) = {};

INT WINAPI $ShowCursor(BOOL show)
{
    return show ? 0 : -1;
}

BOOL WINAPI $EnableWindow(HWND hwnd, BOOL enable)
{
    return TRUE;
}

VOID WINAPI $Sleep(DWORD time)
{
    g_Sleep(time ? 1 : 0);
}

BOOL WINAPI $EnumDisplaySettingsA(PVOID name, DWORD idx, PDEVMODEA dm)
{
    if (!g_EnumDisplaySettingsA(name, idx, dm))
        return FALSE;

    dm->dmPelsWidth = 1280;
    dm->dmPelsHeight = 960;
    dm->dmDisplayFrequency = 0;

    return TRUE;
}

BOOL WINAPI $EnumDisplaySettingsW(PVOID name, DWORD idx, PDEVMODEW dm)
{
    if (!g_EnumDisplaySettingsW(name, idx, dm))
        return FALSE;

    dm->dmPelsWidth = 1280;
    dm->dmPelsHeight = 960;
    dm->dmDisplayFrequency = 0;

    return TRUE;
}

VOID HOOKS_DllMain(VOID)
{
    CreateDetour(ShowCursor, $ShowCursor);
    CreateDetour(EnableWindow, $EnableWindow);

    g_Sleep = CreateDetour(Sleep, $Sleep);
    g_EnumDisplaySettingsA = CreateDetour(EnumDisplaySettingsA, $EnumDisplaySettingsA);
    g_EnumDisplaySettingsW = CreateDetour(EnumDisplaySettingsW, $EnumDisplaySettingsW);
}