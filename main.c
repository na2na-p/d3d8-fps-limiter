#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <mmsystem.h>

#ifdef _MSC_VER
    #include <intrin.h>
    #define FORCE_INLINE static __forceinline
    #pragma comment(lib, "winmm.lib")
#else
    #include <x86intrin.h>
    #define FORCE_INLINE static inline __attribute__((always_inline))
#endif

#ifndef CREATE_WAITABLE_TIMER_HIGH_RESOLUTION
#define CREATE_WAITABLE_TIMER_HIGH_RESOLUTION 0x00000002
#endif

#define VTABLE_INDEX_CREATEDEVICE 15
#define VTABLE_INDEX_PRESENT      15
#define BUSYWAIT_MARGIN_MS        0.5
#define SLEEP_MARGIN_MS           2.0
#define TIMER_RESOLUTION_MS       1
#define RESYNC_THRESHOLD_MS       500.0
#define CONFIG_FILENAME           "d3d8_limiter.ini"
#define CONFIG_SECTION            "limiter"
#define CONFIG_KEY_FPS            "fps"
#define DEFAULT_FPS               60

typedef HANDLE (WINAPI *PFN_CreateWaitableTimerExW)(
    LPSECURITY_ATTRIBUTES, LPCWSTR, DWORD, DWORD);

typedef struct IDirect3D8 { void **lpVtbl; } IDirect3D8;
typedef struct IDirect3DDevice8 { void **lpVtbl; } IDirect3DDevice8;

typedef IDirect3D8* (WINAPI *PFN_Direct3DCreate8)(UINT);
typedef HRESULT (WINAPI *PFN_CreateDevice)(IDirect3D8*, UINT, DWORD, HWND, DWORD, void*, IDirect3DDevice8**);
typedef HRESULT (WINAPI *PFN_Present)(IDirect3DDevice8*, const RECT*, const RECT*, HWND, void*);

static HMODULE hRealD3D8 = NULL;
static PFN_Direct3DCreate8 Real_Direct3DCreate8 = NULL;

static LARGE_INTEGER g_freq;
static LONGLONG g_nextFrameTime;
static LONGLONG g_targetFrameTicks = 0;
static int g_targetFPS = DEFAULT_FPS;
static BOOL g_initialized = FALSE;

static BOOL g_timerResolutionSet = FALSE;
static HANDLE g_hTimer = NULL;
static BOOL g_useHighResTimer = FALSE;
static LONGLONG g_busywaitMargin = 0;
static LONGLONG g_resyncThreshold = 0;

// 実行時の分岐を避けるため関数ポインタで処理を切り替え
static void (*DoFrameLimit_Impl)(void) = NULL;

static PFN_Present Real_Present = NULL;
static void** g_presentVtableEntry = NULL;
static PFN_CreateDevice Real_CreateDevice = NULL;
static void** g_createDeviceVtableEntry = NULL;

static void DoFrameLimit_HighRes(void);
static void DoFrameLimit_Fallback(void);
static HRESULT WINAPI Hooked_Present(IDirect3DDevice8*, const RECT*, const RECT*, HWND, void*);
static HRESULT WINAPI Hooked_CreateDevice(IDirect3D8*, UINT, DWORD, HWND, DWORD, void*, IDirect3DDevice8**);

static void LoadConfig(void) {
    char path[MAX_PATH];
    GetModuleFileNameA(NULL, path, MAX_PATH);
    char *lastSlash = strrchr(path, '\\');
    if (lastSlash) {
        size_t prefix_len = lastSlash - path + 1;
        size_t config_len = strlen(CONFIG_FILENAME);
        if (prefix_len + config_len < MAX_PATH) {
            strcpy(lastSlash + 1, CONFIG_FILENAME);
            g_targetFPS = GetPrivateProfileIntA(CONFIG_SECTION, CONFIG_KEY_FPS, DEFAULT_FPS, path);
        }
    }
}

static void InitFrameLimiter(void) {
    if (g_initialized) return;

    QueryPerformanceFrequency(&g_freq);

    LoadConfig();

    if (g_targetFPS > 0) {
        double targetFrameTime = (double)g_freq.QuadPart / (double)g_targetFPS;
        g_targetFrameTicks = (LONGLONG)targetFrameTime;
    }

    g_nextFrameTime = 0;
    g_resyncThreshold = (LONGLONG)(g_freq.QuadPart * RESYNC_THRESHOLD_MS / 1000.0);

    // Windows 10 1803+のみ利用可能
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

    if (g_useHighResTimer && g_hTimer) {
        g_busywaitMargin = (LONGLONG)(g_freq.QuadPart * BUSYWAIT_MARGIN_MS / 1000.0);
        DoFrameLimit_Impl = DoFrameLimit_HighRes;
    } else {
        if (timeBeginPeriod(TIMER_RESOLUTION_MS) == TIMERR_NOERROR) {
            g_timerResolutionSet = TRUE;
        }
        DoFrameLimit_Impl = DoFrameLimit_Fallback;
    }

    g_initialized = TRUE;
}

static void ShutdownFrameLimiter(void) {
    if (g_hTimer) {
        CloseHandle(g_hTimer);
        g_hTimer = NULL;
    }
    if (g_timerResolutionSet) {
        timeEndPeriod(TIMER_RESOLUTION_MS);
        g_timerResolutionSet = FALSE;
    }
}

