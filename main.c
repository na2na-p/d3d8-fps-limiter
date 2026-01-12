#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <mmsystem.h>
#include <intrin.h>

#pragma comment(lib, "winmm.lib")

#ifndef CREATE_WAITABLE_TIMER_HIGH_RESOLUTION
#define CREATE_WAITABLE_TIMER_HIGH_RESOLUTION 0x00000002
#endif

typedef HANDLE (WINAPI *PFN_CreateWaitableTimerExW)(
    LPSECURITY_ATTRIBUTES, LPCWSTR, DWORD, DWORD);

typedef struct IDirect3D8 { void **lpVtbl; } IDirect3D8;
typedef struct IDirect3DDevice8 { void **lpVtbl; } IDirect3DDevice8;

typedef IDirect3D8* (WINAPI *PFN_Direct3DCreate8)(UINT);
typedef HRESULT (WINAPI *PFN_CreateDevice)(IDirect3D8*, UINT, DWORD, HWND, DWORD, void*, IDirect3DDevice8**);
typedef HRESULT (WINAPI *PFN_Present)(IDirect3DDevice8*, const RECT*, const RECT*, HWND, void*);

static PFN_Direct3DCreate8 Real_Direct3DCreate8 = NULL;
static HMODULE hRealD3D8 = NULL;

static LARGE_INTEGER g_freq;
static LONGLONG g_nextFrameTime;
static double g_targetFrameTime;
static int g_targetFPS = 60;
static BOOL g_initialized = FALSE;
static BOOL g_timerResolutionSet = FALSE;
static HANDLE g_hTimer = NULL;
static BOOL g_useHighResTimer = FALSE;

static PFN_Present Real_Present = NULL;
static BYTE g_originalBytes[5];
static void* g_presentAddr = NULL;

static PFN_CreateDevice Real_CreateDevice = NULL;
static BYTE g_createDeviceOriginal[5];
static void* g_createDeviceAddr = NULL;

static HRESULT WINAPI Hooked_Present(IDirect3DDevice8*, const RECT*, const RECT*, HWND, void*);
static HRESULT WINAPI Hooked_CreateDevice(IDirect3D8*, UINT, DWORD, HWND, DWORD, void*, IDirect3DDevice8**);

static void LoadConfig(void) {
    char path[MAX_PATH];
    GetModuleFileNameA(NULL, path, MAX_PATH);
    char *lastSlash = strrchr(path, '\\');
    if (lastSlash) {
        strcpy(lastSlash + 1, "d3d8_limiter.ini");
        g_targetFPS = GetPrivateProfileIntA("limiter", "fps", 60, path);
    }
}

static void InitFrameLimiter(void) {
    if (!g_initialized) {
        QueryPerformanceFrequency(&g_freq);

        LARGE_INTEGER now;
        QueryPerformanceCounter(&now);
        g_nextFrameTime = now.QuadPart;

        LoadConfig();

        if (g_targetFPS > 0) {
            g_targetFrameTime = (double)g_freq.QuadPart / (double)g_targetFPS;
        }

        // Try high-resolution waitable timer (Windows 10 1803+)
        HMODULE hKernel32 = GetModuleHandleA("kernel32.dll");
        if (hKernel32) {
            PFN_CreateWaitableTimerExW pCreateWaitableTimerExW =
                (PFN_CreateWaitableTimerExW)GetProcAddress(hKernel32, "CreateWaitableTimerExW");
            if (pCreateWaitableTimerExW) {
                g_hTimer = pCreateWaitableTimerExW(NULL, NULL,
                    CREATE_WAITABLE_TIMER_HIGH_RESOLUTION, TIMER_ALL_ACCESS);
                if (g_hTimer) {
                    g_useHighResTimer = TRUE;
                }
            }
        }

        // Fallback: use timeBeginPeriod for Sleep accuracy
        if (!g_useHighResTimer) {
            if (timeBeginPeriod(1) == TIMERR_NOERROR) {
                g_timerResolutionSet = TRUE;
            }
        }

        g_initialized = TRUE;
    }
}

