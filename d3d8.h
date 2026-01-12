/**
 * D3D8 to D3D9 Converter
 * D3D8 Interface Definitions
 *
 * Note: d3d9.h and d3d8types.h must be included before this file
 */

#ifndef D3D8_H
#define D3D8_H

// ============================================================================
// Forward Declarations
// ============================================================================

typedef struct IDirect3D8 IDirect3D8;
typedef struct IDirect3DDevice8 IDirect3DDevice8;
typedef struct IDirect3DResource8 IDirect3DResource8;
typedef struct IDirect3DBaseTexture8 IDirect3DBaseTexture8;
typedef struct IDirect3DTexture8 IDirect3DTexture8;
typedef struct IDirect3DCubeTexture8 IDirect3DCubeTexture8;
typedef struct IDirect3DVolumeTexture8 IDirect3DVolumeTexture8;
typedef struct IDirect3DSurface8 IDirect3DSurface8;
typedef struct IDirect3DVolume8 IDirect3DVolume8;
typedef struct IDirect3DVertexBuffer8 IDirect3DVertexBuffer8;
typedef struct IDirect3DIndexBuffer8 IDirect3DIndexBuffer8;
typedef struct IDirect3DSwapChain8 IDirect3DSwapChain8;

// ============================================================================
// IDirect3D8 Interface
// ============================================================================

typedef struct IDirect3D8Vtbl {
    // IUnknown
    HRESULT (WINAPI *QueryInterface)(IDirect3D8 *This, REFIID riid, void **ppvObject);
    ULONG (WINAPI *AddRef)(IDirect3D8 *This);
    ULONG (WINAPI *Release)(IDirect3D8 *This);

    // IDirect3D8
    HRESULT (WINAPI *RegisterSoftwareDevice)(IDirect3D8 *This, void *pInitializeFunction);
    UINT (WINAPI *GetAdapterCount)(IDirect3D8 *This);
    HRESULT (WINAPI *GetAdapterIdentifier)(IDirect3D8 *This, UINT Adapter, DWORD Flags, D3DADAPTER_IDENTIFIER8 *pIdentifier);
    UINT (WINAPI *GetAdapterModeCount)(IDirect3D8 *This, UINT Adapter);
    HRESULT (WINAPI *EnumAdapterModes)(IDirect3D8 *This, UINT Adapter, UINT Mode, D3DDISPLAYMODE *pMode);
    HRESULT (WINAPI *GetAdapterDisplayMode)(IDirect3D8 *This, UINT Adapter, D3DDISPLAYMODE *pMode);
    HRESULT (WINAPI *CheckDeviceType)(IDirect3D8 *This, UINT Adapter, D3DDEVTYPE CheckType, D3DFORMAT DisplayFormat, D3DFORMAT BackBufferFormat, BOOL Windowed);
    HRESULT (WINAPI *CheckDeviceFormat)(IDirect3D8 *This, UINT Adapter, D3DDEVTYPE DeviceType, D3DFORMAT AdapterFormat, DWORD Usage, D3DRESOURCETYPE RType, D3DFORMAT CheckFormat);
    HRESULT (WINAPI *CheckDeviceMultiSampleType)(IDirect3D8 *This, UINT Adapter, D3DDEVTYPE DeviceType, D3DFORMAT SurfaceFormat, BOOL Windowed, D3DMULTISAMPLE_TYPE MultiSampleType);
    HRESULT (WINAPI *CheckDepthStencilMatch)(IDirect3D8 *This, UINT Adapter, D3DDEVTYPE DeviceType, D3DFORMAT AdapterFormat, D3DFORMAT RenderTargetFormat, D3DFORMAT DepthStencilFormat);
    HRESULT (WINAPI *GetDeviceCaps)(IDirect3D8 *This, UINT Adapter, D3DDEVTYPE DeviceType, D3DCAPS8 *pCaps);
    HMONITOR (WINAPI *GetAdapterMonitor)(IDirect3D8 *This, UINT Adapter);
    HRESULT (WINAPI *CreateDevice)(IDirect3D8 *This, UINT Adapter, D3DDEVTYPE DeviceType, HWND hFocusWindow, DWORD BehaviorFlags, D3DPRESENT_PARAMETERS8 *pPresentationParameters, IDirect3DDevice8 **ppReturnedDeviceInterface);
} IDirect3D8Vtbl;

struct IDirect3D8 {
    IDirect3D8Vtbl *lpVtbl;
};

// ============================================================================
// IDirect3DDevice8 Interface
// ============================================================================

