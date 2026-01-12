/*
 * D3D8 Frame Rate Limiter using inline hook on Present
 */

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

/* D3D8 types */
typedef struct IDirect3D8 { void **lpVtbl; } IDirect3D8;
typedef struct IDirect3DDevice8 { void **lpVtbl; } IDirect3DDevice8;

typedef IDirect3D8* (WINAPI *PFN_Direct3DCreate8)(UINT);
typedef HRESULT (WINAPI *PFN_CreateDevice)(IDirect3D8*, UINT, DWORD, HWND, DWORD, void*, IDirect3DDevice8**);
typedef HRESULT (WINAPI *PFN_Present)(IDirect3DDevice8*, const RECT*, const RECT*, HWND, void*);

static PFN_Direct3DCreate8 Real_Direct3DCreate8 = NULL;
static HMODULE hRealD3D8 = NULL;

/* Frame limiter */
static LARGE_INTEGER g_freq;
static LARGE_INTEGER g_lastTime;
static int g_targetFPS = 60;
static BOOL g_initialized = FALSE;

/* Hook state */
static PFN_Present Real_Present = NULL;
static BYTE g_originalBytes[5];
static void* g_presentAddr = NULL;

static void LoadConfig(void) {
    char path[MAX_PATH];
    GetModuleFileNameA(NULL, path, MAX_PATH);
    char *lastSlash = strrchr(path, '\\');
    if (lastSlash) {
        strcpy(lastSlash + 1, "dec.ini");
        g_targetFPS = GetPrivateProfileIntA("global", "Limit", 60, path);
    }
}

static void InitFrameLimiter(void) {
    if (!g_initialized) {
        QueryPerformanceFrequency(&g_freq);
        QueryPerformanceCounter(&g_lastTime);
        LoadConfig();
        g_initialized = TRUE;
    }
}

static void DoFrameLimit(void) {
    if (g_targetFPS <= 0) return;

    LARGE_INTEGER now;
    QueryPerformanceCounter(&now);

    double elapsed = (double)(now.QuadPart - g_lastTime.QuadPart) / (double)g_freq.QuadPart;
    double targetTime = 1.0 / (double)g_targetFPS;

    if (elapsed < targetTime) {
        double sleepMs = (targetTime - elapsed) * 1000.0;
        if (sleepMs > 1.0) {
            Sleep((DWORD)(sleepMs - 1.0));
        }
        do {
            QueryPerformanceCounter(&now);
            elapsed = (double)(now.QuadPart - g_lastTime.QuadPart) / (double)g_freq.QuadPart;
        } while (elapsed < targetTime);
    }

    QueryPerformanceCounter(&g_lastTime);
}

/* Forward declarations */
static HRESULT WINAPI Hooked_Present(IDirect3DDevice8*, const RECT*, const RECT*, HWND, void*);
static HRESULT WINAPI Hooked_CreateDevice(IDirect3D8*, UINT, DWORD, HWND, DWORD, void*, IDirect3DDevice8**);

/* Trampoline to call original Present */
static HRESULT CallOriginalPresent(IDirect3DDevice8 *This, const RECT *src, const RECT *dst, HWND hwnd, void *dirty) {
    DWORD oldProtect;
    HRESULT hr;

    /* Restore original bytes temporarily */
    VirtualProtect(g_presentAddr, 5, PAGE_EXECUTE_READWRITE, &oldProtect);
    memcpy(g_presentAddr, g_originalBytes, 5);
    VirtualProtect(g_presentAddr, 5, oldProtect, &oldProtect);

    /* Call original */
    hr = Real_Present(This, src, dst, hwnd, dirty);

    /* Reinstall hook */
    VirtualProtect(g_presentAddr, 5, PAGE_EXECUTE_READWRITE, &oldProtect);
    BYTE jmp[5];
    jmp[0] = 0xE9;
    DWORD rel = (DWORD)((BYTE*)Hooked_Present - (BYTE*)g_presentAddr - 5);
    memcpy(&jmp[1], &rel, 4);
    memcpy(g_presentAddr, jmp, 5);
    VirtualProtect(g_presentAddr, 5, oldProtect, &oldProtect);

    return hr;
}

static HRESULT WINAPI Hooked_Present(IDirect3DDevice8 *This, const RECT *src, const RECT *dst, HWND hwnd, void *dirty) {
    HRESULT hr = CallOriginalPresent(This, src, dst, hwnd, dirty);
    DoFrameLimit();
    return hr;
}

