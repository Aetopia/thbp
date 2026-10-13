#include <windef.h>
#include <wingdi.h>
#include <memoryapi.h>
#include <processthreadsapi.h>

INT WINAPI $ShowCursor(BOOL show)
{
    return show ? 1 : -1;
}

BOOL WINAPI $EnumDisplaySettingsW(PVOID name, DWORD idx, PDEVMODEW dm)
{
    dm->dmPelsWidth = 1280;
    dm->dmPelsHeight = 960;
    return TRUE;
}

PVOID CreateHook(PVOID src, PVOID dst)
{
    struct __attribute((packed))
    {
        BYTE code;
        PVOID addr;
    } jmp = {0xE9};

    PVOID ptr = VirtualAlloc(NULL, sizeof jmp * 2, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
    jmp.addr = (PVOID)((src + sizeof jmp) - (ptr + sizeof jmp * 2));

    __movsb(ptr, src, sizeof jmp);
    __movsb(ptr + sizeof jmp, (PVOID)&jmp, sizeof jmp);

    DWORD flags = {};
    VirtualProtect(src, sizeof jmp, PAGE_EXECUTE_READWRITE, &flags);

    jmp.addr = (PVOID)(dst - (src + sizeof jmp));
    __movsb(src, (PVOID)&jmp, sizeof jmp);

    VirtualProtect(src, sizeof jmp, flags, &(DWORD){});
    FlushInstructionCache(GetCurrentProcess(), src, sizeof jmp);

    return ptr;
}