typedef struct IDirect3DDevice8Vtbl {
    // IUnknown
    HRESULT (WINAPI *QueryInterface)(IDirect3DDevice8 *This, REFIID riid, void **ppvObject);
    ULONG (WINAPI *AddRef)(IDirect3DDevice8 *This);
    ULONG (WINAPI *Release)(IDirect3DDevice8 *This);

    // IDirect3DDevice8
    HRESULT (WINAPI *TestCooperativeLevel)(IDirect3DDevice8 *This);
    UINT (WINAPI *GetAvailableTextureMem)(IDirect3DDevice8 *This);
    HRESULT (WINAPI *ResourceManagerDiscardBytes)(IDirect3DDevice8 *This, DWORD Bytes);
    HRESULT (WINAPI *GetDirect3D)(IDirect3DDevice8 *This, IDirect3D8 **ppD3D8);
    HRESULT (WINAPI *GetDeviceCaps)(IDirect3DDevice8 *This, D3DCAPS8 *pCaps);
    HRESULT (WINAPI *GetDisplayMode)(IDirect3DDevice8 *This, D3DDISPLAYMODE *pMode);
    HRESULT (WINAPI *GetCreationParameters)(IDirect3DDevice8 *This, void *pParameters);
    HRESULT (WINAPI *SetCursorProperties)(IDirect3DDevice8 *This, UINT XHotSpot, UINT YHotSpot, IDirect3DSurface8 *pCursorBitmap);
    void (WINAPI *SetCursorPosition)(IDirect3DDevice8 *This, int X, int Y, DWORD Flags);
    BOOL (WINAPI *ShowCursor)(IDirect3DDevice8 *This, BOOL bShow);
    HRESULT (WINAPI *CreateAdditionalSwapChain)(IDirect3DDevice8 *This, D3DPRESENT_PARAMETERS8 *pPresentationParameters, IDirect3DSwapChain8 **ppSwapChain);
    HRESULT (WINAPI *Reset)(IDirect3DDevice8 *This, D3DPRESENT_PARAMETERS8 *pPresentationParameters);
    HRESULT (WINAPI *Present)(IDirect3DDevice8 *This, const RECT *pSourceRect, const RECT *pDestRect, HWND hDestWindowOverride, const RGNDATA *pDirtyRegion);
    HRESULT (WINAPI *GetBackBuffer)(IDirect3DDevice8 *This, UINT BackBuffer, D3DBACKBUFFER_TYPE Type, IDirect3DSurface8 **ppBackBuffer);
    HRESULT (WINAPI *GetRasterStatus)(IDirect3DDevice8 *This, D3DRASTER_STATUS *pRasterStatus);
    void (WINAPI *SetGammaRamp)(IDirect3DDevice8 *This, DWORD Flags, const D3DGAMMARAMP *pRamp);
    void (WINAPI *GetGammaRamp)(IDirect3DDevice8 *This, D3DGAMMARAMP *pRamp);
    HRESULT (WINAPI *CreateTexture)(IDirect3DDevice8 *This, UINT Width, UINT Height, UINT Levels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DTexture8 **ppTexture);
    HRESULT (WINAPI *CreateVolumeTexture)(IDirect3DDevice8 *This, UINT Width, UINT Height, UINT Depth, UINT Levels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DVolumeTexture8 **ppVolumeTexture);
    HRESULT (WINAPI *CreateCubeTexture)(IDirect3DDevice8 *This, UINT EdgeLength, UINT Levels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DCubeTexture8 **ppCubeTexture);
    HRESULT (WINAPI *CreateVertexBuffer)(IDirect3DDevice8 *This, UINT Length, DWORD Usage, DWORD FVF, D3DPOOL Pool, IDirect3DVertexBuffer8 **ppVertexBuffer);
    HRESULT (WINAPI *CreateIndexBuffer)(IDirect3DDevice8 *This, UINT Length, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DIndexBuffer8 **ppIndexBuffer);
    HRESULT (WINAPI *CreateRenderTarget)(IDirect3DDevice8 *This, UINT Width, UINT Height, D3DFORMAT Format, D3DMULTISAMPLE_TYPE MultiSample, BOOL Lockable, IDirect3DSurface8 **ppSurface);
    HRESULT (WINAPI *CreateDepthStencilSurface)(IDirect3DDevice8 *This, UINT Width, UINT Height, D3DFORMAT Format, D3DMULTISAMPLE_TYPE MultiSample, IDirect3DSurface8 **ppSurface);
    HRESULT (WINAPI *CreateImageSurface)(IDirect3DDevice8 *This, UINT Width, UINT Height, D3DFORMAT Format, IDirect3DSurface8 **ppSurface);
    HRESULT (WINAPI *CopyRects)(IDirect3DDevice8 *This, IDirect3DSurface8 *pSourceSurface, const RECT *pSourceRectsArray, UINT cRects, IDirect3DSurface8 *pDestinationSurface, const POINT *pDestPointsArray);
    HRESULT (WINAPI *UpdateTexture)(IDirect3DDevice8 *This, IDirect3DBaseTexture8 *pSourceTexture, IDirect3DBaseTexture8 *pDestinationTexture);
    HRESULT (WINAPI *GetFrontBuffer)(IDirect3DDevice8 *This, IDirect3DSurface8 *pDestSurface);
    HRESULT (WINAPI *SetRenderTarget)(IDirect3DDevice8 *This, IDirect3DSurface8 *pRenderTarget, IDirect3DSurface8 *pNewZStencil);
    HRESULT (WINAPI *GetRenderTarget)(IDirect3DDevice8 *This, IDirect3DSurface8 **ppRenderTarget);
    HRESULT (WINAPI *GetDepthStencilSurface)(IDirect3DDevice8 *This, IDirect3DSurface8 **ppZStencilSurface);
    HRESULT (WINAPI *BeginScene)(IDirect3DDevice8 *This);
    HRESULT (WINAPI *EndScene)(IDirect3DDevice8 *This);
    HRESULT (WINAPI *Clear)(IDirect3DDevice8 *This, DWORD Count, const D3DRECT *pRects, DWORD Flags, D3DCOLOR Color, float Z, DWORD Stencil);
    HRESULT (WINAPI *SetTransform)(IDirect3DDevice8 *This, D3DTRANSFORMSTATETYPE State, const D3DMATRIX *pMatrix);
    HRESULT (WINAPI *GetTransform)(IDirect3DDevice8 *This, D3DTRANSFORMSTATETYPE State, D3DMATRIX *pMatrix);
    HRESULT (WINAPI *MultiplyTransform)(IDirect3DDevice8 *This, D3DTRANSFORMSTATETYPE State, const D3DMATRIX *pMatrix);
    HRESULT (WINAPI *SetViewport)(IDirect3DDevice8 *This, const D3DVIEWPORT8 *pViewport);
    HRESULT (WINAPI *GetViewport)(IDirect3DDevice8 *This, D3DVIEWPORT8 *pViewport);
    HRESULT (WINAPI *SetMaterial)(IDirect3DDevice8 *This, const D3DMATERIAL8 *pMaterial);
    HRESULT (WINAPI *GetMaterial)(IDirect3DDevice8 *This, D3DMATERIAL8 *pMaterial);
    HRESULT (WINAPI *SetLight)(IDirect3DDevice8 *This, DWORD Index, const D3DLIGHT8 *pLight);
    HRESULT (WINAPI *GetLight)(IDirect3DDevice8 *This, DWORD Index, D3DLIGHT8 *pLight);
    HRESULT (WINAPI *LightEnable)(IDirect3DDevice8 *This, DWORD Index, BOOL Enable);
    HRESULT (WINAPI *GetLightEnable)(IDirect3DDevice8 *This, DWORD Index, BOOL *pEnable);
    HRESULT (WINAPI *SetClipPlane)(IDirect3DDevice8 *This, DWORD Index, const float *pPlane);
    HRESULT (WINAPI *GetClipPlane)(IDirect3DDevice8 *This, DWORD Index, float *pPlane);
    HRESULT (WINAPI *SetRenderState)(IDirect3DDevice8 *This, D3DRENDERSTATETYPE State, DWORD Value);
    HRESULT (WINAPI *GetRenderState)(IDirect3DDevice8 *This, D3DRENDERSTATETYPE State, DWORD *pValue);
    HRESULT (WINAPI *BeginStateBlock)(IDirect3DDevice8 *This);
    HRESULT (WINAPI *EndStateBlock)(IDirect3DDevice8 *This, DWORD *pToken);
    HRESULT (WINAPI *ApplyStateBlock)(IDirect3DDevice8 *This, DWORD Token);
    HRESULT (WINAPI *CaptureStateBlock)(IDirect3DDevice8 *This, DWORD Token);
    HRESULT (WINAPI *DeleteStateBlock)(IDirect3DDevice8 *This, DWORD Token);
    HRESULT (WINAPI *CreateStateBlock)(IDirect3DDevice8 *This, D3DSTATEBLOCKTYPE Type, DWORD *pToken);
    HRESULT (WINAPI *SetClipStatus)(IDirect3DDevice8 *This, const D3DCLIPSTATUS8 *pClipStatus);
    HRESULT (WINAPI *GetClipStatus)(IDirect3DDevice8 *This, D3DCLIPSTATUS8 *pClipStatus);
    HRESULT (WINAPI *GetTexture)(IDirect3DDevice8 *This, DWORD Stage, IDirect3DBaseTexture8 **ppTexture);
    HRESULT (WINAPI *SetTexture)(IDirect3DDevice8 *This, DWORD Stage, IDirect3DBaseTexture8 *pTexture);
    HRESULT (WINAPI *GetTextureStageState)(IDirect3DDevice8 *This, DWORD Stage, D3DTEXTURESTAGESTATETYPE Type, DWORD *pValue);
    HRESULT (WINAPI *SetTextureStageState)(IDirect3DDevice8 *This, DWORD Stage, D3DTEXTURESTAGESTATETYPE Type, DWORD Value);
    HRESULT (WINAPI *ValidateDevice)(IDirect3DDevice8 *This, DWORD *pNumPasses);
    HRESULT (WINAPI *GetInfo)(IDirect3DDevice8 *This, DWORD DevInfoID, void *pDevInfoStruct, DWORD DevInfoStructSize);
    HRESULT (WINAPI *SetPaletteEntries)(IDirect3DDevice8 *This, UINT PaletteNumber, const PALETTEENTRY *pEntries);
    HRESULT (WINAPI *GetPaletteEntries)(IDirect3DDevice8 *This, UINT PaletteNumber, PALETTEENTRY *pEntries);
    HRESULT (WINAPI *SetCurrentTexturePalette)(IDirect3DDevice8 *This, UINT PaletteNumber);
    HRESULT (WINAPI *GetCurrentTexturePalette)(IDirect3DDevice8 *This, UINT *pPaletteNumber);
    HRESULT (WINAPI *DrawPrimitive)(IDirect3DDevice8 *This, D3DPRIMITIVETYPE PrimitiveType, UINT StartVertex, UINT PrimitiveCount);
    HRESULT (WINAPI *DrawIndexedPrimitive)(IDirect3DDevice8 *This, D3DPRIMITIVETYPE PrimitiveType, UINT MinVertexIndex, UINT NumVertices, UINT StartIndex, UINT PrimitiveCount);
    HRESULT (WINAPI *DrawPrimitiveUP)(IDirect3DDevice8 *This, D3DPRIMITIVETYPE PrimitiveType, UINT PrimitiveCount, const void *pVertexStreamZeroData, UINT VertexStreamZeroStride);
    HRESULT (WINAPI *DrawIndexedPrimitiveUP)(IDirect3DDevice8 *This, D3DPRIMITIVETYPE PrimitiveType, UINT MinVertexIndex, UINT NumVertices, UINT PrimitiveCount, const void *pIndexData, D3DFORMAT IndexDataFormat, const void *pVertexStreamZeroData, UINT VertexStreamZeroStride);
    HRESULT (WINAPI *ProcessVertices)(IDirect3DDevice8 *This, UINT SrcStartIndex, UINT DestIndex, UINT VertexCount, IDirect3DVertexBuffer8 *pDestBuffer, DWORD Flags);
    HRESULT (WINAPI *CreateVertexShader)(IDirect3DDevice8 *This, const DWORD *pDeclaration, const DWORD *pFunction, DWORD *pHandle, DWORD Usage);
    HRESULT (WINAPI *SetVertexShader)(IDirect3DDevice8 *This, DWORD Handle);
    HRESULT (WINAPI *GetVertexShader)(IDirect3DDevice8 *This, DWORD *pHandle);
    HRESULT (WINAPI *DeleteVertexShader)(IDirect3DDevice8 *This, DWORD Handle);
    HRESULT (WINAPI *SetVertexShaderConstant)(IDirect3DDevice8 *This, DWORD Register, const void *pConstantData, DWORD ConstantCount);
    HRESULT (WINAPI *GetVertexShaderConstant)(IDirect3DDevice8 *This, DWORD Register, void *pConstantData, DWORD ConstantCount);
    HRESULT (WINAPI *GetVertexShaderDeclaration)(IDirect3DDevice8 *This, DWORD Handle, void *pData, DWORD *pSizeOfData);
    HRESULT (WINAPI *GetVertexShaderFunction)(IDirect3DDevice8 *This, DWORD Handle, void *pData, DWORD *pSizeOfData);
    HRESULT (WINAPI *SetStreamSource)(IDirect3DDevice8 *This, UINT StreamNumber, IDirect3DVertexBuffer8 *pStreamData, UINT Stride);
    HRESULT (WINAPI *GetStreamSource)(IDirect3DDevice8 *This, UINT StreamNumber, IDirect3DVertexBuffer8 **ppStreamData, UINT *pStride);
    HRESULT (WINAPI *SetIndices)(IDirect3DDevice8 *This, IDirect3DIndexBuffer8 *pIndexData, UINT BaseVertexIndex);
    HRESULT (WINAPI *GetIndices)(IDirect3DDevice8 *This, IDirect3DIndexBuffer8 **ppIndexData, UINT *pBaseVertexIndex);
    HRESULT (WINAPI *CreatePixelShader)(IDirect3DDevice8 *This, const DWORD *pFunction, DWORD *pHandle);
    HRESULT (WINAPI *SetPixelShader)(IDirect3DDevice8 *This, DWORD Handle);
    HRESULT (WINAPI *GetPixelShader)(IDirect3DDevice8 *This, DWORD *pHandle);
    HRESULT (WINAPI *DeletePixelShader)(IDirect3DDevice8 *This, DWORD Handle);
    HRESULT (WINAPI *SetPixelShaderConstant)(IDirect3DDevice8 *This, DWORD Register, const void *pConstantData, DWORD ConstantCount);
    HRESULT (WINAPI *GetPixelShaderConstant)(IDirect3DDevice8 *This, DWORD Register, void *pConstantData, DWORD ConstantCount);
    HRESULT (WINAPI *GetPixelShaderFunction)(IDirect3DDevice8 *This, DWORD Handle, void *pData, DWORD *pSizeOfData);
    HRESULT (WINAPI *DrawRectPatch)(IDirect3DDevice8 *This, UINT Handle, const float *pNumSegs, const void *pRectPatchInfo);
    HRESULT (WINAPI *DrawTriPatch)(IDirect3DDevice8 *This, UINT Handle, const float *pNumSegs, const void *pTriPatchInfo);
    HRESULT (WINAPI *DeletePatch)(IDirect3DDevice8 *This, UINT Handle);
} IDirect3DDevice8Vtbl;

