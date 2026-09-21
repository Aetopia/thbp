#include "d3d9.c"
#include <dwmapi.h>
#include <dinput.h>
#include <dinputd.h>
#include <shlwapi.h>

HRESULT WINAPI (*g_SetCooperativeLevel)(PVOID, HWND, DWORD) = {};
HRESULT WINAPI (*g_DirectInput8Create)(PVOID, DWORD, LPCVOID, PVOID, PVOID) = {};

HRESULT WINAPI SetCooperativeLevel(PVOID this, HWND wnd, DWORD flags)
{
    flags &= ~DISCL_NOWINKEY;
    return g_SetCooperativeLevel(this, wnd, flags);
}

__declspec(dllexport) HRESULT WINAPI DirectInput8Create(HINSTANCE instance, DWORD version, REFIID iid, LPVOID *object,
                                                        LPUNKNOWN unknown)
{
    return g_DirectInput8Create(instance, version, iid, object, unknown);
}

BOOL WINAPI DllMain(HINSTANCE instance, DWORD reason, PVOID reserved)
{
    if (reason == DLL_PROCESS_ATTACH)
    {
        DisableThreadLibraryCalls(instance);
        SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);

        WCHAR path[MAX_PATH] = {};

        GetSystemDirectoryW(path, MAX_PATH);
        PathCombineW(path, path, L"dinput8.dll");

        g_DirectInput8Create = (PVOID)GetProcAddress(LoadLibraryW(path), "DirectInput8Create");

        LPDIRECTINPUT8W dinput = {};
        LPDIRECTINPUTDEVICE8W device = {};
        LPDIRECT3D9 d3d9 = Direct3DCreate9(D3D_SDK_VERSION);

        g_DirectInput8Create(instance, DIRECTINPUT_VERSION, &IID_IDirectInput8W, (PVOID)&dinput, NULL);
        dinput->lpVtbl->CreateDevice(dinput, &GUID_SysMouseEm, &device, NULL);

        g_CreateDevice = CreateHook(d3d9->lpVtbl->CreateDevice, CreateDevice);
        g_SetCooperativeLevel = CreateHook(device->lpVtbl->SetCooperativeLevel, SetCooperativeLevel);

        d3d9->lpVtbl->Release(d3d9);
        device->lpVtbl->Release(device);
        dinput->lpVtbl->Release(dinput);

        DwmEnableMMCSS(TRUE);
    }
    return TRUE;
}