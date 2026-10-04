#include "d3d9.c"
#include <mfapi.h>
#include <dwmapi.h>
#include <dinput.h>
#include <dinputd.h>

PVOID __wrap_memcpy(PVOID dst, PVOID src, SIZE_T count)
{
    __movsb(dst, src, count);
    return dst;
}

PVOID __wrap_memset(PVOID dst, BYTE data, SIZE_T count)
{
    __stosb(dst, data, count);
    return dst;
}

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

        WNDCLASSW wc = {
            .lpszClassName = L" ",
            .lpfnWndProc = DefWindowProcW,
            .hCursor = LoadCursorW(NULL, IDC_ARROW),
            .hbrBackground = GetStockObject(BLACK_BRUSH),
        };

        RegisterClassW(&wc);
        DwmEnableMMCSS(TRUE);

        QueryPerformanceFrequency(&g_Frequency);
        g_Frequency.QuadPart = MFllMulDiv(g_Frequency.QuadPart, 1, 60, 0);

        WCHAR path[MAX_PATH] = {};
        GetSystemDirectoryW(path, MAX_PATH);

        HMODULE module = LoadLibraryW(lstrcatW(path, L"/DINPUT8"));
        g_DirectInput8Create = (PVOID)GetProcAddress(module, "DirectInput8Create");

        LPDIRECTINPUT8W dinput8 = {};
        LPDIRECTINPUTDEVICE8W device = {};
        LPDIRECT3D9 d3d9 = Direct3DCreate9(D3D_SDK_VERSION);

        g_DirectInput8Create(instance, DIRECTINPUT_VERSION, &IID_IDirectInput8W, (PVOID)&dinput8, NULL);
        dinput8->lpVtbl->CreateDevice(dinput8, &GUID_SysMouseEm, &device, NULL);

        MH_Initialize();

        MH_CreateHook(d3d9->lpVtbl->CreateDevice, CreateDevice, (PVOID)&g_CreateDevice);
        MH_CreateHook(device->lpVtbl->SetCooperativeLevel, SetCooperativeLevel, (PVOID)&g_SetCooperativeLevel);

        MH_QueueEnableHook(d3d9->lpVtbl->CreateDevice);
        MH_QueueEnableHook(device->lpVtbl->SetCooperativeLevel);

        MH_ApplyQueued();

        d3d9->lpVtbl->Release(d3d9);
        device->lpVtbl->Release(device);
        dinput8->lpVtbl->Release(dinput8);
    }
    return TRUE;
}