struct IDirect3DDevice8 {
    IDirect3DDevice8Vtbl *lpVtbl;
};

// ============================================================================
// IDirect3DResource8 Interface
// ============================================================================

typedef struct IDirect3DResource8Vtbl {
    // IUnknown
    HRESULT (WINAPI *QueryInterface)(IDirect3DResource8 *This, REFIID riid, void **ppvObject);
    ULONG (WINAPI *AddRef)(IDirect3DResource8 *This);
    ULONG (WINAPI *Release)(IDirect3DResource8 *This);

    // IDirect3DResource8
    HRESULT (WINAPI *GetDevice)(IDirect3DResource8 *This, IDirect3DDevice8 **ppDevice);
    HRESULT (WINAPI *SetPrivateData)(IDirect3DResource8 *This, REFGUID refguid, const void *pData, DWORD SizeOfData, DWORD Flags);
    HRESULT (WINAPI *GetPrivateData)(IDirect3DResource8 *This, REFGUID refguid, void *pData, DWORD *pSizeOfData);
    HRESULT (WINAPI *FreePrivateData)(IDirect3DResource8 *This, REFGUID refguid);
    DWORD (WINAPI *SetPriority)(IDirect3DResource8 *This, DWORD PriorityNew);
    DWORD (WINAPI *GetPriority)(IDirect3DResource8 *This);
    void (WINAPI *PreLoad)(IDirect3DResource8 *This);
    D3DRESOURCETYPE (WINAPI *GetType)(IDirect3DResource8 *This);
} IDirect3DResource8Vtbl;

