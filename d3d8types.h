/**
 * D3D8 to D3D9 Converter
 * D3D8 Type Definitions
 */

#ifndef D3D8TYPES_H
#define D3D8TYPES_H

#include <windows.h>

// ============================================================================
// Basic Types
// ============================================================================

typedef DWORD D3DCOLOR;

typedef struct _D3DVECTOR {
    float x, y, z;
} D3DVECTOR;

typedef struct _D3DCOLORVALUE {
    float r, g, b, a;
} D3DCOLORVALUE;

typedef struct _D3DRECT {
    LONG x1, y1, x2, y2;
} D3DRECT;

typedef struct _D3DMATRIX {
    union {
        struct {
            float _11, _12, _13, _14;
            float _21, _22, _23, _24;
            float _31, _32, _33, _34;
            float _41, _42, _43, _44;
        };
        float m[4][4];
    };
} D3DMATRIX;

typedef struct _D3DVIEWPORT8 {
    DWORD X, Y;
    DWORD Width, Height;
    float MinZ, MaxZ;
} D3DVIEWPORT8;

// ============================================================================
// Enumerations
// ============================================================================

typedef enum _D3DDEVTYPE {
    D3DDEVTYPE_HAL         = 1,
    D3DDEVTYPE_REF         = 2,
    D3DDEVTYPE_SW          = 3,
    D3DDEVTYPE_FORCE_DWORD = 0x7fffffff
} D3DDEVTYPE;

typedef enum _D3DFORMAT {
    D3DFMT_UNKNOWN              = 0,
    D3DFMT_R8G8B8               = 20,
    D3DFMT_A8R8G8B8             = 21,
    D3DFMT_X8R8G8B8             = 22,
    D3DFMT_R5G6B5               = 23,
    D3DFMT_X1R5G5B5             = 24,
    D3DFMT_A1R5G5B5             = 25,
    D3DFMT_A4R4G4B4             = 26,
    D3DFMT_R3G3B2               = 27,
    D3DFMT_A8                   = 28,
    D3DFMT_A8R3G3B2             = 29,
    D3DFMT_X4R4G4B4             = 30,
    D3DFMT_A2B10G10R10          = 31,
    D3DFMT_G16R16               = 34,
    D3DFMT_A8P8                 = 40,
    D3DFMT_P8                   = 41,
    D3DFMT_L8                   = 50,
    D3DFMT_A8L8                 = 51,
    D3DFMT_A4L4                 = 52,
    D3DFMT_V8U8                 = 60,
    D3DFMT_L6V5U5               = 61,
    D3DFMT_X8L8V8U8             = 62,
    D3DFMT_Q8W8V8U8             = 63,
    D3DFMT_V16U16               = 64,
    D3DFMT_W11V11U10            = 65,
    D3DFMT_A2W10V10U10          = 67,
    D3DFMT_UYVY                 = 0x59565955, // MAKEFOURCC('U','Y','V','Y')
    D3DFMT_YUY2                 = 0x32595559, // MAKEFOURCC('Y','U','Y','2')
    D3DFMT_DXT1                 = 0x31545844, // MAKEFOURCC('D','X','T','1')
    D3DFMT_DXT2                 = 0x32545844, // MAKEFOURCC('D','X','T','2')
    D3DFMT_DXT3                 = 0x33545844, // MAKEFOURCC('D','X','T','3')
    D3DFMT_DXT4                 = 0x34545844, // MAKEFOURCC('D','X','T','4')
    D3DFMT_DXT5                 = 0x35545844, // MAKEFOURCC('D','X','T','5')
    D3DFMT_D16_LOCKABLE         = 70,
    D3DFMT_D32                  = 71,
    D3DFMT_D15S1                = 73,
    D3DFMT_D24S8                = 75,
    D3DFMT_D16                  = 80,
    D3DFMT_D24X8                = 77,
    D3DFMT_D24X4S4              = 79,
    D3DFMT_VERTEXDATA           = 100,
    D3DFMT_INDEX16              = 101,
    D3DFMT_INDEX32              = 102,
    D3DFMT_FORCE_DWORD          = 0x7fffffff
} D3DFORMAT;

