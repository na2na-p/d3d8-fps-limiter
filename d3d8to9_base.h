/**
 * D3D8 to D3D9 Converter - Base Definitions
 */

#ifndef D3D8TO9_BASE_H
#define D3D8TO9_BASE_H

#define WIN32_LEAN_AND_MEAN
#define CINTERFACE
#define COBJMACROS
#include <windows.h>
#include <d3d9.h>
#include "d3d8types.h"  // D3D8-specific types (must come after d3d9.h)
#include "d3d8.h"       // D3D8 interfaces

// ============================================================================
// Wrapper Structure Forward Declarations
// ============================================================================

typedef struct Direct3D8 Direct3D8;
typedef struct Direct3DDevice8 Direct3DDevice8;
typedef struct Direct3DSurface8 Direct3DSurface8;
typedef struct Direct3DTexture8 Direct3DTexture8;
typedef struct Direct3DCubeTexture8 Direct3DCubeTexture8;
typedef struct Direct3DVolumeTexture8 Direct3DVolumeTexture8;
typedef struct Direct3DVolume8 Direct3DVolume8;
typedef struct Direct3DVertexBuffer8 Direct3DVertexBuffer8;
typedef struct Direct3DIndexBuffer8 Direct3DIndexBuffer8;
typedef struct Direct3DSwapChain8 Direct3DSwapChain8;

// ============================================================================
// Wrapper Structures
// ============================================================================

struct Direct3D8 {
    IDirect3D8Vtbl *lpVtbl;
    ULONG RefCount;
    IDirect3D9 *pD3D9;
};

struct Direct3DDevice8 {
    IDirect3DDevice8Vtbl *lpVtbl;
    ULONG RefCount;
    IDirect3DDevice9 *pDevice9;
    Direct3D8 *pD3D8;
    INT BaseVertexIndex;
    DWORD CurrentVertexShaderHandle;
    DWORD CurrentPixelShaderHandle;
    // Shader handle management
    void **VertexShaderHandles;
    DWORD VertexShaderCount;
    DWORD VertexShaderCapacity;
    void **PixelShaderHandles;
    DWORD PixelShaderCount;
    DWORD PixelShaderCapacity;
    // Z-bias conversion
    float ZBiasScale;
};

struct Direct3DSurface8 {
    IDirect3DSurface8Vtbl *lpVtbl;
    ULONG RefCount;
    IDirect3DSurface9 *pSurface9;
    Direct3DDevice8 *pDevice8;
};

struct Direct3DTexture8 {
    IDirect3DTexture8Vtbl *lpVtbl;
    ULONG RefCount;
    IDirect3DTexture9 *pTexture9;
    Direct3DDevice8 *pDevice8;
};

struct Direct3DVertexBuffer8 {
    IDirect3DVertexBuffer8Vtbl *lpVtbl;
    ULONG RefCount;
    IDirect3DVertexBuffer9 *pVB9;
    Direct3DDevice8 *pDevice8;
};

struct Direct3DIndexBuffer8 {
    IDirect3DIndexBuffer8Vtbl *lpVtbl;
    ULONG RefCount;
    IDirect3DIndexBuffer9 *pIB9;
    Direct3DDevice8 *pDevice8;
};

struct Direct3DSwapChain8 {
    IDirect3DSwapChain8Vtbl *lpVtbl;
    ULONG RefCount;
    IDirect3DSwapChain9 *pSwapChain9;
    Direct3DDevice8 *pDevice8;
};

struct Direct3DCubeTexture8 {
    IDirect3DCubeTexture8Vtbl *lpVtbl;
    ULONG RefCount;
    IDirect3DCubeTexture9 *pCubeTexture9;
    Direct3DDevice8 *pDevice8;
};

struct Direct3DVolumeTexture8 {
    IDirect3DVolumeTexture8Vtbl *lpVtbl;
    ULONG RefCount;
    IDirect3DVolumeTexture9 *pVolumeTexture9;
    Direct3DDevice8 *pDevice8;
};

struct Direct3DVolume8 {
    IDirect3DVolume8Vtbl *lpVtbl;
    ULONG RefCount;
    IDirect3DVolume9 *pVolume9;
    Direct3DDevice8 *pDevice8;
};

// ============================================================================
// Conversion Helpers
// ============================================================================