struct IDirect3DResource8 {
    IDirect3DResource8Vtbl *lpVtbl;
};

// ============================================================================
// IDirect3DBaseTexture8 Interface
// ============================================================================

typedef struct IDirect3DBaseTexture8Vtbl {
    // IUnknown
    HRESULT (WINAPI *QueryInterface)(IDirect3DBaseTexture8 *This, REFIID riid, void **ppvObject);
    ULONG (WINAPI *AddRef)(IDirect3DBaseTexture8 *This);
    ULONG (WINAPI *Release)(IDirect3DBaseTexture8 *This);

    // IDirect3DResource8
    HRESULT (WINAPI *GetDevice)(IDirect3DBaseTexture8 *This, IDirect3DDevice8 **ppDevice);
    HRESULT (WINAPI *SetPrivateData)(IDirect3DBaseTexture8 *This, REFGUID refguid, const void *pData, DWORD SizeOfData, DWORD Flags);
    HRESULT (WINAPI *GetPrivateData)(IDirect3DBaseTexture8 *This, REFGUID refguid, void *pData, DWORD *pSizeOfData);
    HRESULT (WINAPI *FreePrivateData)(IDirect3DBaseTexture8 *This, REFGUID refguid);
    DWORD (WINAPI *SetPriority)(IDirect3DBaseTexture8 *This, DWORD PriorityNew);
    DWORD (WINAPI *GetPriority)(IDirect3DBaseTexture8 *This);
    void (WINAPI *PreLoad)(IDirect3DBaseTexture8 *This);
    D3DRESOURCETYPE (WINAPI *GetType)(IDirect3DBaseTexture8 *This);

    // IDirect3DBaseTexture8
    DWORD (WINAPI *SetLOD)(IDirect3DBaseTexture8 *This, DWORD LODNew);
    DWORD (WINAPI *GetLOD)(IDirect3DBaseTexture8 *This);
    DWORD (WINAPI *GetLevelCount)(IDirect3DBaseTexture8 *This);
} IDirect3DBaseTexture8Vtbl;

