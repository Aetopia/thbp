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

    PVOID og = VirtualAlloc(NULL, sizeof jmp * 2, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
    jmp.addr = (PVOID)((target + sizeof jmp) - (og + sizeof jmp * 2));

    __movsb(og, target, sizeof jmp);
    __movsb(og + sizeof jmp, (PVOID)&jmp, sizeof jmp);

    DWORD flags = {};
    VirtualProtect(target, sizeof jmp, PAGE_EXECUTE_READWRITE, &flags);

    jmp.addr = (PVOID)(detour - (target + sizeof jmp));
    __movsb(target, (PVOID)&jmp, sizeof jmp);

    VirtualProtect(target, sizeof jmp, flags, &(DWORD){});
    FlushInstructionCache(GetCurrentProcess(), target, sizeof jmp);

    return og;
}