static void ShutdownFrameLimiter(void) {
    if (g_hTimer) {
        CloseHandle(g_hTimer);
        g_hTimer = NULL;
    }
    if (g_timerResolutionSet) {
        timeEndPeriod(1);
        g_timerResolutionSet = FALSE;
    }
}

static void DoFrameLimit(void) {
    if (g_targetFPS <= 0) return;

    LARGE_INTEGER now;
    QueryPerformanceCounter(&now);

    LONGLONG remaining = g_nextFrameTime - now.QuadPart;

    if (remaining > 0) {
        if (g_useHighResTimer && g_hTimer) {
            // Convert QPC ticks to 100-nanosecond intervals (negative = relative)
            // Leave 0.5ms margin for final busy-wait
            LONGLONG margin = g_freq.QuadPart / 2000;
            if (remaining > margin) {
                LARGE_INTEGER dueTime;
                dueTime.QuadPart = -(LONGLONG)((remaining - margin) * 10000000LL / g_freq.QuadPart);
                if (SetWaitableTimer(g_hTimer, &dueTime, 0, NULL, NULL, FALSE)) {
                    WaitForSingleObject(g_hTimer, INFINITE);
                }
            }
            // Final busy-wait for precision
            do {
                _mm_pause();
                QueryPerformanceCounter(&now);
            } while (now.QuadPart < g_nextFrameTime);
        } else {
            // Fallback: Sleep + busy-wait
            double remainingMs = (double)remaining * 1000.0 / (double)g_freq.QuadPart;
            if (remainingMs > 2.0) {
                Sleep((DWORD)(remainingMs - 2.0));
            }
            do {
                _mm_pause();
                QueryPerformanceCounter(&now);
            } while (now.QuadPart < g_nextFrameTime);
        }
    }

    g_nextFrameTime += (LONGLONG)g_targetFrameTime;

    // Prevent catch-up stutter when falling behind
    QueryPerformanceCounter(&now);
    LONGLONG halfFrame = (LONGLONG)(g_targetFrameTime * 0.5);
    if (now.QuadPart > g_nextFrameTime + halfFrame) {
        g_nextFrameTime = now.QuadPart;
    }
}

static HRESULT CallOriginalPresent(IDirect3DDevice8 *This, const RECT *src, const RECT *dst, HWND hwnd, void *dirty) {
    DWORD oldProtect;
    HRESULT hr;

    VirtualProtect(g_presentAddr, 5, PAGE_EXECUTE_READWRITE, &oldProtect);
    memcpy(g_presentAddr, g_originalBytes, 5);
    VirtualProtect(g_presentAddr, 5, oldProtect, &oldProtect);

    hr = Real_Present(This, src, dst, hwnd, dirty);

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

    void **vtbl = device->lpVtbl;
    g_presentAddr = vtbl[15];
    Real_Present = (PFN_Present)g_presentAddr;

    memcpy(g_originalBytes, g_presentAddr, 5);

    DWORD oldProtect;
    VirtualProtect(g_presentAddr, 5, PAGE_EXECUTE_READWRITE, &oldProtect);

    BYTE jmp[5];
    jmp[0] = 0xE9;
    DWORD rel = (DWORD)((BYTE*)Hooked_Present - (BYTE*)g_presentAddr - 5);
    memcpy(&jmp[1], &rel, 4);
    memcpy(g_presentAddr, jmp, 5);

    VirtualProtect(g_presentAddr, 5, oldProtect, &oldProtect);
}

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
    g_createDeviceAddr = vtbl[15];
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

    switch (reason) {
    case DLL_PROCESS_DETACH:
        ShutdownFrameLimiter();
        if (hRealD3D8) {
            FreeLibrary(hRealD3D8);
        }
        break;
    }

    return TRUE;
}
