#pragma once
#include <memoryapi.h>
#include <processthreadsapi.h>

PVOID CreateHook(PVOID src, PVOID dst)
{
    struct __attribute__((packed))
    {
        BYTE code;
        PVOID addr;
    } jmp = {.code = 0xE9};

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