static void InstallPresentHook(IDirect3DDevice8 *device) {
    InitFrameLimiter();

    /* Get Present address from vtable */
    void **vtbl = device->lpVtbl;
    g_presentAddr = vtbl[15];  /* Present is index 15 */
    Real_Present = (PFN_Present)g_presentAddr;

    /* Save original bytes */
    memcpy(g_originalBytes, g_presentAddr, 5);

    /* Write jump to our hook */
    DWORD oldProtect;
    VirtualProtect(g_presentAddr, 5, PAGE_EXECUTE_READWRITE, &oldProtect);

    BYTE jmp[5];
    jmp[0] = 0xE9;
    DWORD rel = (DWORD)((BYTE*)Hooked_Present - (BYTE*)g_presentAddr - 5);
    memcpy(&jmp[1], &rel, 4);
    memcpy(g_presentAddr, jmp, 5);

    VirtualProtect(g_presentAddr, 5, oldProtect, &oldProtect);
}

/* Hooked CreateDevice */
static PFN_CreateDevice Real_CreateDevice = NULL;
static BYTE g_createDeviceOriginal[5];
static void* g_createDeviceAddr = NULL;

static HRESULT CallOriginalCreateDevice(IDirect3D8 *This, UINT adapter, DWORD type, HWND hwnd,
                                        DWORD flags, void *params, IDirect3DDevice8 **device) {
    DWORD oldProtect;
    HRESULT hr;

    VirtualProtect(g_createDeviceAddr, 5, PAGE_EXECUTE_READWRITE, &oldProtect);
    memcpy(g_createDeviceAddr, g_createDeviceOriginal, 5);
    VirtualProtect(g_createDeviceAddr, 5, oldProtect, &oldProtect);

    hr = Real_CreateDevice(This, adapter, type, hwnd, flags, params, device);

    VirtualProtect(g_createDeviceAddr, 5, PAGE_EXECUTE_READWRITE, &oldProtect);
    BYTE jmp[5];
    jmp[0] = 0xE9;
    DWORD rel = (DWORD)((BYTE*)Hooked_CreateDevice - (BYTE*)g_createDeviceAddr - 5);
    memcpy(&jmp[1], &rel, 4);
    memcpy(g_createDeviceAddr, jmp, 5);
    VirtualProtect(g_createDeviceAddr, 5, oldProtect, &oldProtect);

    return hr;
}

static HRESULT WINAPI Hooked_CreateDevice(IDirect3D8 *This, UINT adapter, DWORD type, HWND hwnd,
                                          DWORD flags, void *params, IDirect3DDevice8 **device) {
    HRESULT hr = CallOriginalCreateDevice(This, adapter, type, hwnd, flags, params, device);

    if (SUCCEEDED(hr) && device && *device) {
        InstallPresentHook(*device);
    }

    return hr;
}

static void InstallCreateDeviceHook(IDirect3D8 *d3d8) {
    void **vtbl = d3d8->lpVtbl;
    g_createDeviceAddr = vtbl[15];  /* CreateDevice is index 15 */
    Real_CreateDevice = (PFN_CreateDevice)g_createDeviceAddr;

    memcpy(g_createDeviceOriginal, g_createDeviceAddr, 5);

    DWORD oldProtect;
    VirtualProtect(g_createDeviceAddr, 5, PAGE_EXECUTE_READWRITE, &oldProtect);

    BYTE jmp[5];
    jmp[0] = 0xE9;
    DWORD rel = (DWORD)((BYTE*)Hooked_CreateDevice - (BYTE*)g_createDeviceAddr - 5);
    memcpy(&jmp[1], &rel, 4);
    memcpy(g_createDeviceAddr, jmp, 5);

    VirtualProtect(g_createDeviceAddr, 5, oldProtect, &oldProtect);
}

__declspec(dllexport) IDirect3D8* WINAPI Wrapper_Direct3DCreate8(UINT SDKVersion) {
    if (!hRealD3D8) {
        char path[MAX_PATH];
        GetSystemDirectoryA(path, MAX_PATH);
        strcat(path, "\\d3d8.dll");
        hRealD3D8 = LoadLibraryA(path);
        if (!hRealD3D8) return NULL;

        Real_Direct3DCreate8 = (PFN_Direct3DCreate8)GetProcAddress(hRealD3D8, "Direct3DCreate8");
        if (!Real_Direct3DCreate8) return NULL;
    }

    IDirect3D8 *d3d8 = Real_Direct3DCreate8(SDKVersion);
    if (!d3d8) return NULL;

    InstallCreateDeviceHook(d3d8);

    return d3d8;
}

BOOL WINAPI DllMain(HINSTANCE hInst, DWORD reason, LPVOID reserved) {
    (void)hInst; (void)reserved;
    if (reason == DLL_PROCESS_DETACH && hRealD3D8) {
        FreeLibrary(hRealD3D8);
    }
    return TRUE;
}