struct IDirect3DBaseTexture8 {
    IDirect3DBaseTexture8Vtbl *lpVtbl;
};

// ============================================================================
// IDirect3DTexture8 Interface
// ============================================================================

typedef struct IDirect3DTexture8Vtbl {
    // IUnknown
    HRESULT (WINAPI *QueryInterface)(IDirect3DTexture8 *This, REFIID riid, void **ppvObject);
    ULONG (WINAPI *AddRef)(IDirect3DTexture8 *This);
    ULONG (WINAPI *Release)(IDirect3DTexture8 *This);

    // IDirect3DResource8
    HRESULT (WINAPI *GetDevice)(IDirect3DTexture8 *This, IDirect3DDevice8 **ppDevice);
    HRESULT (WINAPI *SetPrivateData)(IDirect3DTexture8 *This, REFGUID refguid, const void *pData, DWORD SizeOfData, DWORD Flags);
    HRESULT (WINAPI *GetPrivateData)(IDirect3DTexture8 *This, REFGUID refguid, void *pData, DWORD *pSizeOfData);
    HRESULT (WINAPI *FreePrivateData)(IDirect3DTexture8 *This, REFGUID refguid);
    DWORD (WINAPI *SetPriority)(IDirect3DTexture8 *This, DWORD PriorityNew);
    DWORD (WINAPI *GetPriority)(IDirect3DTexture8 *This);
    void (WINAPI *PreLoad)(IDirect3DTexture8 *This);
    D3DRESOURCETYPE (WINAPI *GetType)(IDirect3DTexture8 *This);

    // IDirect3DBaseTexture8
    DWORD (WINAPI *SetLOD)(IDirect3DTexture8 *This, DWORD LODNew);
    DWORD (WINAPI *GetLOD)(IDirect3DTexture8 *This);
    DWORD (WINAPI *GetLevelCount)(IDirect3DTexture8 *This);

    // IDirect3DTexture8
    HRESULT (WINAPI *GetLevelDesc)(IDirect3DTexture8 *This, UINT Level, D3DSURFACE_DESC8 *pDesc);
    HRESULT (WINAPI *GetSurfaceLevel)(IDirect3DTexture8 *This, UINT Level, IDirect3DSurface8 **ppSurfaceLevel);
    HRESULT (WINAPI *LockRect)(IDirect3DTexture8 *This, UINT Level, D3DLOCKED_RECT *pLockedRect, const RECT *pRect, DWORD Flags);
    HRESULT (WINAPI *UnlockRect)(IDirect3DTexture8 *This, UINT Level);
    HRESULT (WINAPI *AddDirtyRect)(IDirect3DTexture8 *This, const RECT *pDirtyRect);
} IDirect3DTexture8Vtbl;

struct IDirect3DTexture8 {
    IDirect3DTexture8Vtbl *lpVtbl;
};

// ============================================================================
// IDirect3DSurface8 Interface
// ============================================================================

typedef struct IDirect3DSurface8Vtbl {
    // IUnknown
    HRESULT (WINAPI *QueryInterface)(IDirect3DSurface8 *This, REFIID riid, void **ppvObject);
    ULONG (WINAPI *AddRef)(IDirect3DSurface8 *This);
    ULONG (WINAPI *Release)(IDirect3DSurface8 *This);

    // IDirect3DSurface8
    HRESULT (WINAPI *GetDevice)(IDirect3DSurface8 *This, IDirect3DDevice8 **ppDevice);
    HRESULT (WINAPI *SetPrivateData)(IDirect3DSurface8 *This, REFGUID refguid, const void *pData, DWORD SizeOfData, DWORD Flags);
    HRESULT (WINAPI *GetPrivateData)(IDirect3DSurface8 *This, REFGUID refguid, void *pData, DWORD *pSizeOfData);
    HRESULT (WINAPI *FreePrivateData)(IDirect3DSurface8 *This, REFGUID refguid);
    HRESULT (WINAPI *GetContainer)(IDirect3DSurface8 *This, REFIID riid, void **ppContainer);
    HRESULT (WINAPI *GetDesc)(IDirect3DSurface8 *This, D3DSURFACE_DESC8 *pDesc);
    HRESULT (WINAPI *LockRect)(IDirect3DSurface8 *This, D3DLOCKED_RECT *pLockedRect, const RECT *pRect, DWORD Flags);
    HRESULT (WINAPI *UnlockRect)(IDirect3DSurface8 *This);
} IDirect3DSurface8Vtbl;

