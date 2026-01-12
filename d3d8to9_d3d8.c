/**
 * D3D8 to D3D9 Converter - IDirect3D8 Implementation
 */

// Define INITGUID before including headers to instantiate GUIDs
#include <initguid.h>
#include "d3d8to9_base.h"
#include <stdio.h>

// Forward declarations
extern IDirect3DDevice8Vtbl g_Direct3DDevice8_Vtbl;
HRESULT CreateDirect3DDevice8(Direct3D8 *pD3D8, IDirect3DDevice9 *pDevice9, Direct3DDevice8 **ppDevice8);

// ============================================================================
// Configuration
// ============================================================================

#define CONFIG_FILENAME "d3d8to9.ini"
#define CONFIG_SECTION "d3d8to9"

// Global config instance
D3D8to9Config g_Config = {
    .VSyncMode = VSYNC_USE_GAME_SETTING,
    .ForceSoftwareVP = 0,
    .DisableLightingOnFVF = 1  // Default: disable lighting on FVF fallback
};

static BOOL g_ConfigLoaded = FALSE;

static void LoadConfig(void)
{
    if (g_ConfigLoaded) return;
    g_ConfigLoaded = TRUE;

    // Get the path to the INI file (same directory as the DLL/EXE)
    char iniPath[MAX_PATH];
    GetModuleFileNameA(NULL, iniPath, MAX_PATH);

    // Find the last backslash and replace filename with config filename
    char *lastSlash = strrchr(iniPath, '\\');
    if (lastSlash) {
        strcpy(lastSlash + 1, CONFIG_FILENAME);
    } else {
        strcpy(iniPath, CONFIG_FILENAME);
    }

    // Load VSync setting
    g_Config.VSyncMode = GetPrivateProfileIntA(CONFIG_SECTION, "vsync", VSYNC_USE_GAME_SETTING, iniPath);

    // Validate range
    if (g_Config.VSyncMode < 0 || g_Config.VSyncMode > 2) {
        g_Config.VSyncMode = VSYNC_USE_GAME_SETTING;
    }

    // Load ForceSoftwareVP setting (0 = game setting, 1 = force software VP)
    g_Config.ForceSoftwareVP = GetPrivateProfileIntA(CONFIG_SECTION, "force_software_vp", 0, iniPath);

    // Load DisableLightingOnFVF setting (1 = disable lighting when using FVF fallback)
    g_Config.DisableLightingOnFVF = GetPrivateProfileIntA(CONFIG_SECTION, "disable_lighting_on_fvf", 1, iniPath);
}

// ============================================================================
// IDirect3D8 Implementation
// ============================================================================

static HRESULT WINAPI Direct3D8_QueryInterface(IDirect3D8 *This, REFIID riid, void **ppvObject)
{
    Direct3D8 *self = (Direct3D8 *)This;
    if (ppvObject == NULL) return E_POINTER;

    if (IsEqualGUID(riid, &IID_IUnknown) || IsEqualGUID(riid, &IID_IDirect3D8)) {
        self->RefCount++;
        *ppvObject = This;
        return S_OK;
    }

    *ppvObject = NULL;
    return E_NOINTERFACE;
}

static ULONG WINAPI Direct3D8_AddRef(IDirect3D8 *This)
{
    Direct3D8 *self = (Direct3D8 *)This;
    return ++self->RefCount;
}

static ULONG WINAPI Direct3D8_Release(IDirect3D8 *This)
{
    Direct3D8 *self = (Direct3D8 *)This;
    ULONG ref = --self->RefCount;
    if (ref == 0) {
        if (self->pD3D9) {
            IDirect3D9_Release(self->pD3D9);
        }
        HeapFree(GetProcessHeap(), 0, self);
    }
    return ref;
}

static HRESULT WINAPI Direct3D8_RegisterSoftwareDevice(IDirect3D8 *This, void *pInitializeFunction)
{
    Direct3D8 *self = (Direct3D8 *)This;
    return IDirect3D9_RegisterSoftwareDevice(self->pD3D9, pInitializeFunction);
}

static UINT WINAPI Direct3D8_GetAdapterCount(IDirect3D8 *This)
{
    Direct3D8 *self = (Direct3D8 *)This;
    return IDirect3D9_GetAdapterCount(self->pD3D9);
}

static HRESULT WINAPI Direct3D8_GetAdapterIdentifier(IDirect3D8 *This, UINT Adapter, DWORD Flags, D3DADAPTER_IDENTIFIER8 *pIdentifier)
{
    Direct3D8 *self = (Direct3D8 *)This;
    D3DADAPTER_IDENTIFIER9 ident9;

    // D3D8 flags: D3DENUM_NO_WHQL_LEVEL = 2
    HRESULT hr = IDirect3D9_GetAdapterIdentifier(self->pD3D9, Adapter, Flags, &ident9);
    if (SUCCEEDED(hr) && pIdentifier) {
        memcpy(pIdentifier->Driver, ident9.Driver, sizeof(pIdentifier->Driver));
        memcpy(pIdentifier->Description, ident9.Description, sizeof(pIdentifier->Description));
        pIdentifier->DriverVersion = ident9.DriverVersion;
        pIdentifier->VendorId = ident9.VendorId;
        pIdentifier->DeviceId = ident9.DeviceId;
        pIdentifier->SubSysId = ident9.SubSysId;
        pIdentifier->Revision = ident9.Revision;
        pIdentifier->DeviceIdentifier = ident9.DeviceIdentifier;
        pIdentifier->WHQLLevel = ident9.WHQLLevel;
    }
    return hr;
}