typedef enum _D3DMULTISAMPLE_TYPE {
    D3DMULTISAMPLE_NONE        = 0,
    D3DMULTISAMPLE_2_SAMPLES   = 2,
    D3DMULTISAMPLE_3_SAMPLES   = 3,
    D3DMULTISAMPLE_4_SAMPLES   = 4,
    D3DMULTISAMPLE_5_SAMPLES   = 5,
    D3DMULTISAMPLE_6_SAMPLES   = 6,
    D3DMULTISAMPLE_7_SAMPLES   = 7,
    D3DMULTISAMPLE_8_SAMPLES   = 8,
    D3DMULTISAMPLE_9_SAMPLES   = 9,
    D3DMULTISAMPLE_10_SAMPLES  = 10,
    D3DMULTISAMPLE_11_SAMPLES  = 11,
    D3DMULTISAMPLE_12_SAMPLES  = 12,
    D3DMULTISAMPLE_13_SAMPLES  = 13,
    D3DMULTISAMPLE_14_SAMPLES  = 14,
    D3DMULTISAMPLE_15_SAMPLES  = 15,
    D3DMULTISAMPLE_16_SAMPLES  = 16,
    D3DMULTISAMPLE_FORCE_DWORD = 0x7fffffff
} D3DMULTISAMPLE_TYPE;

typedef enum _D3DSWAPEFFECT {
    D3DSWAPEFFECT_DISCARD     = 1,
    D3DSWAPEFFECT_FLIP        = 2,
    D3DSWAPEFFECT_COPY        = 3,
    D3DSWAPEFFECT_COPY_VSYNC  = 4,  // D3D8 only
    D3DSWAPEFFECT_FORCE_DWORD = 0x7fffffff
} D3DSWAPEFFECT;

typedef enum _D3DPOOL {
    D3DPOOL_DEFAULT     = 0,
    D3DPOOL_MANAGED     = 1,
    D3DPOOL_SYSTEMMEM   = 2,
    D3DPOOL_SCRATCH     = 3,
    D3DPOOL_FORCE_DWORD = 0x7fffffff
} D3DPOOL;

typedef enum _D3DRESOURCETYPE {
    D3DRTYPE_SURFACE       = 1,
    D3DRTYPE_VOLUME        = 2,
    D3DRTYPE_TEXTURE       = 3,
    D3DRTYPE_VOLUMETEXTURE = 4,
    D3DRTYPE_CUBETEXTURE   = 5,
    D3DRTYPE_VERTEXBUFFER  = 6,
    D3DRTYPE_INDEXBUFFER   = 7,
    D3DRTYPE_FORCE_DWORD   = 0x7fffffff
} D3DRESOURCETYPE;

typedef enum _D3DPRIMITIVETYPE {
    D3DPT_POINTLIST     = 1,
    D3DPT_LINELIST      = 2,
    D3DPT_LINESTRIP     = 3,
    D3DPT_TRIANGLELIST  = 4,
    D3DPT_TRIANGLESTRIP = 5,
    D3DPT_TRIANGLEFAN   = 6,
    D3DPT_FORCE_DWORD   = 0x7fffffff
} D3DPRIMITIVETYPE;

typedef enum _D3DTRANSFORMSTATETYPE {
    D3DTS_VIEW          = 2,
    D3DTS_PROJECTION    = 3,
    D3DTS_TEXTURE0      = 16,
    D3DTS_TEXTURE1      = 17,
    D3DTS_TEXTURE2      = 18,
    D3DTS_TEXTURE3      = 19,
    D3DTS_TEXTURE4      = 20,
    D3DTS_TEXTURE5      = 21,
    D3DTS_TEXTURE6      = 22,
    D3DTS_TEXTURE7      = 23,
    D3DTS_FORCE_DWORD   = 0x7fffffff
} D3DTRANSFORMSTATETYPE;