struct IDirect3DSurface8 {
    IDirect3DSurface8Vtbl *lpVtbl;
};

// ============================================================================
// IDirect3DVertexBuffer8 Interface
// ============================================================================

typedef struct IDirect3DVertexBuffer8Vtbl {
    // IUnknown
    HRESULT (WINAPI *QueryInterface)(IDirect3DVertexBuffer8 *This, REFIID riid, void **ppvObject);
    ULONG (WINAPI *AddRef)(IDirect3DVertexBuffer8 *This);
    ULONG (WINAPI *Release)(IDirect3DVertexBuffer8 *This);

    // IDirect3DResource8
    HRESULT (WINAPI *GetDevice)(IDirect3DVertexBuffer8 *This, IDirect3DDevice8 **ppDevice);
    HRESULT (WINAPI *SetPrivateData)(IDirect3DVertexBuffer8 *This, REFGUID refguid, const void *pData, DWORD SizeOfData, DWORD Flags);
    HRESULT (WINAPI *GetPrivateData)(IDirect3DVertexBuffer8 *This, REFGUID refguid, void *pData, DWORD *pSizeOfData);
    HRESULT (WINAPI *FreePrivateData)(IDirect3DVertexBuffer8 *This, REFGUID refguid);
    DWORD (WINAPI *SetPriority)(IDirect3DVertexBuffer8 *This, DWORD PriorityNew);
    DWORD (WINAPI *GetPriority)(IDirect3DVertexBuffer8 *This);
    void (WINAPI *PreLoad)(IDirect3DVertexBuffer8 *This);
    D3DRESOURCETYPE (WINAPI *GetType)(IDirect3DVertexBuffer8 *This);

    // IDirect3DVertexBuffer8
    HRESULT (WINAPI *Lock)(IDirect3DVertexBuffer8 *This, UINT OffsetToLock, UINT SizeToLock, BYTE **ppbData, DWORD Flags);
    HRESULT (WINAPI *Unlock)(IDirect3DVertexBuffer8 *This);
    HRESULT (WINAPI *GetDesc)(IDirect3DVertexBuffer8 *This, D3DVERTEXBUFFER_DESC *pDesc);
} IDirect3DVertexBuffer8Vtbl;

struct IDirect3DVertexBuffer8 {
    IDirect3DVertexBuffer8Vtbl *lpVtbl;
};

// ============================================================================
// IDirect3DIndexBuffer8 Interface
// ============================================================================

typedef struct IDirect3DIndexBuffer8Vtbl {
    // IUnknown
    HRESULT (WINAPI *QueryInterface)(IDirect3DIndexBuffer8 *This, REFIID riid, void **ppvObject);
    ULONG (WINAPI *AddRef)(IDirect3DIndexBuffer8 *This);
    ULONG (WINAPI *Release)(IDirect3DIndexBuffer8 *This);

    // IDirect3DResource8
    HRESULT (WINAPI *GetDevice)(IDirect3DIndexBuffer8 *This, IDirect3DDevice8 **ppDevice);
    HRESULT (WINAPI *SetPrivateData)(IDirect3DIndexBuffer8 *This, REFGUID refguid, const void *pData, DWORD SizeOfData, DWORD Flags);
    HRESULT (WINAPI *GetPrivateData)(IDirect3DIndexBuffer8 *This, REFGUID refguid, void *pData, DWORD *pSizeOfData);
    HRESULT (WINAPI *FreePrivateData)(IDirect3DIndexBuffer8 *This, REFGUID refguid);
    DWORD (WINAPI *SetPriority)(IDirect3DIndexBuffer8 *This, DWORD PriorityNew);
    DWORD (WINAPI *GetPriority)(IDirect3DIndexBuffer8 *This);
    void (WINAPI *PreLoad)(IDirect3DIndexBuffer8 *This);
    D3DRESOURCETYPE (WINAPI *GetType)(IDirect3DIndexBuffer8 *This);

    // IDirect3DIndexBuffer8
    HRESULT (WINAPI *Lock)(IDirect3DIndexBuffer8 *This, UINT OffsetToLock, UINT SizeToLock, BYTE **ppbData, DWORD Flags);
    HRESULT (WINAPI *Unlock)(IDirect3DIndexBuffer8 *This);
    HRESULT (WINAPI *GetDesc)(IDirect3DIndexBuffer8 *This, D3DINDEXBUFFER_DESC *pDesc);
} IDirect3DIndexBuffer8Vtbl;

struct IDirect3DIndexBuffer8 {
    IDirect3DIndexBuffer8Vtbl *lpVtbl;
};

// ============================================================================
// IDirect3DSwapChain8 Interface
// ============================================================================

typedef struct IDirect3DSwapChain8Vtbl {
    // IUnknown
    HRESULT (WINAPI *QueryInterface)(IDirect3DSwapChain8 *This, REFIID riid, void **ppvObject);
    ULONG (WINAPI *AddRef)(IDirect3DSwapChain8 *This);
    ULONG (WINAPI *Release)(IDirect3DSwapChain8 *This);

    // IDirect3DSwapChain8
    HRESULT (WINAPI *Present)(IDirect3DSwapChain8 *This, const RECT *pSourceRect, const RECT *pDestRect, HWND hDestWindowOverride, const RGNDATA *pDirtyRegion);
    HRESULT (WINAPI *GetBackBuffer)(IDirect3DSwapChain8 *This, UINT BackBuffer, D3DBACKBUFFER_TYPE Type, IDirect3DSurface8 **ppBackBuffer);
} IDirect3DSwapChain8Vtbl;

