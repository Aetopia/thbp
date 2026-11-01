#include <windef.h>
#include <wingdi.h>
#include <synchapi.h>
#include <memoryapi.h>
#include <processthreadsapi.h>

VOID WINAPI (*g_Sleep)(DWORD) = {};
BOOL WINAPI (*g_EnumDisplaySettingsW)(PVOID, DWORD, PVOID) = {};
BOOL WINAPI (*g_EnumDisplaySettingsA)(PVOID, DWORD, PVOID) = {};

INT WINAPI $ShowCursor(BOOL _)
{
    return _ ? 0 : -1;
}

VOID WINAPI $Sleep(DWORD _)
{
    g_Sleep(_ ? 1 : 0);
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

PVOID CreateDetour(PVOID src, PVOID dst)
{
    struct __attribute((packed))
    {
        BYTE code;
        ptrdiff_t addr;
    } jmp = {0xE9};

    PVOID ptr = VirtualAlloc(NULL, sizeof jmp * 2, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
    jmp.addr = (src + sizeof jmp) - (ptr + sizeof jmp * 2);

    __movsb(ptr, src, sizeof jmp);
    __movsb(ptr + sizeof jmp, (PVOID)&jmp, sizeof jmp);

    DWORD flags = {};
    VirtualProtect(src, sizeof jmp, PAGE_EXECUTE_READWRITE, &flags);

    jmp.addr = dst - (src + sizeof jmp);
    __movsb(src, (PVOID)&jmp, sizeof jmp);

    VirtualProtect(src, sizeof jmp, flags, &(DWORD){});
    FlushInstructionCache(GetCurrentProcess(), NULL, 0);

    return ptr;
}