static void DoFrameLimit_HighRes(void) {
    LARGE_INTEGER now;
    QueryPerformanceCounter(&now);

    if (g_nextFrameTime == 0 || (now.QuadPart - g_nextFrameTime) > g_resyncThreshold) {
        g_nextFrameTime = now.QuadPart + g_targetFrameTicks;
        return;
    }

    g_nextFrameTime += g_targetFrameTicks;

    if (g_nextFrameTime <= now.QuadPart) {
        LONGLONG delay = now.QuadPart - g_nextFrameTime;
        LONGLONG framesToSkip = delay / g_targetFrameTicks + 1;
        g_nextFrameTime += framesToSkip * g_targetFrameTicks;
    }

    LONGLONG remaining = g_nextFrameTime - now.QuadPart;

    if (remaining > g_busywaitMargin) {
        LARGE_INTEGER dueTime;
        dueTime.QuadPart = -(LONGLONG)((remaining - g_busywaitMargin) * 10000000LL / g_freq.QuadPart);
        if (SetWaitableTimer(g_hTimer, &dueTime, 0, NULL, NULL, FALSE)) {
            WaitForSingleObject(g_hTimer, INFINITE);
        }
    }

    do {
        _mm_pause();
        QueryPerformanceCounter(&now);
    } while (now.QuadPart < g_nextFrameTime);
}

static void DoFrameLimit_Fallback(void) {
    LARGE_INTEGER now;
    QueryPerformanceCounter(&now);

    if (g_nextFrameTime == 0 || (now.QuadPart - g_nextFrameTime) > g_resyncThreshold) {
        g_nextFrameTime = now.QuadPart + g_targetFrameTicks;
        return;
    }

    g_nextFrameTime += g_targetFrameTicks;

    if (g_nextFrameTime <= now.QuadPart) {
        LONGLONG delay = now.QuadPart - g_nextFrameTime;
        LONGLONG framesToSkip = delay / g_targetFrameTicks + 1;
        g_nextFrameTime += framesToSkip * g_targetFrameTicks;
    }

    LONGLONG remaining = g_nextFrameTime - now.QuadPart;

    double remainingMs = (double)remaining * 1000.0 / (double)g_freq.QuadPart;
    if (remainingMs > SLEEP_MARGIN_MS) {
        Sleep((DWORD)(remainingMs - SLEEP_MARGIN_MS));
    }

    do {
        _mm_pause();
        QueryPerformanceCounter(&now);
    } while (now.QuadPart < g_nextFrameTime);
}

FORCE_INLINE void DoFrameLimit(void) {
    if (g_targetFPS > 0 && DoFrameLimit_Impl) {
        DoFrameLimit_Impl();
    }
}

static HRESULT WINAPI Hooked_Present(IDirect3DDevice8 *This, const RECT *src, const RECT *dst, HWND hwnd, void *dirty) {
    HRESULT hr = Real_Present(This, src, dst, hwnd, dirty);
    DoFrameLimit();
    return hr;
}

static void InstallPresentHook(IDirect3DDevice8 *device) {
    InitFrameLimiter();

    void **vtbl = device->lpVtbl;
    g_presentVtableEntry = &vtbl[VTABLE_INDEX_PRESENT];
    Real_Present = (PFN_Present)vtbl[VTABLE_INDEX_PRESENT];

    DWORD oldProtect;
    VirtualProtect(g_presentVtableEntry, sizeof(void*), PAGE_READWRITE, &oldProtect);
    *g_presentVtableEntry = (void*)Hooked_Present;
    VirtualProtect(g_presentVtableEntry, sizeof(void*), oldProtect, &oldProtect);
}

static HRESULT WINAPI Hooked_CreateDevice(IDirect3D8 *This, UINT adapter, DWORD type, HWND hwnd,
                                          DWORD flags, void *params, IDirect3DDevice8 **device) {
    HRESULT hr = Real_CreateDevice(This, adapter, type, hwnd, flags, params, device);

    if (SUCCEEDED(hr) && device && *device) {
        InstallPresentHook(*device);
    }

    return hr;
}

static void InstallCreateDeviceHook(IDirect3D8 *d3d8) {
    void **vtbl = d3d8->lpVtbl;
    g_createDeviceVtableEntry = &vtbl[VTABLE_INDEX_CREATEDEVICE];
    Real_CreateDevice = (PFN_CreateDevice)vtbl[VTABLE_INDEX_CREATEDEVICE];

    DWORD oldProtect;
    VirtualProtect(g_createDeviceVtableEntry, sizeof(void*), PAGE_READWRITE, &oldProtect);
    *g_createDeviceVtableEntry = (void*)Hooked_CreateDevice;
    VirtualProtect(g_createDeviceVtableEntry, sizeof(void*), oldProtect, &oldProtect);
}

__declspec(dllexport) IDirect3D8* WINAPI Wrapper_Direct3DCreate8(UINT SDKVersion) {
    if (!hRealD3D8) {
        char path[MAX_PATH];
        UINT syslen = GetSystemDirectoryA(path, MAX_PATH);
        if (syslen == 0 || syslen + 10 >= MAX_PATH) return NULL;

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
    (void)hInst;
    (void)reserved;

    switch (reason) {
    case DLL_PROCESS_DETACH:
        ShutdownFrameLimiter();
        if (hRealD3D8) {
            FreeLibrary(hRealD3D8);
            hRealD3D8 = NULL;
        }
        break;
    }

    return TRUE;
}