static UINT WINAPI Direct3D8_GetAdapterModeCount(IDirect3D8 *This, UINT Adapter)
{
    Direct3D8 *self = (Direct3D8 *)This;
    // D3D9 requires format, use common desktop format
    return IDirect3D9_GetAdapterModeCount(self->pD3D9, Adapter, D3DFMT_X8R8G8B8);
}

static HRESULT WINAPI Direct3D8_EnumAdapterModes(IDirect3D8 *This, UINT Adapter, UINT Mode, D3DDISPLAYMODE *pMode)
{
    Direct3D8 *self = (Direct3D8 *)This;
    return IDirect3D9_EnumAdapterModes(self->pD3D9, Adapter, D3DFMT_X8R8G8B8, Mode, pMode);
}

static HRESULT WINAPI Direct3D8_GetAdapterDisplayMode(IDirect3D8 *This, UINT Adapter, D3DDISPLAYMODE *pMode)
{
    Direct3D8 *self = (Direct3D8 *)This;
    return IDirect3D9_GetAdapterDisplayMode(self->pD3D9, Adapter, pMode);
}

static HRESULT WINAPI Direct3D8_CheckDeviceType(IDirect3D8 *This, UINT Adapter, D3DDEVTYPE CheckType, D3DFORMAT DisplayFormat, D3DFORMAT BackBufferFormat, BOOL Windowed)
{
    Direct3D8 *self = (Direct3D8 *)This;
    return IDirect3D9_CheckDeviceType(self->pD3D9, Adapter, CheckType, DisplayFormat, BackBufferFormat, Windowed);
}

static HRESULT WINAPI Direct3D8_CheckDeviceFormat(IDirect3D8 *This, UINT Adapter, D3DDEVTYPE DeviceType, D3DFORMAT AdapterFormat, DWORD Usage, D3DRESOURCETYPE RType, D3DFORMAT CheckFormat)
{
    Direct3D8 *self = (Direct3D8 *)This;
    return IDirect3D9_CheckDeviceFormat(self->pD3D9, Adapter, DeviceType, AdapterFormat, Usage, RType, CheckFormat);
}

static HRESULT WINAPI Direct3D8_CheckDeviceMultiSampleType(IDirect3D8 *This, UINT Adapter, D3DDEVTYPE DeviceType, D3DFORMAT SurfaceFormat, BOOL Windowed, D3DMULTISAMPLE_TYPE MultiSampleType)
{
    Direct3D8 *self = (Direct3D8 *)This;
    return IDirect3D9_CheckDeviceMultiSampleType(self->pD3D9, Adapter, DeviceType, SurfaceFormat, Windowed, MultiSampleType, NULL);
}

static HRESULT WINAPI Direct3D8_CheckDepthStencilMatch(IDirect3D8 *This, UINT Adapter, D3DDEVTYPE DeviceType, D3DFORMAT AdapterFormat, D3DFORMAT RenderTargetFormat, D3DFORMAT DepthStencilFormat)
{
    Direct3D8 *self = (Direct3D8 *)This;
    return IDirect3D9_CheckDepthStencilMatch(self->pD3D9, Adapter, DeviceType, AdapterFormat, RenderTargetFormat, DepthStencilFormat);
}

static HRESULT WINAPI Direct3D8_GetDeviceCaps(IDirect3D8 *This, UINT Adapter, D3DDEVTYPE DeviceType, D3DCAPS8 *pCaps)
{
    Direct3D8 *self = (Direct3D8 *)This;
    D3DCAPS9 caps9;

    HRESULT hr = IDirect3D9_GetDeviceCaps(self->pD3D9, Adapter, DeviceType, &caps9);
    if (SUCCEEDED(hr) && pCaps) {
        ConvertCaps9to8(&caps9, pCaps);
    }
    return hr;
}

static HMONITOR WINAPI Direct3D8_GetAdapterMonitor(IDirect3D8 *This, UINT Adapter)
{
    Direct3D8 *self = (Direct3D8 *)This;
    return IDirect3D9_GetAdapterMonitor(self->pD3D9, Adapter);
}

