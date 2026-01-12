/**
 * D3D8 to D3D9 Converter
 * D3D8-specific Type Definitions
 *
 * This file only defines types that are specific to D3D8 or differ from D3D9.
 * Common types are provided by the D3D9 headers.
 */

#ifndef D3D8TYPES_H
#define D3D8TYPES_H

// Note: d3d9.h must be included before this file

// ============================================================================
// D3D8-only Constants
// ============================================================================

// D3D8-only swap effect (removed in D3D9)
#define D3DSWAPEFFECT_COPY_VSYNC 4

// D3D8-only render states (not in D3D9)
#define D3DRS8_LINEPATTERN              10
#define D3DRS8_ZVISIBLE                 30
#define D3DRS8_EDGEANTIALIAS            40
#define D3DRS8_ZBIAS                    47
#define D3DRS8_SOFTWAREVERTEXPROCESSING 153
#define D3DRS8_PATCHSEGMENTS            164
#define D3DRS8_POSITIONORDER            172
#define D3DRS8_NORMALORDER              173

// D3D8 texture stage states that map to D3D9 sampler states
#define D3DTSS8_ADDRESSU       13
#define D3DTSS8_ADDRESSV       14
#define D3DTSS8_BORDERCOLOR    15
#define D3DTSS8_MAGFILTER      16
#define D3DTSS8_MINFILTER      17
#define D3DTSS8_MIPFILTER      18
#define D3DTSS8_MIPMAPLODBIAS  19
#define D3DTSS8_MAXMIPLEVEL    20
#define D3DTSS8_MAXANISOTROPY  21
#define D3DTSS8_ADDRESSW       25

// D3D8 SDK Version
#define D3D8_SDK_VERSION 220

// ============================================================================
// D3D8-specific Structures (different from D3D9)
// ============================================================================

// D3D8 Present Parameters (different layout from D3D9)
typedef struct _D3DPRESENT_PARAMETERS8 {
    UINT BackBufferWidth;
    UINT BackBufferHeight;
    D3DFORMAT BackBufferFormat;
    UINT BackBufferCount;
    D3DMULTISAMPLE_TYPE MultiSampleType;
    D3DSWAPEFFECT SwapEffect;
    HWND hDeviceWindow;
    BOOL Windowed;
    BOOL EnableAutoDepthStencil;
    D3DFORMAT AutoDepthStencilFormat;
    DWORD Flags;
    UINT FullScreen_RefreshRateInHz;
    UINT FullScreen_PresentationInterval;  // Named differently in D3D9
} D3DPRESENT_PARAMETERS8;

// D3D8 Surface Description (has Size field, D3D9 doesn't)
typedef struct _D3DSURFACE_DESC8 {
    D3DFORMAT Format;
    D3DRESOURCETYPE Type;
    DWORD Usage;
    D3DPOOL Pool;
    UINT Size;  // D3D8 only
    D3DMULTISAMPLE_TYPE MultiSampleType;
    UINT Width;
    UINT Height;
} D3DSURFACE_DESC8;

// D3D8 Volume Description (has Size field)
typedef struct _D3DVOLUME_DESC8 {
    D3DFORMAT Format;
    D3DRESOURCETYPE Type;
    DWORD Usage;
    D3DPOOL Pool;
    UINT Size;  // D3D8 only
    UINT Width;
    UINT Height;
    UINT Depth;
} D3DVOLUME_DESC8;

// D3D8 Adapter Identifier (slightly different from D3D9)
typedef struct _D3DADAPTER_IDENTIFIER8 {
    char Driver[512];
    char Description[512];
    LARGE_INTEGER DriverVersion;
    DWORD VendorId;
    DWORD DeviceId;
    DWORD SubSysId;
    DWORD Revision;
    GUID DeviceIdentifier;
    DWORD WHQLLevel;
} D3DADAPTER_IDENTIFIER8;

// D3D8 Device Caps (different from D3D9, missing many D3D9 fields)
typedef struct _D3DCAPS8 {
    D3DDEVTYPE DeviceType;
    UINT AdapterOrdinal;
    DWORD Caps;
    DWORD Caps2;
    DWORD Caps3;
    DWORD PresentationIntervals;
    DWORD CursorCaps;
    DWORD DevCaps;
    DWORD PrimitiveMiscCaps;
    DWORD RasterCaps;
    DWORD ZCmpCaps;
    DWORD SrcBlendCaps;
    DWORD DestBlendCaps;
    DWORD AlphaCmpCaps;
    DWORD ShadeCaps;
    DWORD TextureCaps;
    DWORD TextureFilterCaps;
    DWORD CubeTextureFilterCaps;
    DWORD VolumeTextureFilterCaps;
    DWORD TextureAddressCaps;
    DWORD VolumeTextureAddressCaps;
    DWORD LineCaps;
    DWORD MaxTextureWidth;
    DWORD MaxTextureHeight;
    DWORD MaxVolumeExtent;
    DWORD MaxTextureRepeat;
    DWORD MaxTextureAspectRatio;
    DWORD MaxAnisotropy;
    float MaxVertexW;
    float GuardBandLeft;
    float GuardBandTop;
    float GuardBandRight;
    float GuardBandBottom;
    float ExtentsAdjust;
    DWORD StencilCaps;
    DWORD FVFCaps;
    DWORD TextureOpCaps;
    DWORD MaxTextureBlendStages;
    DWORD MaxSimultaneousTextures;
    DWORD VertexProcessingCaps;
    DWORD MaxActiveLights;
    DWORD MaxUserClipPlanes;
    DWORD MaxVertexBlendMatrices;
    DWORD MaxVertexBlendMatrixIndex;
    float MaxPointSize;
    DWORD MaxPrimitiveCount;
    DWORD MaxVertexIndex;
    DWORD MaxStreams;
    DWORD MaxStreamStride;
    DWORD VertexShaderVersion;
    DWORD MaxVertexShaderConst;
    DWORD PixelShaderVersion;
    float MaxPixelShaderValue;  // D3D8 only name (PixelShader1xMaxValue in D3D9)
} D3DCAPS8;

// D3D8 Clip Status
typedef struct _D3DCLIPSTATUS8 {
    DWORD ClipUnion;
    DWORD ClipIntersection;
} D3DCLIPSTATUS8;

// ============================================================================
// Type Aliases (same structure as D3D9)
// ============================================================================

typedef D3DMATERIAL9 D3DMATERIAL8;
typedef D3DLIGHT9 D3DLIGHT8;
typedef D3DVIEWPORT9 D3DVIEWPORT8;

#endif // D3D8TYPES_H