#define D3DTS_WORLDMATRIX(index) (D3DTRANSFORMSTATETYPE)(index + 256)
#define D3DTS_WORLD  D3DTS_WORLDMATRIX(0)
#define D3DTS_WORLD1 D3DTS_WORLDMATRIX(1)
#define D3DTS_WORLD2 D3DTS_WORLDMATRIX(2)
#define D3DTS_WORLD3 D3DTS_WORLDMATRIX(3)

typedef enum _D3DRENDERSTATETYPE {
    D3DRS_ZENABLE                   = 7,
    D3DRS_FILLMODE                  = 8,
    D3DRS_SHADEMODE                 = 9,
    D3DRS_LINEPATTERN               = 10,  // D3D8 only
    D3DRS_ZWRITEENABLE              = 14,
    D3DRS_ALPHATESTENABLE           = 15,
    D3DRS_LASTPIXEL                 = 16,
    D3DRS_SRCBLEND                  = 19,
    D3DRS_DESTBLEND                 = 20,
    D3DRS_CULLMODE                  = 22,
    D3DRS_ZFUNC                     = 23,
    D3DRS_ALPHAREF                  = 24,
    D3DRS_ALPHAFUNC                 = 25,
    D3DRS_DITHERENABLE              = 26,
    D3DRS_ALPHABLENDENABLE          = 27,
    D3DRS_FOGENABLE                 = 28,
    D3DRS_SPECULARENABLE            = 29,
    D3DRS_ZVISIBLE                  = 30,  // D3D8 only
    D3DRS_FOGCOLOR                  = 34,
    D3DRS_FOGTABLEMODE              = 35,
    D3DRS_FOGSTART                  = 36,
    D3DRS_FOGEND                    = 37,
    D3DRS_FOGDENSITY                = 38,
    D3DRS_EDGEANTIALIAS             = 40,  // D3D8 only
    D3DRS_ZBIAS                     = 47,  // D3D8 only - maps to D3DRS_DEPTHBIAS in D3D9
    D3DRS_RANGEFOGENABLE            = 48,
    D3DRS_STENCILENABLE             = 52,
    D3DRS_STENCILFAIL               = 53,
    D3DRS_STENCILZFAIL              = 54,
    D3DRS_STENCILPASS               = 55,
    D3DRS_STENCILFUNC               = 56,
    D3DRS_STENCILREF                = 57,
    D3DRS_STENCILMASK               = 58,
    D3DRS_STENCILWRITEMASK          = 59,
    D3DRS_TEXTUREFACTOR             = 60,
    D3DRS_WRAP0                     = 128,
    D3DRS_WRAP1                     = 129,
    D3DRS_WRAP2                     = 130,
    D3DRS_WRAP3                     = 131,
    D3DRS_WRAP4                     = 132,
    D3DRS_WRAP5                     = 133,
    D3DRS_WRAP6                     = 134,
    D3DRS_WRAP7                     = 135,
    D3DRS_CLIPPING                  = 136,
    D3DRS_LIGHTING                  = 137,
    D3DRS_AMBIENT                   = 139,
    D3DRS_FOGVERTEXMODE             = 140,
    D3DRS_COLORVERTEX               = 141,
    D3DRS_LOCALVIEWER               = 142,
    D3DRS_NORMALIZENORMALS          = 143,
    D3DRS_DIFFUSEMATERIALSOURCE     = 145,
    D3DRS_SPECULARMATERIALSOURCE    = 146,
    D3DRS_AMBIENTMATERIALSOURCE     = 147,
    D3DRS_EMISSIVEMATERIALSOURCE    = 148,
    D3DRS_VERTEXBLEND               = 151,
    D3DRS_CLIPPLANEENABLE           = 152,
    D3DRS_SOFTWAREVERTEXPROCESSING  = 153,  // D3D8 only
    D3DRS_POINTSIZE                 = 154,
    D3DRS_POINTSIZE_MIN             = 155,
    D3DRS_POINTSPRITEENABLE         = 156,
    D3DRS_POINTSCALEENABLE          = 157,
    D3DRS_POINTSCALE_A              = 158,
    D3DRS_POINTSCALE_B              = 159,
    D3DRS_POINTSCALE_C              = 160,
    D3DRS_MULTISAMPLEANTIALIAS      = 161,
    D3DRS_MULTISAMPLEMASK           = 162,
    D3DRS_PATCHEDGESTYLE            = 163,
    D3DRS_PATCHSEGMENTS             = 164,  // D3D8 only
    D3DRS_DEBUGMONITORTOKEN         = 165,
    D3DRS_POINTSIZE_MAX             = 166,
    D3DRS_INDEXEDVERTEXBLENDENABLE  = 167,
    D3DRS_COLORWRITEENABLE          = 168,
    D3DRS_TWEENFACTOR               = 170,
    D3DRS_BLENDOP                   = 171,
    D3DRS_POSITIONORDER             = 172,  // D3D8 only
    D3DRS_NORMALORDER               = 173,  // D3D8 only
    D3DRS_FORCE_DWORD               = 0x7fffffff
} D3DRENDERSTATETYPE;