struct IDirect3DSwapChain8 {
    IDirect3DSwapChain8Vtbl *lpVtbl;
};

// ============================================================================
// IDirect3DCubeTexture8 Interface
// ============================================================================

typedef struct IDirect3DCubeTexture8Vtbl {
    // IUnknown
    HRESULT (WINAPI *QueryInterface)(IDirect3DCubeTexture8 *This, REFIID riid, void **ppvObject);
    ULONG (WINAPI *AddRef)(IDirect3DCubeTexture8 *This);
    ULONG (WINAPI *Release)(IDirect3DCubeTexture8 *This);

    // IDirect3DResource8
    HRESULT (WINAPI *GetDevice)(IDirect3DCubeTexture8 *This, IDirect3DDevice8 **ppDevice);
    HRESULT (WINAPI *SetPrivateData)(IDirect3DCubeTexture8 *This, REFGUID refguid, const void *pData, DWORD SizeOfData, DWORD Flags);
    HRESULT (WINAPI *GetPrivateData)(IDirect3DCubeTexture8 *This, REFGUID refguid, void *pData, DWORD *pSizeOfData);
    HRESULT (WINAPI *FreePrivateData)(IDirect3DCubeTexture8 *This, REFGUID refguid);
    DWORD (WINAPI *SetPriority)(IDirect3DCubeTexture8 *This, DWORD PriorityNew);
    DWORD (WINAPI *GetPriority)(IDirect3DCubeTexture8 *This);
    void (WINAPI *PreLoad)(IDirect3DCubeTexture8 *This);
    D3DRESOURCETYPE (WINAPI *GetType)(IDirect3DCubeTexture8 *This);

    // IDirect3DBaseTexture8
    DWORD (WINAPI *SetLOD)(IDirect3DCubeTexture8 *This, DWORD LODNew);
    DWORD (WINAPI *GetLOD)(IDirect3DCubeTexture8 *This);
    DWORD (WINAPI *GetLevelCount)(IDirect3DCubeTexture8 *This);

    // IDirect3DCubeTexture8
    HRESULT (WINAPI *GetLevelDesc)(IDirect3DCubeTexture8 *This, UINT Level, D3DSURFACE_DESC8 *pDesc);
    HRESULT (WINAPI *GetCubeMapSurface)(IDirect3DCubeTexture8 *This, D3DCUBEMAP_FACES FaceType, UINT Level, IDirect3DSurface8 **ppCubeMapSurface);
    HRESULT (WINAPI *LockRect)(IDirect3DCubeTexture8 *This, D3DCUBEMAP_FACES FaceType, UINT Level, D3DLOCKED_RECT *pLockedRect, const RECT *pRect, DWORD Flags);
    HRESULT (WINAPI *UnlockRect)(IDirect3DCubeTexture8 *This, D3DCUBEMAP_FACES FaceType, UINT Level);
    HRESULT (WINAPI *AddDirtyRect)(IDirect3DCubeTexture8 *This, D3DCUBEMAP_FACES FaceType, const RECT *pDirtyRect);
} IDirect3DCubeTexture8Vtbl;

struct IDirect3DCubeTexture8 {
    IDirect3DCubeTexture8Vtbl *lpVtbl;
};

// ============================================================================
// IDirect3DVolumeTexture8 Interface
// ============================================================================

typedef struct IDirect3DVolumeTexture8Vtbl {
    // IUnknown
    HRESULT (WINAPI *QueryInterface)(IDirect3DVolumeTexture8 *This, REFIID riid, void **ppvObject);
    ULONG (WINAPI *AddRef)(IDirect3DVolumeTexture8 *This);
    ULONG (WINAPI *Release)(IDirect3DVolumeTexture8 *This);

    // IDirect3DResource8
    HRESULT (WINAPI *GetDevice)(IDirect3DVolumeTexture8 *This, IDirect3DDevice8 **ppDevice);
    HRESULT (WINAPI *SetPrivateData)(IDirect3DVolumeTexture8 *This, REFGUID refguid, const void *pData, DWORD SizeOfData, DWORD Flags);
    HRESULT (WINAPI *GetPrivateData)(IDirect3DVolumeTexture8 *This, REFGUID refguid, void *pData, DWORD *pSizeOfData);
    HRESULT (WINAPI *FreePrivateData)(IDirect3DVolumeTexture8 *This, REFGUID refguid);
    DWORD (WINAPI *SetPriority)(IDirect3DVolumeTexture8 *This, DWORD PriorityNew);
    DWORD (WINAPI *GetPriority)(IDirect3DVolumeTexture8 *This);
    void (WINAPI *PreLoad)(IDirect3DVolumeTexture8 *This);
    D3DRESOURCETYPE (WINAPI *GetType)(IDirect3DVolumeTexture8 *This);

    // IDirect3DBaseTexture8
    DWORD (WINAPI *SetLOD)(IDirect3DVolumeTexture8 *This, DWORD LODNew);
    DWORD (WINAPI *GetLOD)(IDirect3DVolumeTexture8 *This);
    DWORD (WINAPI *GetLevelCount)(IDirect3DVolumeTexture8 *This);

    // IDirect3DVolumeTexture8
    HRESULT (WINAPI *GetLevelDesc)(IDirect3DVolumeTexture8 *This, UINT Level, D3DVOLUME_DESC8 *pDesc);
    HRESULT (WINAPI *GetVolumeLevel)(IDirect3DVolumeTexture8 *This, UINT Level, IDirect3DVolume8 **ppVolumeLevel);
    HRESULT (WINAPI *LockBox)(IDirect3DVolumeTexture8 *This, UINT Level, D3DLOCKED_BOX *pLockedVolume, const D3DBOX *pBox, DWORD Flags);
    HRESULT (WINAPI *UnlockBox)(IDirect3DVolumeTexture8 *This, UINT Level);
    HRESULT (WINAPI *AddDirtyBox)(IDirect3DVolumeTexture8 *This, const D3DBOX *pDirtyBox);
} IDirect3DVolumeTexture8Vtbl;

