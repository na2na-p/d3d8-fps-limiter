#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <mmsystem.h>

// Compiler-specific settings
#ifdef _MSC_VER
    #include <intrin.h>
    #define FORCE_INLINE static __forceinline
    #pragma comment(lib, "winmm.lib")
#else
    // GCC/MinGW
    #include <x86intrin.h>
    #define FORCE_INLINE static inline __attribute__((always_inline))
#endif

// ============================================================================
// Constants
// ============================================================================

#ifndef CREATE_WAITABLE_TIMER_HIGH_RESOLUTION
#define CREATE_WAITABLE_TIMER_HIGH_RESOLUTION 0x00000002
#endif

// Direct3D8 vtable indices
#define VTABLE_INDEX_CREATEDEVICE 15
#define VTABLE_INDEX_PRESENT      15

// Frame limiter timing constants
#define BUSYWAIT_MARGIN_MS        0.5   // Margin before final busy-wait (high-res timer)
#define SLEEP_MARGIN_MS           2.0   // Margin before final busy-wait (fallback)
#define TIMER_RESOLUTION_MS       1     // timeBeginPeriod resolution

// Configuration file
#define CONFIG_FILENAME           "d3d8_limiter.ini"
#define CONFIG_SECTION            "limiter"
#define CONFIG_KEY_FPS            "fps"
#define DEFAULT_FPS               60

// ============================================================================
// Type Definitions
// ============================================================================

typedef HANDLE (WINAPI *PFN_CreateWaitableTimerExW)(
    LPSECURITY_ATTRIBUTES, LPCWSTR, DWORD, DWORD);

typedef struct IDirect3D8 { void **lpVtbl; } IDirect3D8;
typedef struct IDirect3DDevice8 { void **lpVtbl; } IDirect3DDevice8;

typedef IDirect3D8* (WINAPI *PFN_Direct3DCreate8)(UINT);
typedef HRESULT (WINAPI *PFN_CreateDevice)(IDirect3D8*, UINT, DWORD, HWND, DWORD, void*, IDirect3DDevice8**);
typedef HRESULT (WINAPI *PFN_Present)(IDirect3DDevice8*, const RECT*, const RECT*, HWND, void*);

// ============================================================================
// Global Variables
// ============================================================================

// Real D3D8 DLL and function
static HMODULE hRealD3D8 = NULL;
static PFN_Direct3DCreate8 Real_Direct3DCreate8 = NULL;

// Frame limiter state
static LARGE_INTEGER g_freq;
static LONGLONG g_nextFrameTime;
static LONGLONG g_targetFrameTicks = 0;  // Pre-calculated frame time in QPC ticks
static int g_targetFPS = DEFAULT_FPS;
static BOOL g_initialized = FALSE;

// Timer state
static BOOL g_timerResolutionSet = FALSE;
static HANDLE g_hTimer = NULL;
static BOOL g_useHighResTimer = FALSE;
static LONGLONG g_busywaitMargin = 0;  // Pre-calculated margin for busy-wait

// Frame limiter function pointer (avoids branch in hot path)
static void (*DoFrameLimit_Impl)(void) = NULL;

// Hook state
static PFN_Present Real_Present = NULL;
static void** g_presentVtableEntry = NULL;
static PFN_CreateDevice Real_CreateDevice = NULL;
static void** g_createDeviceVtableEntry = NULL;

// ============================================================================
// Forward Declarations
// ============================================================================

static void DoFrameLimit_HighRes(void);
static void DoFrameLimit_Fallback(void);
static HRESULT WINAPI Hooked_Present(IDirect3DDevice8*, const RECT*, const RECT*, HWND, void*);
static HRESULT WINAPI Hooked_CreateDevice(IDirect3D8*, UINT, DWORD, HWND, DWORD, void*, IDirect3DDevice8**);

// ============================================================================
// Configuration
// ============================================================================

