#pragma once
#include <memoryapi.h>
#include <processthreadsapi.h>

PVOID CreateHook(PVOID target, PVOID detour)
{
    struct __attribute__((packed))
    {
        BYTE code;
        PVOID addr;
    } jmp = {.code = 0xE9};

    PVOID ptr = VirtualAlloc(NULL, sizeof jmp * 2, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
    jmp.addr = (PVOID)((target + sizeof jmp) - (ptr - sizeof jmp * 2));

    __movsb(ptr, target, sizeof jmp);
    __movsb(ptr + sizeof jmp, (PVOID)&jmp, sizeof jmp);

    DWORD flags = {};
    VirtualProtect(target, sizeof jmp, PAGE_EXECUTE_READWRITE, &flags);

    jmp.addr = (PVOID)(detour - (ptr + sizeof jmp));
    __movsb(target, (PVOID)&jmp, sizeof jmp);

    VirtualProtect(target, sizeof jmp, flags, &(DWORD){});
    FlushInstructionCache(GetCurrentProcess(), NULL, 0);

    return ptr;
}