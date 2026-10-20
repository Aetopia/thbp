#include <windef.h>
#include <wingdi.h>
#include <winuser.h>
#include <synchapi.h>
#include <memoryapi.h>
#include <processthreadsapi.h>

VOID WINAPI $Sleep(DWORD _)
{
    SleepEx(_ ? 1 : 0, FALSE);
}

INT WINAPI $ShowCursor(BOOL _)
{
    return _ ? 0 : -1;
}

BOOL WINAPI $EnumDisplaySettingsW(PVOID name, DWORD idx, PDEVMODEW dm)
{
    if (!EnumDisplaySettingsExW(name, idx, dm, 0))
        return FALSE;

    dm->dmPelsWidth = 1280;
    dm->dmPelsHeight = 960;

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
    FlushInstructionCache(GetCurrentProcess(), src, sizeof jmp);

    return ptr;
}