// Convert D3D8 present parameters to D3D9
static inline void ConvertPresentParameters8to9(
    const D3DPRESENT_PARAMETERS8 *pParams8,
    D3DPRESENT_PARAMETERS *pParams9)
{
    pParams9->BackBufferWidth = pParams8->BackBufferWidth;
    pParams9->BackBufferHeight = pParams8->BackBufferHeight;
    pParams9->BackBufferFormat = pParams8->BackBufferFormat;
    pParams9->BackBufferCount = pParams8->BackBufferCount;
    pParams9->MultiSampleType = pParams8->MultiSampleType;
    pParams9->MultiSampleQuality = 0;
    pParams9->SwapEffect = pParams8->SwapEffect;
    // Handle D3D8-only D3DSWAPEFFECT_COPY_VSYNC
    if (pParams9->SwapEffect == D3DSWAPEFFECT_COPY_VSYNC) {
        pParams9->SwapEffect = D3DSWAPEFFECT_COPY;
    }
    pParams9->hDeviceWindow = pParams8->hDeviceWindow;
    pParams9->Windowed = pParams8->Windowed;
    pParams9->EnableAutoDepthStencil = pParams8->EnableAutoDepthStencil;
    pParams9->AutoDepthStencilFormat = pParams8->AutoDepthStencilFormat;
    pParams9->Flags = pParams8->Flags;
    pParams9->FullScreen_RefreshRateInHz = pParams8->FullScreen_RefreshRateInHz;
    pParams9->PresentationInterval = pParams8->FullScreen_PresentationInterval;
}

// Convert D3D9 present parameters to D3D8
static inline void ConvertPresentParameters9to8(
    const D3DPRESENT_PARAMETERS *pParams9,
    D3DPRESENT_PARAMETERS8 *pParams8)
{
    pParams8->BackBufferWidth = pParams9->BackBufferWidth;
    pParams8->BackBufferHeight = pParams9->BackBufferHeight;
    pParams8->BackBufferFormat = pParams9->BackBufferFormat;
    pParams8->BackBufferCount = pParams9->BackBufferCount;
    pParams8->MultiSampleType = pParams9->MultiSampleType;
    pParams8->SwapEffect = pParams9->SwapEffect;
    pParams8->hDeviceWindow = pParams9->hDeviceWindow;
    pParams8->Windowed = pParams9->Windowed;
    pParams8->EnableAutoDepthStencil = pParams9->EnableAutoDepthStencil;
    pParams8->AutoDepthStencilFormat = pParams9->AutoDepthStencilFormat;
    pParams8->Flags = pParams9->Flags;
    pParams8->FullScreen_RefreshRateInHz = pParams9->FullScreen_RefreshRateInHz;
    pParams8->FullScreen_PresentationInterval = pParams9->PresentationInterval;
}

// Convert D3D9 caps to D3D8 caps
static inline void ConvertCaps9to8(const D3DCAPS9 *pCaps9, D3DCAPS8 *pCaps8)
{
    pCaps8->DeviceType = pCaps9->DeviceType;
    pCaps8->AdapterOrdinal = pCaps9->AdapterOrdinal;
    pCaps8->Caps = pCaps9->Caps;
    pCaps8->Caps2 = pCaps9->Caps2;
    pCaps8->Caps3 = pCaps9->Caps3;
    pCaps8->PresentationIntervals = pCaps9->PresentationIntervals;
    pCaps8->CursorCaps = pCaps9->CursorCaps;
    pCaps8->DevCaps = pCaps9->DevCaps;
    pCaps8->PrimitiveMiscCaps = pCaps9->PrimitiveMiscCaps;
    pCaps8->RasterCaps = pCaps9->RasterCaps;
    pCaps8->ZCmpCaps = pCaps9->ZCmpCaps;
    pCaps8->SrcBlendCaps = pCaps9->SrcBlendCaps;
    pCaps8->DestBlendCaps = pCaps9->DestBlendCaps;
    pCaps8->AlphaCmpCaps = pCaps9->AlphaCmpCaps;
    pCaps8->ShadeCaps = pCaps9->ShadeCaps;
    pCaps8->TextureCaps = pCaps9->TextureCaps;
    pCaps8->TextureFilterCaps = pCaps9->TextureFilterCaps;
    pCaps8->CubeTextureFilterCaps = pCaps9->CubeTextureFilterCaps;
    pCaps8->VolumeTextureFilterCaps = pCaps9->VolumeTextureFilterCaps;
    pCaps8->TextureAddressCaps = pCaps9->TextureAddressCaps;
    pCaps8->VolumeTextureAddressCaps = pCaps9->VolumeTextureAddressCaps;
    pCaps8->LineCaps = pCaps9->LineCaps;
    pCaps8->MaxTextureWidth = pCaps9->MaxTextureWidth;
    pCaps8->MaxTextureHeight = pCaps9->MaxTextureHeight;
    pCaps8->MaxVolumeExtent = pCaps9->MaxVolumeExtent;
    pCaps8->MaxTextureRepeat = pCaps9->MaxTextureRepeat;
    pCaps8->MaxTextureAspectRatio = pCaps9->MaxTextureAspectRatio;
    pCaps8->MaxAnisotropy = pCaps9->MaxAnisotropy;
    pCaps8->MaxVertexW = pCaps9->MaxVertexW;
    pCaps8->GuardBandLeft = pCaps9->GuardBandLeft;
    pCaps8->GuardBandTop = pCaps9->GuardBandTop;
    pCaps8->GuardBandRight = pCaps9->GuardBandRight;
    pCaps8->GuardBandBottom = pCaps9->GuardBandBottom;
    pCaps8->ExtentsAdjust = pCaps9->ExtentsAdjust;
    pCaps8->StencilCaps = pCaps9->StencilCaps;
    pCaps8->FVFCaps = pCaps9->FVFCaps;
    pCaps8->TextureOpCaps = pCaps9->TextureOpCaps;
    pCaps8->MaxTextureBlendStages = pCaps9->MaxTextureBlendStages;
    pCaps8->MaxSimultaneousTextures = pCaps9->MaxSimultaneousTextures;
    pCaps8->VertexProcessingCaps = pCaps9->VertexProcessingCaps;
    pCaps8->MaxActiveLights = pCaps9->MaxActiveLights;
    pCaps8->MaxUserClipPlanes = pCaps9->MaxUserClipPlanes;
    pCaps8->MaxVertexBlendMatrices = pCaps9->MaxVertexBlendMatrices;
    pCaps8->MaxVertexBlendMatrixIndex = pCaps9->MaxVertexBlendMatrixIndex;
    pCaps8->MaxPointSize = pCaps9->MaxPointSize;
    pCaps8->MaxPrimitiveCount = pCaps9->MaxPrimitiveCount;
    pCaps8->MaxVertexIndex = pCaps9->MaxVertexIndex;
    pCaps8->MaxStreams = pCaps9->MaxStreams;
    pCaps8->MaxStreamStride = pCaps9->MaxStreamStride;
    // Clamp shader versions to D3D8 maximum
    pCaps8->VertexShaderVersion = (pCaps9->VertexShaderVersion > 0xFFFE0101) ? 0xFFFE0101 : pCaps9->VertexShaderVersion;
    pCaps8->MaxVertexShaderConst = (pCaps9->MaxVertexShaderConst > 96) ? 96 : pCaps9->MaxVertexShaderConst;
    pCaps8->PixelShaderVersion = (pCaps9->PixelShaderVersion > 0xFFFF0104) ? 0xFFFF0104 : pCaps9->PixelShaderVersion;
    pCaps8->MaxPixelShaderValue = pCaps9->PixelShader1xMaxValue;
}