struct IDirect3DVolumeTexture8 {
    IDirect3DVolumeTexture8Vtbl *lpVtbl;
};

// ============================================================================
// IDirect3DVolume8 Interface
// ============================================================================

typedef struct IDirect3DVolume8Vtbl {
    // IUnknown
    HRESULT (WINAPI *QueryInterface)(IDirect3DVolume8 *This, REFIID riid, void **ppvObject);
    ULONG (WINAPI *AddRef)(IDirect3DVolume8 *This);
    ULONG (WINAPI *Release)(IDirect3DVolume8 *This);

    // IDirect3DVolume8
    HRESULT (WINAPI *GetDevice)(IDirect3DVolume8 *This, IDirect3DDevice8 **ppDevice);
    HRESULT (WINAPI *SetPrivateData)(IDirect3DVolume8 *This, REFGUID refguid, const void *pData, DWORD SizeOfData, DWORD Flags);
    HRESULT (WINAPI *GetPrivateData)(IDirect3DVolume8 *This, REFGUID refguid, void *pData, DWORD *pSizeOfData);
    HRESULT (WINAPI *FreePrivateData)(IDirect3DVolume8 *This, REFGUID refguid);
    HRESULT (WINAPI *GetContainer)(IDirect3DVolume8 *This, REFIID riid, void **ppContainer);
    HRESULT (WINAPI *GetDesc)(IDirect3DVolume8 *This, D3DVOLUME_DESC8 *pDesc);
    HRESULT (WINAPI *LockBox)(IDirect3DVolume8 *This, D3DLOCKED_BOX *pLockedVolume, const D3DBOX *pBox, DWORD Flags);
    HRESULT (WINAPI *UnlockBox)(IDirect3DVolume8 *This);
} IDirect3DVolume8Vtbl;

struct IDirect3DVolume8 {
    IDirect3DVolume8Vtbl *lpVtbl;
};

// ============================================================================
// D3D8 GUIDs
// ============================================================================

DEFINE_GUID(IID_IDirect3D8, 0x1dd9e8da, 0x1c77, 0x4d40, 0xb0, 0xcf, 0x98, 0xfe, 0xfd, 0xff, 0x95, 0x12);
DEFINE_GUID(IID_IDirect3DDevice8, 0x7385e5df, 0x8fe8, 0x41d5, 0x86, 0xb6, 0xd7, 0xb4, 0x85, 0x47, 0xb6, 0xcf);
DEFINE_GUID(IID_IDirect3DResource8, 0x1b36bb7b, 0x9b7, 0x410a, 0xb4, 0x45, 0x7d, 0x14, 0x30, 0xd7, 0xb3, 0x3f);
DEFINE_GUID(IID_IDirect3DBaseTexture8, 0xb4211cfa, 0x51b9, 0x4a9f, 0xab, 0x78, 0xdb, 0x99, 0xb2, 0xbb, 0x67, 0x8e);
DEFINE_GUID(IID_IDirect3DTexture8, 0xe4cdd575, 0x2866, 0x4f01, 0xb1, 0x2e, 0x7e, 0xec, 0xe1, 0xec, 0x93, 0x58);
DEFINE_GUID(IID_IDirect3DCubeTexture8, 0x3ee5b968, 0x2aca, 0x4c34, 0x8b, 0xb5, 0x7e, 0x0c, 0x3d, 0x19, 0xb7, 0x50);
DEFINE_GUID(IID_IDirect3DVolumeTexture8, 0x4b8aaafa, 0x140f, 0x42ba, 0x91, 0x31, 0x59, 0x7e, 0xaf, 0xaa, 0x2e, 0xad);
DEFINE_GUID(IID_IDirect3DSurface8, 0xb96eebca, 0xb326, 0x4ea5, 0x88, 0x2f, 0x2f, 0xf5, 0xba, 0xe0, 0x21, 0xdd);
DEFINE_GUID(IID_IDirect3DVolume8, 0xbd7349f5, 0x14f1, 0x42e4, 0x9c, 0x79, 0x97, 0x23, 0x80, 0xdb, 0x40, 0xc0);
DEFINE_GUID(IID_IDirect3DSwapChain8, 0x928c088b, 0x76b9, 0x4c6b, 0xa5, 0x36, 0xa5, 0x90, 0x85, 0x38, 0x76, 0xcd);
DEFINE_GUID(IID_IDirect3DVertexBuffer8, 0x8aeeeac7, 0x05f9, 0x44d4, 0xb5, 0x91, 0x00, 0x0b, 0x0d, 0xf1, 0xcb, 0x95);
DEFINE_GUID(IID_IDirect3DIndexBuffer8, 0x0e689c9a, 0x053d, 0x44a0, 0x9d, 0x92, 0xdb, 0x0e, 0x3d, 0x75, 0x0f, 0x86);

// ============================================================================
// Exported Function
// ============================================================================

#ifdef __cplusplus
extern "C" {
#endif

IDirect3D8 * WINAPI Direct3DCreate8(UINT SDKVersion);

#ifdef __cplusplus
}
#endif

#endif // D3D8_H
