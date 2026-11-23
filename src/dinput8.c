#pragma once

#include <dinput.h>
#include <dinputd.h>

#include "detour.c"

HRESULT WINAPI (*g_SetCooperativeLevel)(PVOID, HWND, DWORD) = {};
HRESULT WINAPI (*g_DirectInput8Create)(PVOID, DWORD, REFIID, PVOID, PVOID) = {};

HRESULT WINAPI SetCooperativeLevel(PVOID this, HWND hwnd, DWORD flags)
{
    flags &= ~DISCL_NOWINKEY;
    return g_SetCooperativeLevel(this, hwnd, flags);
}

__declspec(dllexport) HRESULT WINAPI DirectInput8Create(HMODULE lib, DWORD ver, REFIID iid, PVOID *obj, LPUNKNOWN unk)
{
    return g_DirectInput8Create(lib, ver, iid, obj, unk);
}

VOID DINPUT8_DllMain(VOID)
{
    WCHAR path[MAX_PATH] = {};
    GetSystemDirectoryW(path, MAX_PATH);

    HMODULE lib = LoadLibraryW(lstrcatW(path, L"/DINPUT8"));
    g_DirectInput8Create = (PVOID)GetProcAddress(lib, "DirectInput8Create");

    LPDIRECTINPUT8W di8 = {};
    DirectInput8Create(lib, DIRECTINPUT_VERSION, &IID_IDirectInput8W, (PVOID)&di8, NULL);

    LPDIRECTINPUTDEVICE8W dev = {};
    di8->lpVtbl->CreateDevice(di8, &GUID_SysMouseEm, &dev, NULL);

    g_SetCooperativeLevel = CreateDetour(dev->lpVtbl->SetCooperativeLevel, SetCooperativeLevel);

    dev->lpVtbl->Release(dev);
    di8->lpVtbl->Release(di8);
}