// Convert D3D9 surface desc to D3D8
static inline void ConvertSurfaceDesc9to8(const D3DSURFACE_DESC *pDesc9, D3DSURFACE_DESC8 *pDesc8)
{
    pDesc8->Format = pDesc9->Format;
    pDesc8->Type = pDesc9->Type;
    pDesc8->Usage = pDesc9->Usage;
    pDesc8->Pool = pDesc9->Pool;
    pDesc8->Size = 0;  // D3D8 computed this, D3D9 doesn't provide it
    pDesc8->MultiSampleType = pDesc9->MultiSampleType;
    pDesc8->Width = pDesc9->Width;
    pDesc8->Height = pDesc9->Height;
}

// D3D8 sampler state types mapped to D3D9 sampler states
#define D3DSAMP_ADDRESSU       1
#define D3DSAMP_ADDRESSV       2
#define D3DSAMP_ADDRESSW       3
#define D3DSAMP_BORDERCOLOR    4
#define D3DSAMP_MAGFILTER      5
#define D3DSAMP_MINFILTER      6
#define D3DSAMP_MIPFILTER      7
#define D3DSAMP_MIPMAPLODBIAS  8
#define D3DSAMP_MAXMIPLEVEL    9
#define D3DSAMP_MAXANISOTROPY  10

// D3D9 depth bias render state
#define D3DRS_DEPTHBIAS 195

// ============================================================================
// Configuration
// ============================================================================

// VSync modes
#define VSYNC_USE_GAME_SETTING 0
#define VSYNC_FORCE_ON         1
#define VSYNC_FORCE_OFF        2

typedef struct D3D8to9Config {
    int VSyncMode;  // 0 = game setting, 1 = force on, 2 = force off
} D3D8to9Config;

// Global config (defined in d3d8to9_d3d8.c)
extern D3D8to9Config g_Config;

// Apply VSync setting to presentation interval
static inline void ApplyVSyncSetting(D3DPRESENT_PARAMETERS *pParams9)
{
    switch (g_Config.VSyncMode) {
        case VSYNC_FORCE_ON:
            pParams9->PresentationInterval = D3DPRESENT_INTERVAL_ONE;
            break;
        case VSYNC_FORCE_OFF:
            pParams9->PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
            break;
        case VSYNC_USE_GAME_SETTING:
        default:
            // Keep the game's original setting
            break;
    }
}

#endif // D3D8TO9_BASE_H