typedef enum _D3DTEXTURESTAGESTATETYPE {
    D3DTSS_COLOROP               = 1,
    D3DTSS_COLORARG1             = 2,
    D3DTSS_COLORARG2             = 3,
    D3DTSS_ALPHAOP               = 4,
    D3DTSS_ALPHAARG1             = 5,
    D3DTSS_ALPHAARG2             = 6,
    D3DTSS_BUMPENVMAT00          = 7,
    D3DTSS_BUMPENVMAT01          = 8,
    D3DTSS_BUMPENVMAT10          = 9,
    D3DTSS_BUMPENVMAT11          = 10,
    D3DTSS_TEXCOORDINDEX         = 11,
    D3DTSS_ADDRESSU              = 13,  // D3D8 only - maps to sampler state in D3D9
    D3DTSS_ADDRESSV              = 14,  // D3D8 only
    D3DTSS_BORDERCOLOR           = 15,  // D3D8 only
    D3DTSS_MAGFILTER             = 16,  // D3D8 only
    D3DTSS_MINFILTER             = 17,  // D3D8 only
    D3DTSS_MIPFILTER             = 18,  // D3D8 only
    D3DTSS_MIPMAPLODBIAS         = 19,  // D3D8 only
    D3DTSS_MAXMIPLEVEL           = 20,  // D3D8 only
    D3DTSS_MAXANISOTROPY         = 21,  // D3D8 only
    D3DTSS_BUMPENVLSCALE         = 22,
    D3DTSS_BUMPENVLOFFSET        = 23,
    D3DTSS_TEXTURETRANSFORMFLAGS = 24,
    D3DTSS_ADDRESSW              = 25,  // D3D8 only
    D3DTSS_COLORARG0             = 26,
    D3DTSS_ALPHAARG0             = 27,
    D3DTSS_RESULTARG             = 28,
    D3DTSS_FORCE_DWORD           = 0x7fffffff
} D3DTEXTURESTAGESTATETYPE;

typedef enum _D3DBACKBUFFER_TYPE {
    D3DBACKBUFFER_TYPE_MONO        = 0,
    D3DBACKBUFFER_TYPE_LEFT        = 1,
    D3DBACKBUFFER_TYPE_RIGHT       = 2,
    D3DBACKBUFFER_TYPE_FORCE_DWORD = 0x7fffffff
} D3DBACKBUFFER_TYPE;

typedef enum _D3DCUBEMAP_FACES {
    D3DCUBEMAP_FACE_POSITIVE_X = 0,
    D3DCUBEMAP_FACE_NEGATIVE_X = 1,
    D3DCUBEMAP_FACE_POSITIVE_Y = 2,
    D3DCUBEMAP_FACE_NEGATIVE_Y = 3,
    D3DCUBEMAP_FACE_POSITIVE_Z = 4,
    D3DCUBEMAP_FACE_NEGATIVE_Z = 5,
    D3DCUBEMAP_FACE_FORCE_DWORD = 0x7fffffff
} D3DCUBEMAP_FACES;

