#pragma once

#include <memoryapi.h>
#include <processthreadsapi.h>

PVOID CreateDetour(PVOID src, PVOID dst)
{
    struct __attribute((packed))
    {
        BYTE code;
        INT_PTR addr;
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