static HRESULT WINAPI Direct3D8_CreateDevice(IDirect3D8 *This, UINT Adapter, D3DDEVTYPE DeviceType, HWND hFocusWindow, DWORD BehaviorFlags, D3DPRESENT_PARAMETERS8 *pPresentationParameters, IDirect3DDevice8 **ppReturnedDeviceInterface)
{
    Direct3D8 *self = (Direct3D8 *)This;
    if (ppReturnedDeviceInterface == NULL || pPresentationParameters == NULL) {
        return D3DERR_INVALIDCALL;
    }

    D3DPRESENT_PARAMETERS params9;
    ConvertPresentParameters8to9(pPresentationParameters, &params9);

    // Apply VSync setting from config
    ApplyVSyncSetting(&params9);

    // Apply software vertex processing if configured
    DWORD modifiedBehaviorFlags = BehaviorFlags;
    if (g_Config.ForceSoftwareVP) {
        // Remove hardware VP flags and add software VP
        modifiedBehaviorFlags &= ~(D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MIXED_VERTEXPROCESSING);
        modifiedBehaviorFlags |= D3DCREATE_SOFTWARE_VERTEXPROCESSING;
    }

    IDirect3DDevice9 *pDevice9 = NULL;
    HRESULT hr = IDirect3D9_CreateDevice(self->pD3D9, Adapter, DeviceType, hFocusWindow, modifiedBehaviorFlags, &params9, &pDevice9);

    if (SUCCEEDED(hr)) {
        // Convert back any modified parameters
        ConvertPresentParameters9to8(&params9, pPresentationParameters);

        Direct3DDevice8 *pDevice8 = NULL;
        hr = CreateDirect3DDevice8(self, pDevice9, &pDevice8);

        if (SUCCEEDED(hr)) {
            *ppReturnedDeviceInterface = (IDirect3DDevice8 *)pDevice8;
        } else {
            IDirect3DDevice9_Release(pDevice9);
            *ppReturnedDeviceInterface = NULL;
        }
    }

    return hr;
}

// IDirect3D8 VTable
IDirect3D8Vtbl g_Direct3D8_Vtbl = {
    Direct3D8_QueryInterface,
    Direct3D8_AddRef,
    Direct3D8_Release,
    Direct3D8_RegisterSoftwareDevice,
    Direct3D8_GetAdapterCount,
    Direct3D8_GetAdapterIdentifier,
    Direct3D8_GetAdapterModeCount,
    Direct3D8_EnumAdapterModes,
    Direct3D8_GetAdapterDisplayMode,
    Direct3D8_CheckDeviceType,
    Direct3D8_CheckDeviceFormat,
    Direct3D8_CheckDeviceMultiSampleType,
    Direct3D8_CheckDepthStencilMatch,
    Direct3D8_GetDeviceCaps,
    Direct3D8_GetAdapterMonitor,
    Direct3D8_CreateDevice
};

// ============================================================================
// Direct3DCreate8 Export
// ============================================================================

typedef IDirect3D9 * (WINAPI *PFN_Direct3DCreate9)(UINT);
static HMODULE g_hD3D9 = NULL;
static PFN_Direct3DCreate9 g_pDirect3DCreate9 = NULL;

IDirect3D8 * WINAPI Direct3DCreate8(UINT SDKVersion)
{
    (void)SDKVersion;

    // Load configuration
    LoadConfig();

    // Load d3d9.dll if not already loaded
    if (g_hD3D9 == NULL) {
        char systemPath[MAX_PATH];
        GetSystemDirectoryA(systemPath, MAX_PATH);
        strcat(systemPath, "\\d3d9.dll");
        g_hD3D9 = LoadLibraryA(systemPath);

        if (g_hD3D9 == NULL) {
            return NULL;
        }

        g_pDirect3DCreate9 = (PFN_Direct3DCreate9)GetProcAddress(g_hD3D9, "Direct3DCreate9");
        if (g_pDirect3DCreate9 == NULL) {
            FreeLibrary(g_hD3D9);
            g_hD3D9 = NULL;
            return NULL;
        }
    }

    // Create D3D9 object
    IDirect3D9 *pD3D9 = g_pDirect3DCreate9(D3D_SDK_VERSION);
    if (pD3D9 == NULL) {
        return NULL;
    }

    // Create wrapper
    Direct3D8 *pD3D8 = (Direct3D8 *)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(Direct3D8));
    if (pD3D8 == NULL) {
        IDirect3D9_Release(pD3D9);
        return NULL;
    }

    pD3D8->lpVtbl = &g_Direct3D8_Vtbl;
    pD3D8->RefCount = 1;
    pD3D8->pD3D9 = pD3D9;

    return (IDirect3D8 *)pD3D8;
}

// DLL Entry Point
BOOL WINAPI DllMain(HINSTANCE hInstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
    (void)hInstDLL;
    (void)lpvReserved;

    switch (fdwReason) {
        case DLL_PROCESS_ATTACH:
            DisableThreadLibraryCalls(hInstDLL);
            break;
        case DLL_PROCESS_DETACH:
            if (g_hD3D9) {
                FreeLibrary(g_hD3D9);
                g_hD3D9 = NULL;
            }
            break;
    }
    return TRUE;
}