static void LoadConfig(void) {
    char path[MAX_PATH];
    GetModuleFileNameA(NULL, path, MAX_PATH);
    char *lastSlash = strrchr(path, '\\');
    if (lastSlash) {
        size_t prefix_len = lastSlash - path + 1;
        size_t config_len = strlen(CONFIG_FILENAME);
        // Ensure buffer has space for config filename
        if (prefix_len + config_len < MAX_PATH) {
            strcpy(lastSlash + 1, CONFIG_FILENAME);
            g_targetFPS = GetPrivateProfileIntA(CONFIG_SECTION, CONFIG_KEY_FPS, DEFAULT_FPS, path);
        }
    }
}

// ============================================================================
// Frame Limiter Implementation
// ============================================================================

static void InitFrameLimiter(void) {
    if (g_initialized) return;

    // Initialize performance counter
    QueryPerformanceFrequency(&g_freq);

    LARGE_INTEGER now;
    QueryPerformanceCounter(&now);
    g_nextFrameTime = now.QuadPart;

    // Load target FPS from config file
    LoadConfig();

    if (g_targetFPS > 0) {
        // Pre-calculate frame time in ticks (avoids conversion every frame)
        double targetFrameTime = (double)g_freq.QuadPart / (double)g_targetFPS;
        g_targetFrameTicks = (LONGLONG)targetFrameTime;
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

    // Select appropriate frame limiter implementation and pre-calculate margin
    if (g_useHighResTimer && g_hTimer) {
        // High-resolution timer: pre-calculate margin for busy-wait
        g_busywaitMargin = (LONGLONG)(g_freq.QuadPart * BUSYWAIT_MARGIN_MS / 1000.0);
        DoFrameLimit_Impl = DoFrameLimit_HighRes;
    } else {
        // Fallback: use timeBeginPeriod for Sleep accuracy
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

// High-resolution timer implementation (no branching in hot path)
static void DoFrameLimit_HighRes(void) {
    LARGE_INTEGER now;
    QueryPerformanceCounter(&now);

    LONGLONG remaining = g_nextFrameTime - now.QuadPart;

    // Only wait if we're ahead of schedule
    // If we're behind, skip waiting entirely to avoid making things worse
    if (remaining > 0) {
        // Use pre-calculated margin to avoid computation
        if (remaining > g_busywaitMargin) {
            // Convert QPC ticks to 100-nanosecond intervals (negative = relative time)
            LARGE_INTEGER dueTime;
            dueTime.QuadPart = -(LONGLONG)((remaining - g_busywaitMargin) * 10000000LL / g_freq.QuadPart);
            if (SetWaitableTimer(g_hTimer, &dueTime, 0, NULL, NULL, FALSE)) {
                WaitForSingleObject(g_hTimer, INFINITE);
            }
        }
        // Final busy-wait for sub-millisecond precision
        do {
            _mm_pause();
            QueryPerformanceCounter(&now);
        } while (now.QuadPart < g_nextFrameTime);
    } else {
        // We're behind schedule - get current time for accurate adjustment
        QueryPerformanceCounter(&now);
    }

    // Always advance by exactly one frame interval - maintain strict fixed cadence
    // For high-FPS games (1000+ fps), this ensures perfectly even frame pacing
    g_nextFrameTime += g_targetFrameTicks;
}

// Fallback implementation using Sleep (no branching in hot path)
static void DoFrameLimit_Fallback(void) {
    LARGE_INTEGER now;
    QueryPerformanceCounter(&now);

    LONGLONG remaining = g_nextFrameTime - now.QuadPart;

    // Only wait if we're ahead of schedule
    if (remaining > 0) {
        double remainingMs = (double)remaining * 1000.0 / (double)g_freq.QuadPart;
        if (remainingMs > SLEEP_MARGIN_MS) {
            Sleep((DWORD)(remainingMs - SLEEP_MARGIN_MS));
        }
        // Final busy-wait for precision
        do {
            _mm_pause();
            QueryPerformanceCounter(&now);
        } while (now.QuadPart < g_nextFrameTime);
    }
    // If behind, skip waiting and proceed immediately

    // Always advance by exactly one frame interval - maintain strict fixed cadence
    // For high-FPS games (1000+ fps), this ensures perfectly even frame pacing
    g_nextFrameTime += g_targetFrameTicks;
}

// Main frame limiter entry point (inline wrapper)
FORCE_INLINE void DoFrameLimit(void) {
    if (g_targetFPS > 0 && DoFrameLimit_Impl) {
        DoFrameLimit_Impl();
    }
}

// ============================================================================
// Hook Functions
// ============================================================================

static HRESULT WINAPI Hooked_Present(IDirect3DDevice8 *This, const RECT *src, const RECT *dst, HWND hwnd, void *dirty) {
    // Call original Present function
    HRESULT hr = Real_Present(This, src, dst, hwnd, dirty);
    // Apply frame rate limiting
    DoFrameLimit();
    return hr;
}

static void InstallPresentHook(IDirect3DDevice8 *device) {
    InitFrameLimiter();

    // Get vtable and save original Present function pointer
    void **vtbl = device->lpVtbl;
    g_presentVtableEntry = &vtbl[VTABLE_INDEX_PRESENT];
    Real_Present = (PFN_Present)vtbl[VTABLE_INDEX_PRESENT];

    // Modify vtable entry to point to our hook
    DWORD oldProtect;
    VirtualProtect(g_presentVtableEntry, sizeof(void*), PAGE_READWRITE, &oldProtect);
    *g_presentVtableEntry = (void*)Hooked_Present;
    VirtualProtect(g_presentVtableEntry, sizeof(void*), oldProtect, &oldProtect);
}

static HRESULT WINAPI Hooked_CreateDevice(IDirect3D8 *This, UINT adapter, DWORD type, HWND hwnd,
                                          DWORD flags, void *params, IDirect3DDevice8 **device) {
    // Call original CreateDevice function
    HRESULT hr = Real_CreateDevice(This, adapter, type, hwnd, flags, params, device);

    // If device creation succeeded, install Present hook
    if (SUCCEEDED(hr) && device && *device) {
        InstallPresentHook(*device);
    }

    return hr;
}

static void InstallCreateDeviceHook(IDirect3D8 *d3d8) {
    // Get vtable and save original CreateDevice function pointer
    void **vtbl = d3d8->lpVtbl;
    g_createDeviceVtableEntry = &vtbl[VTABLE_INDEX_CREATEDEVICE];
    Real_CreateDevice = (PFN_CreateDevice)vtbl[VTABLE_INDEX_CREATEDEVICE];

    // Modify vtable entry to point to our hook
    DWORD oldProtect;
    VirtualProtect(g_createDeviceVtableEntry, sizeof(void*), PAGE_READWRITE, &oldProtect);
    *g_createDeviceVtableEntry = (void*)Hooked_CreateDevice;
    VirtualProtect(g_createDeviceVtableEntry, sizeof(void*), oldProtect, &oldProtect);
}

// ============================================================================
// Exported Functions
// ============================================================================

__declspec(dllexport) IDirect3D8* WINAPI Wrapper_Direct3DCreate8(UINT SDKVersion) {
    // Load real d3d8.dll from system directory on first call
    if (!hRealD3D8) {
        char path[MAX_PATH];
        UINT syslen = GetSystemDirectoryA(path, MAX_PATH);
        if (syslen == 0 || syslen + 10 >= MAX_PATH) return NULL; // 10 = strlen("\\d3d8.dll") + 1

        strcat(path, "\\d3d8.dll");
        hRealD3D8 = LoadLibraryA(path);
        if (!hRealD3D8) return NULL;

        Real_Direct3DCreate8 = (PFN_Direct3DCreate8)GetProcAddress(hRealD3D8, "Direct3DCreate8");
        if (!Real_Direct3DCreate8) return NULL;
    }

    // Create real Direct3D8 object
    IDirect3D8 *d3d8 = Real_Direct3DCreate8(SDKVersion);
    if (!d3d8) return NULL;

    // Install CreateDevice hook to intercept device creation
    InstallCreateDeviceHook(d3d8);

    return d3d8;
}

// ============================================================================
// DLL Entry Point
// ============================================================================

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