typedef enum _D3DSTATEBLOCKTYPE {
    D3DSBT_ALL         = 1,
    D3DSBT_PIXELSTATE  = 2,
    D3DSBT_VERTEXSTATE = 3,
    D3DSBT_FORCE_DWORD = 0x7fffffff
} D3DSTATEBLOCKTYPE;

typedef enum _D3DLIGHTTYPE {
    D3DLIGHT_POINT       = 1,
    D3DLIGHT_SPOT        = 2,
    D3DLIGHT_DIRECTIONAL = 3,
    D3DLIGHT_FORCE_DWORD = 0x7fffffff
} D3DLIGHTTYPE;

// ============================================================================
// Structures
// ============================================================================

typedef struct _D3DDISPLAYMODE {
    UINT Width;
    UINT Height;
    UINT RefreshRate;
    D3DFORMAT Format;
} D3DDISPLAYMODE;

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
    UINT FullScreen_PresentationInterval;
} D3DPRESENT_PARAMETERS8;

typedef struct _D3DSURFACE_DESC8 {
    D3DFORMAT Format;
    D3DRESOURCETYPE Type;
    DWORD Usage;
    D3DPOOL Pool;
    UINT Size;
    D3DMULTISAMPLE_TYPE MultiSampleType;
    UINT Width;
    UINT Height;
} D3DSURFACE_DESC8;

typedef struct _D3DVOLUME_DESC8 {
    D3DFORMAT Format;
    D3DRESOURCETYPE Type;
    DWORD Usage;
    D3DPOOL Pool;
    UINT Size;
    UINT Width;
    UINT Height;
    UINT Depth;
} D3DVOLUME_DESC8;

typedef struct _D3DLOCKED_RECT {
    INT Pitch;
    void *pBits;
} D3DLOCKED_RECT;

typedef struct _D3DLOCKED_BOX {
    INT RowPitch;
    INT SlicePitch;
    void *pBits;
} D3DLOCKED_BOX;

typedef struct _D3DBOX {
    UINT Left;
    UINT Top;
    UINT Right;
    UINT Bottom;
    UINT Front;
    UINT Back;
} D3DBOX;

typedef struct _D3DCLIPSTATUS8 {
    DWORD ClipUnion;
    DWORD ClipIntersection;
} D3DCLIPSTATUS8;

typedef struct _D3DMATERIAL8 {
    D3DCOLORVALUE Diffuse;
    D3DCOLORVALUE Ambient;
    D3DCOLORVALUE Specular;
    D3DCOLORVALUE Emissive;
    float Power;
} D3DMATERIAL8;

typedef struct _D3DLIGHT8 {
    D3DLIGHTTYPE Type;
    D3DCOLORVALUE Diffuse;
    D3DCOLORVALUE Specular;
    D3DCOLORVALUE Ambient;
    D3DVECTOR Position;
    D3DVECTOR Direction;
    float Range;
    float Falloff;
    float Attenuation0;
    float Attenuation1;
    float Attenuation2;
    float Theta;
    float Phi;
} D3DLIGHT8;

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
    float MaxPixelShaderValue;
} D3DCAPS8;

typedef struct _D3DRASTER_STATUS {
    BOOL InVBlank;
    UINT ScanLine;
} D3DRASTER_STATUS;

typedef struct _D3DGAMMARAMP {
    WORD red[256];
    WORD green[256];
    WORD blue[256];
} D3DGAMMARAMP;

typedef struct _D3DVERTEXBUFFER_DESC {
    D3DFORMAT Format;
    D3DRESOURCETYPE Type;
    DWORD Usage;
    D3DPOOL Pool;
    UINT Size;
    DWORD FVF;
} D3DVERTEXBUFFER_DESC;

typedef struct _D3DINDEXBUFFER_DESC {
    D3DFORMAT Format;
    D3DRESOURCETYPE Type;
    DWORD Usage;
    D3DPOOL Pool;
    UINT Size;
} D3DINDEXBUFFER_DESC;

// ============================================================================
// D3D8 Usage Flags
// ============================================================================

#define D3DUSAGE_RENDERTARGET       0x00000001
#define D3DUSAGE_DEPTHSTENCIL       0x00000002
#define D3DUSAGE_WRITEONLY          0x00000008
#define D3DUSAGE_SOFTWAREPROCESSING 0x00000010
#define D3DUSAGE_DONOTCLIP          0x00000020
#define D3DUSAGE_POINTS             0x00000040
#define D3DUSAGE_RTPATCHES          0x00000080
#define D3DUSAGE_NPATCHES           0x00000100
#define D3DUSAGE_DYNAMIC            0x00000200

// ============================================================================
// D3D8 Lock Flags
// ============================================================================

#define D3DLOCK_READONLY        0x00000010
#define D3DLOCK_DISCARD         0x00002000
#define D3DLOCK_NOOVERWRITE     0x00001000
#define D3DLOCK_NOSYSLOCK       0x00000800
#define D3DLOCK_NO_DIRTY_UPDATE 0x00008000

// ============================================================================
// D3D8 Clear Flags
// ============================================================================

#define D3DCLEAR_TARGET  0x00000001
#define D3DCLEAR_ZBUFFER 0x00000002
#define D3DCLEAR_STENCIL 0x00000004

// ============================================================================
// D3D8 Create Flags
// ============================================================================

#define D3DCREATE_FPU_PRESERVE              0x00000002
#define D3DCREATE_MULTITHREADED             0x00000004
#define D3DCREATE_PUREDEVICE                0x00000010
#define D3DCREATE_SOFTWARE_VERTEXPROCESSING 0x00000020
#define D3DCREATE_HARDWARE_VERTEXPROCESSING 0x00000040
#define D3DCREATE_MIXED_VERTEXPROCESSING    0x00000080

// ============================================================================
// D3D8 Presentation Intervals
// ============================================================================

#define D3DPRESENT_INTERVAL_DEFAULT   0x00000000
#define D3DPRESENT_INTERVAL_ONE       0x00000001
#define D3DPRESENT_INTERVAL_TWO       0x00000002
#define D3DPRESENT_INTERVAL_THREE     0x00000004
#define D3DPRESENT_INTERVAL_FOUR      0x00000008
#define D3DPRESENT_INTERVAL_IMMEDIATE 0x80000000

// ============================================================================
// D3D8 Present Flags
// ============================================================================

#define D3DPRESENTFLAG_LOCKABLE_BACKBUFFER 0x00000001

// ============================================================================
// Flexible Vertex Format (FVF) Codes
// ============================================================================

#define D3DFVF_RESERVED0        0x0001
#define D3DFVF_POSITION_MASK    0x000E
#define D3DFVF_XYZ              0x0002
#define D3DFVF_XYZRHW           0x0004
#define D3DFVF_XYZB1            0x0006
#define D3DFVF_XYZB2            0x0008
#define D3DFVF_XYZB3            0x000a
#define D3DFVF_XYZB4            0x000c
#define D3DFVF_XYZB5            0x000e
#define D3DFVF_NORMAL           0x0010
#define D3DFVF_PSIZE            0x0020
#define D3DFVF_DIFFUSE          0x0040
#define D3DFVF_SPECULAR         0x0080
#define D3DFVF_TEXCOUNT_MASK    0x0f00
#define D3DFVF_TEXCOUNT_SHIFT   8
#define D3DFVF_TEX0             0x0000
#define D3DFVF_TEX1             0x0100
#define D3DFVF_TEX2             0x0200
#define D3DFVF_TEX3             0x0300
#define D3DFVF_TEX4             0x0400
#define D3DFVF_TEX5             0x0500
#define D3DFVF_TEX6             0x0600
#define D3DFVF_TEX7             0x0700
#define D3DFVF_TEX8             0x0800
#define D3DFVF_LASTBETA_UBYTE4  0x1000
#define D3DFVF_RESERVED2        0xE000

// ============================================================================
// D3D8 SDK Version
// ============================================================================

#define D3D_SDK_VERSION 220

#endif // D3D8TYPES_H
