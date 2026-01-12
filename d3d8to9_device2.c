/**
 * D3D8 to D3D9 Converter - IDirect3DDevice8 Implementation (Part 2)
 */

#include "d3d8to9_base.h"

// External VTable declarations
extern IDirect3DDevice8Vtbl g_Direct3DDevice8_Vtbl;

// Forward declarations for resource wrappers
HRESULT CreateDirect3DSurface8(Direct3DDevice8 *pDevice8, IDirect3DSurface9 *pSurface9, Direct3DSurface8 **ppSurface8);
HRESULT CreateDirect3DTexture8(Direct3DDevice8 *pDevice8, IDirect3DTexture9 *pTexture9, Direct3DTexture8 **ppTexture8);
HRESULT CreateDirect3DVertexBuffer8(Direct3DDevice8 *pDevice8, IDirect3DVertexBuffer9 *pVB9, Direct3DVertexBuffer8 **ppVB8);
HRESULT CreateDirect3DIndexBuffer8(Direct3DDevice8 *pDevice8, IDirect3DIndexBuffer9 *pIB9, Direct3DIndexBuffer8 **ppIB8);

// Forward declarations for device functions from d3d8to9_device.c
HRESULT WINAPI Direct3DDevice8_QueryInterface(IDirect3DDevice8 *This, REFIID riid, void **ppvObject);
ULONG WINAPI Direct3DDevice8_AddRef(IDirect3DDevice8 *This);
ULONG WINAPI Direct3DDevice8_Release(IDirect3DDevice8 *This);
HRESULT WINAPI Direct3DDevice8_TestCooperativeLevel(IDirect3DDevice8 *This);
UINT WINAPI Direct3DDevice8_GetAvailableTextureMem(IDirect3DDevice8 *This);
HRESULT WINAPI Direct3DDevice8_ResourceManagerDiscardBytes(IDirect3DDevice8 *This, DWORD Bytes);
HRESULT WINAPI Direct3DDevice8_GetDirect3D(IDirect3DDevice8 *This, IDirect3D8 **ppD3D8);
HRESULT WINAPI Direct3DDevice8_GetDeviceCaps(IDirect3DDevice8 *This, D3DCAPS8 *pCaps);
HRESULT WINAPI Direct3DDevice8_GetDisplayMode(IDirect3DDevice8 *This, D3DDISPLAYMODE *pMode);
HRESULT WINAPI Direct3DDevice8_GetCreationParameters(IDirect3DDevice8 *This, void *pParameters);
HRESULT WINAPI Direct3DDevice8_SetCursorProperties(IDirect3DDevice8 *This, UINT XHotSpot, UINT YHotSpot, IDirect3DSurface8 *pCursorBitmap);
void WINAPI Direct3DDevice8_SetCursorPosition(IDirect3DDevice8 *This, int X, int Y, DWORD Flags);
BOOL WINAPI Direct3DDevice8_ShowCursor(IDirect3DDevice8 *This, BOOL bShow);
HRESULT WINAPI Direct3DDevice8_CreateAdditionalSwapChain(IDirect3DDevice8 *This, D3DPRESENT_PARAMETERS8 *pPresentationParameters, IDirect3DSwapChain8 **ppSwapChain);
HRESULT WINAPI Direct3DDevice8_Reset(IDirect3DDevice8 *This, D3DPRESENT_PARAMETERS8 *pPresentationParameters);
HRESULT WINAPI Direct3DDevice8_Present(IDirect3DDevice8 *This, const RECT *pSourceRect, const RECT *pDestRect, HWND hDestWindowOverride, const RGNDATA *pDirtyRegion);
HRESULT WINAPI Direct3DDevice8_GetBackBuffer(IDirect3DDevice8 *This, UINT BackBuffer, D3DBACKBUFFER_TYPE Type, IDirect3DSurface8 **ppBackBuffer);
HRESULT WINAPI Direct3DDevice8_GetRasterStatus(IDirect3DDevice8 *This, D3DRASTER_STATUS *pRasterStatus);
void WINAPI Direct3DDevice8_SetGammaRamp(IDirect3DDevice8 *This, DWORD Flags, const D3DGAMMARAMP *pRamp);
void WINAPI Direct3DDevice8_GetGammaRamp(IDirect3DDevice8 *This, D3DGAMMARAMP *pRamp);
HRESULT WINAPI Direct3DDevice8_CreateTexture(IDirect3DDevice8 *This, UINT Width, UINT Height, UINT Levels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DTexture8 **ppTexture);
HRESULT WINAPI Direct3DDevice8_CreateVolumeTexture(IDirect3DDevice8 *This, UINT Width, UINT Height, UINT Depth, UINT Levels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DVolumeTexture8 **ppVolumeTexture);
HRESULT WINAPI Direct3DDevice8_CreateCubeTexture(IDirect3DDevice8 *This, UINT EdgeLength, UINT Levels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DCubeTexture8 **ppCubeTexture);
HRESULT WINAPI Direct3DDevice8_CreateVertexBuffer(IDirect3DDevice8 *This, UINT Length, DWORD Usage, DWORD FVF, D3DPOOL Pool, IDirect3DVertexBuffer8 **ppVertexBuffer);
HRESULT WINAPI Direct3DDevice8_CreateIndexBuffer(IDirect3DDevice8 *This, UINT Length, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DIndexBuffer8 **ppIndexBuffer);
HRESULT WINAPI Direct3DDevice8_CreateRenderTarget(IDirect3DDevice8 *This, UINT Width, UINT Height, D3DFORMAT Format, D3DMULTISAMPLE_TYPE MultiSample, BOOL Lockable, IDirect3DSurface8 **ppSurface);
HRESULT WINAPI Direct3DDevice8_CreateDepthStencilSurface(IDirect3DDevice8 *This, UINT Width, UINT Height, D3DFORMAT Format, D3DMULTISAMPLE_TYPE MultiSample, IDirect3DSurface8 **ppSurface);
HRESULT WINAPI Direct3DDevice8_CreateImageSurface(IDirect3DDevice8 *This, UINT Width, UINT Height, D3DFORMAT Format, IDirect3DSurface8 **ppSurface);
HRESULT WINAPI Direct3DDevice8_CopyRects(IDirect3DDevice8 *This, IDirect3DSurface8 *pSourceSurface, const RECT *pSourceRectsArray, UINT cRects, IDirect3DSurface8 *pDestinationSurface, const POINT *pDestPointsArray);
HRESULT WINAPI Direct3DDevice8_UpdateTexture(IDirect3DDevice8 *This, IDirect3DBaseTexture8 *pSourceTexture, IDirect3DBaseTexture8 *pDestinationTexture);
HRESULT WINAPI Direct3DDevice8_GetFrontBuffer(IDirect3DDevice8 *This, IDirect3DSurface8 *pDestSurface);
HRESULT WINAPI Direct3DDevice8_SetRenderTarget(IDirect3DDevice8 *This, IDirect3DSurface8 *pRenderTarget, IDirect3DSurface8 *pNewZStencil);
HRESULT WINAPI Direct3DDevice8_GetRenderTarget(IDirect3DDevice8 *This, IDirect3DSurface8 **ppRenderTarget);
HRESULT WINAPI Direct3DDevice8_GetDepthStencilSurface(IDirect3DDevice8 *This, IDirect3DSurface8 **ppZStencilSurface);
HRESULT WINAPI Direct3DDevice8_BeginScene(IDirect3DDevice8 *This);
HRESULT WINAPI Direct3DDevice8_EndScene(IDirect3DDevice8 *This);
HRESULT WINAPI Direct3DDevice8_Clear(IDirect3DDevice8 *This, DWORD Count, const D3DRECT *pRects, DWORD Flags, D3DCOLOR Color, float Z, DWORD Stencil);
HRESULT WINAPI Direct3DDevice8_SetTransform(IDirect3DDevice8 *This, D3DTRANSFORMSTATETYPE State, const D3DMATRIX *pMatrix);
HRESULT WINAPI Direct3DDevice8_GetTransform(IDirect3DDevice8 *This, D3DTRANSFORMSTATETYPE State, D3DMATRIX *pMatrix);
HRESULT WINAPI Direct3DDevice8_MultiplyTransform(IDirect3DDevice8 *This, D3DTRANSFORMSTATETYPE State, const D3DMATRIX *pMatrix);
HRESULT WINAPI Direct3DDevice8_SetViewport(IDirect3DDevice8 *This, const D3DVIEWPORT8 *pViewport);
HRESULT WINAPI Direct3DDevice8_GetViewport(IDirect3DDevice8 *This, D3DVIEWPORT8 *pViewport);
HRESULT WINAPI Direct3DDevice8_SetMaterial(IDirect3DDevice8 *This, const D3DMATERIAL8 *pMaterial);
HRESULT WINAPI Direct3DDevice8_GetMaterial(IDirect3DDevice8 *This, D3DMATERIAL8 *pMaterial);
HRESULT WINAPI Direct3DDevice8_SetLight(IDirect3DDevice8 *This, DWORD Index, const D3DLIGHT8 *pLight);
HRESULT WINAPI Direct3DDevice8_GetLight(IDirect3DDevice8 *This, DWORD Index, D3DLIGHT8 *pLight);
HRESULT WINAPI Direct3DDevice8_LightEnable(IDirect3DDevice8 *This, DWORD Index, BOOL Enable);
HRESULT WINAPI Direct3DDevice8_GetLightEnable(IDirect3DDevice8 *This, DWORD Index, BOOL *pEnable);
HRESULT WINAPI Direct3DDevice8_SetClipPlane(IDirect3DDevice8 *This, DWORD Index, const float *pPlane);
HRESULT WINAPI Direct3DDevice8_GetClipPlane(IDirect3DDevice8 *This, DWORD Index, float *pPlane);
HRESULT WINAPI Direct3DDevice8_SetRenderState(IDirect3DDevice8 *This, D3DRENDERSTATETYPE State, DWORD Value);
HRESULT WINAPI Direct3DDevice8_GetRenderState(IDirect3DDevice8 *This, D3DRENDERSTATETYPE State, DWORD *pValue);

// State block stubs
HRESULT WINAPI Direct3DDevice8_BeginStateBlock(IDirect3DDevice8 *This)
{
    Direct3DDevice8 *self = (Direct3DDevice8 *)This;
    return IDirect3DDevice9_BeginStateBlock(self->pDevice9);
}

HRESULT WINAPI Direct3DDevice8_EndStateBlock(IDirect3DDevice8 *This, DWORD *pToken)
{
    Direct3DDevice8 *self = (Direct3DDevice8 *)This;
    IDirect3DStateBlock9 *pStateBlock = NULL;
    HRESULT hr = IDirect3DDevice9_EndStateBlock(self->pDevice9, &pStateBlock);
    if (SUCCEEDED(hr) && pToken) {
        *pToken = (DWORD)(ULONG_PTR)pStateBlock;
    }
    return hr;
}

HRESULT WINAPI Direct3DDevice8_ApplyStateBlock(IDirect3DDevice8 *This, DWORD Token)
{
    IDirect3DStateBlock9 *pStateBlock = (IDirect3DStateBlock9 *)(ULONG_PTR)Token;
    if (!pStateBlock) return D3DERR_INVALIDCALL;
    return IDirect3DStateBlock9_Apply(pStateBlock);
}

HRESULT WINAPI Direct3DDevice8_CaptureStateBlock(IDirect3DDevice8 *This, DWORD Token)
{
    IDirect3DStateBlock9 *pStateBlock = (IDirect3DStateBlock9 *)(ULONG_PTR)Token;
    if (!pStateBlock) return D3DERR_INVALIDCALL;
    return IDirect3DStateBlock9_Capture(pStateBlock);
}

HRESULT WINAPI Direct3DDevice8_DeleteStateBlock(IDirect3DDevice8 *This, DWORD Token)
{
    (void)This;
    IDirect3DStateBlock9 *pStateBlock = (IDirect3DStateBlock9 *)(ULONG_PTR)Token;
    if (!pStateBlock) return D3DERR_INVALIDCALL;
    IDirect3DStateBlock9_Release(pStateBlock);
    return D3D_OK;
}

HRESULT WINAPI Direct3DDevice8_CreateStateBlock(IDirect3DDevice8 *This, D3DSTATEBLOCKTYPE Type, DWORD *pToken)
{
    Direct3DDevice8 *self = (Direct3DDevice8 *)This;
    IDirect3DStateBlock9 *pStateBlock = NULL;
    HRESULT hr = IDirect3DDevice9_CreateStateBlock(self->pDevice9, Type, &pStateBlock);
    if (SUCCEEDED(hr) && pToken) {
        *pToken = (DWORD)(ULONG_PTR)pStateBlock;
    }
    return hr;
}

HRESULT WINAPI Direct3DDevice8_SetClipStatus(IDirect3DDevice8 *This, const D3DCLIPSTATUS8 *pClipStatus)
{
    Direct3DDevice8 *self = (Direct3DDevice8 *)This;
    return IDirect3DDevice9_SetClipStatus(self->pDevice9, (const D3DCLIPSTATUS9 *)pClipStatus);
}

HRESULT WINAPI Direct3DDevice8_GetClipStatus(IDirect3DDevice8 *This, D3DCLIPSTATUS8 *pClipStatus)
{
    Direct3DDevice8 *self = (Direct3DDevice8 *)This;
    return IDirect3DDevice9_GetClipStatus(self->pDevice9, (D3DCLIPSTATUS9 *)pClipStatus);
}

HRESULT WINAPI Direct3DDevice8_GetTexture(IDirect3DDevice8 *This, DWORD Stage, IDirect3DBaseTexture8 **ppTexture)
{
    Direct3DDevice8 *self = (Direct3DDevice8 *)This;
    if (!ppTexture) return D3DERR_INVALIDCALL;

    IDirect3DBaseTexture9 *pTexture9 = NULL;
    HRESULT hr = IDirect3DDevice9_GetTexture(self->pDevice9, Stage, &pTexture9);

    if (SUCCEEDED(hr) && pTexture9) {
        // For simplicity, wrap as texture (most common case)
        hr = CreateDirect3DTexture8(self, (IDirect3DTexture9 *)pTexture9, (Direct3DTexture8 **)ppTexture);
        if (FAILED(hr)) {
            IDirect3DBaseTexture9_Release(pTexture9);
        }
    } else {
        *ppTexture = NULL;
    }
    return hr;
}

HRESULT WINAPI Direct3DDevice8_SetTexture(IDirect3DDevice8 *This, DWORD Stage, IDirect3DBaseTexture8 *pTexture)
{
    Direct3DDevice8 *self = (Direct3DDevice8 *)This;
    Direct3DTexture8 *pTex8 = (Direct3DTexture8 *)pTexture;
    return IDirect3DDevice9_SetTexture(self->pDevice9, Stage, pTex8 ? (IDirect3DBaseTexture9 *)pTex8->pTexture9 : NULL);
}

HRESULT WINAPI Direct3DDevice8_GetTextureStageState(IDirect3DDevice8 *This, DWORD Stage, D3DTEXTURESTAGESTATETYPE Type, DWORD *pValue)
{
    Direct3DDevice8 *self = (Direct3DDevice8 *)This;

    // D3D8 texture stage states that map to D3D9 sampler states
    switch (Type) {
        case D3DTSS8_ADDRESSU:
            return IDirect3DDevice9_GetSamplerState(self->pDevice9, Stage, D3DSAMP_ADDRESSU, pValue);
        case D3DTSS8_ADDRESSV:
            return IDirect3DDevice9_GetSamplerState(self->pDevice9, Stage, D3DSAMP_ADDRESSV, pValue);
        case D3DTSS8_ADDRESSW:
            return IDirect3DDevice9_GetSamplerState(self->pDevice9, Stage, D3DSAMP_ADDRESSW, pValue);
        case D3DTSS8_BORDERCOLOR:
            return IDirect3DDevice9_GetSamplerState(self->pDevice9, Stage, D3DSAMP_BORDERCOLOR, pValue);
        case D3DTSS8_MAGFILTER:
            return IDirect3DDevice9_GetSamplerState(self->pDevice9, Stage, D3DSAMP_MAGFILTER, pValue);
        case D3DTSS8_MINFILTER:
            return IDirect3DDevice9_GetSamplerState(self->pDevice9, Stage, D3DSAMP_MINFILTER, pValue);
        case D3DTSS8_MIPFILTER:
            return IDirect3DDevice9_GetSamplerState(self->pDevice9, Stage, D3DSAMP_MIPFILTER, pValue);
        case D3DTSS8_MIPMAPLODBIAS:
            return IDirect3DDevice9_GetSamplerState(self->pDevice9, Stage, D3DSAMP_MIPMAPLODBIAS, pValue);
        case D3DTSS8_MAXMIPLEVEL:
            return IDirect3DDevice9_GetSamplerState(self->pDevice9, Stage, D3DSAMP_MAXMIPLEVEL, pValue);
        case D3DTSS8_MAXANISOTROPY:
            return IDirect3DDevice9_GetSamplerState(self->pDevice9, Stage, D3DSAMP_MAXANISOTROPY, pValue);
        default:
            return IDirect3DDevice9_GetTextureStageState(self->pDevice9, Stage, Type, pValue);
    }
}

HRESULT WINAPI Direct3DDevice8_SetTextureStageState(IDirect3DDevice8 *This, DWORD Stage, D3DTEXTURESTAGESTATETYPE Type, DWORD Value)
{
    Direct3DDevice8 *self = (Direct3DDevice8 *)This;

    switch (Type) {
        case D3DTSS8_ADDRESSU:
            return IDirect3DDevice9_SetSamplerState(self->pDevice9, Stage, D3DSAMP_ADDRESSU, Value);
        case D3DTSS8_ADDRESSV:
            return IDirect3DDevice9_SetSamplerState(self->pDevice9, Stage, D3DSAMP_ADDRESSV, Value);
        case D3DTSS8_ADDRESSW:
            return IDirect3DDevice9_SetSamplerState(self->pDevice9, Stage, D3DSAMP_ADDRESSW, Value);
        case D3DTSS8_BORDERCOLOR:
            return IDirect3DDevice9_SetSamplerState(self->pDevice9, Stage, D3DSAMP_BORDERCOLOR, Value);
        case D3DTSS8_MAGFILTER:
            return IDirect3DDevice9_SetSamplerState(self->pDevice9, Stage, D3DSAMP_MAGFILTER, Value);
        case D3DTSS8_MINFILTER:
            return IDirect3DDevice9_SetSamplerState(self->pDevice9, Stage, D3DSAMP_MINFILTER, Value);
        case D3DTSS8_MIPFILTER:
            return IDirect3DDevice9_SetSamplerState(self->pDevice9, Stage, D3DSAMP_MIPFILTER, Value);
        case D3DTSS8_MIPMAPLODBIAS:
            return IDirect3DDevice9_SetSamplerState(self->pDevice9, Stage, D3DSAMP_MIPMAPLODBIAS, Value);
        case D3DTSS8_MAXMIPLEVEL:
            return IDirect3DDevice9_SetSamplerState(self->pDevice9, Stage, D3DSAMP_MAXMIPLEVEL, Value);
        case D3DTSS8_MAXANISOTROPY:
            return IDirect3DDevice9_SetSamplerState(self->pDevice9, Stage, D3DSAMP_MAXANISOTROPY, Value);
        default:
            return IDirect3DDevice9_SetTextureStageState(self->pDevice9, Stage, Type, Value);
    }
}

HRESULT WINAPI Direct3DDevice8_ValidateDevice(IDirect3DDevice8 *This, DWORD *pNumPasses)
{
    Direct3DDevice8 *self = (Direct3DDevice8 *)This;
    return IDirect3DDevice9_ValidateDevice(self->pDevice9, pNumPasses);
}

HRESULT WINAPI Direct3DDevice8_GetInfo(IDirect3DDevice8 *This, DWORD DevInfoID, void *pDevInfoStruct, DWORD DevInfoStructSize)
{
    (void)This; (void)DevInfoID; (void)pDevInfoStruct; (void)DevInfoStructSize;
    // D3D9 doesn't have GetInfo
    return E_NOTIMPL;
}

HRESULT WINAPI Direct3DDevice8_SetPaletteEntries(IDirect3DDevice8 *This, UINT PaletteNumber, const PALETTEENTRY *pEntries)
{
    Direct3DDevice8 *self = (Direct3DDevice8 *)This;
    return IDirect3DDevice9_SetPaletteEntries(self->pDevice9, PaletteNumber, pEntries);
}

HRESULT WINAPI Direct3DDevice8_GetPaletteEntries(IDirect3DDevice8 *This, UINT PaletteNumber, PALETTEENTRY *pEntries)
{
    Direct3DDevice8 *self = (Direct3DDevice8 *)This;
    return IDirect3DDevice9_GetPaletteEntries(self->pDevice9, PaletteNumber, pEntries);
}

HRESULT WINAPI Direct3DDevice8_SetCurrentTexturePalette(IDirect3DDevice8 *This, UINT PaletteNumber)
{
    Direct3DDevice8 *self = (Direct3DDevice8 *)This;
    return IDirect3DDevice9_SetCurrentTexturePalette(self->pDevice9, PaletteNumber);
}

HRESULT WINAPI Direct3DDevice8_GetCurrentTexturePalette(IDirect3DDevice8 *This, UINT *pPaletteNumber)
{
    Direct3DDevice8 *self = (Direct3DDevice8 *)This;
    return IDirect3DDevice9_GetCurrentTexturePalette(self->pDevice9, pPaletteNumber);
}

HRESULT WINAPI Direct3DDevice8_DrawPrimitive(IDirect3DDevice8 *This, D3DPRIMITIVETYPE PrimitiveType, UINT StartVertex, UINT PrimitiveCount)
{
    Direct3DDevice8 *self = (Direct3DDevice8 *)This;
    return IDirect3DDevice9_DrawPrimitive(self->pDevice9, PrimitiveType, StartVertex, PrimitiveCount);
}

HRESULT WINAPI Direct3DDevice8_DrawIndexedPrimitive(IDirect3DDevice8 *This, D3DPRIMITIVETYPE PrimitiveType, UINT MinVertexIndex, UINT NumVertices, UINT StartIndex, UINT PrimitiveCount)
{
    Direct3DDevice8 *self = (Direct3DDevice8 *)This;
    return IDirect3DDevice9_DrawIndexedPrimitive(self->pDevice9, PrimitiveType, self->BaseVertexIndex, MinVertexIndex, NumVertices, StartIndex, PrimitiveCount);
}

HRESULT WINAPI Direct3DDevice8_DrawPrimitiveUP(IDirect3DDevice8 *This, D3DPRIMITIVETYPE PrimitiveType, UINT PrimitiveCount, const void *pVertexStreamZeroData, UINT VertexStreamZeroStride)
{
    Direct3DDevice8 *self = (Direct3DDevice8 *)This;
    return IDirect3DDevice9_DrawPrimitiveUP(self->pDevice9, PrimitiveType, PrimitiveCount, pVertexStreamZeroData, VertexStreamZeroStride);
}

HRESULT WINAPI Direct3DDevice8_DrawIndexedPrimitiveUP(IDirect3DDevice8 *This, D3DPRIMITIVETYPE PrimitiveType, UINT MinVertexIndex, UINT NumVertices, UINT PrimitiveCount, const void *pIndexData, D3DFORMAT IndexDataFormat, const void *pVertexStreamZeroData, UINT VertexStreamZeroStride)
{
    Direct3DDevice8 *self = (Direct3DDevice8 *)This;
    return IDirect3DDevice9_DrawIndexedPrimitiveUP(self->pDevice9, PrimitiveType, MinVertexIndex, NumVertices, PrimitiveCount, pIndexData, IndexDataFormat, pVertexStreamZeroData, VertexStreamZeroStride);
}

HRESULT WINAPI Direct3DDevice8_ProcessVertices(IDirect3DDevice8 *This, UINT SrcStartIndex, UINT DestIndex, UINT VertexCount, IDirect3DVertexBuffer8 *pDestBuffer, DWORD Flags)
{
    Direct3DDevice8 *self = (Direct3DDevice8 *)This;
    Direct3DVertexBuffer8 *pVB8 = (Direct3DVertexBuffer8 *)pDestBuffer;
    if (!pVB8) return D3DERR_INVALIDCALL;
    return IDirect3DDevice9_ProcessVertices(self->pDevice9, SrcStartIndex, DestIndex, VertexCount, pVB8->pVB9, NULL, Flags);
}

// Shader management helpers
static DWORD AddPixelShaderHandle(Direct3DDevice8 *self, IDirect3DPixelShader9 *pShader)
{
    if (self->PixelShaderCount >= self->PixelShaderCapacity) {
        DWORD newCapacity = self->PixelShaderCapacity ? self->PixelShaderCapacity * 2 : 16;
        void **newHandles = HeapReAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, self->PixelShaderHandles, newCapacity * sizeof(void *));
        if (!newHandles) {
            newHandles = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, newCapacity * sizeof(void *));
            if (!newHandles) return 0;
            if (self->PixelShaderHandles) {
                memcpy(newHandles, self->PixelShaderHandles, self->PixelShaderCount * sizeof(void *));
                HeapFree(GetProcessHeap(), 0, self->PixelShaderHandles);
            }
        }
        self->PixelShaderHandles = newHandles;
        self->PixelShaderCapacity = newCapacity;
    }
    DWORD handle = self->PixelShaderCount + 1;
    self->PixelShaderHandles[self->PixelShaderCount++] = pShader;
    return handle;
}

// Convert D3D8 vertex declaration to D3D9 vertex declaration
static HRESULT ConvertVertexDeclaration8to9(IDirect3DDevice9 *pDevice9, const DWORD *pDeclaration, IDirect3DVertexDeclaration9 **ppDecl9)
{
    if (!pDeclaration || !ppDecl9) return D3DERR_INVALIDCALL;

    D3DVERTEXELEMENT9_DEF elements[32];
    int elementCount = 0;
    WORD currentStream = 0;
    WORD currentOffset = 0;

    const DWORD *pToken = pDeclaration;
    while (*pToken != 0xFFFFFFFF && elementCount < 31) {
        DWORD token = *pToken++;
        DWORD tokenType = (token & D3DVSD_TOKENTYPEMASK) >> D3DVSD_TOKENTYPESHIFT;

        switch (tokenType) {
            case D3DVSD_TOKEN_NOP:
                // Skip NOP tokens
                break;

            case D3DVSD_TOKEN_STREAM:
                // Stream selector
                currentStream = (WORD)((token & D3DVSD_STREAMNUMBERMASK) >> D3DVSD_STREAMNUMBERSHIFT);
                currentOffset = 0;
                break;

            case D3DVSD_TOKEN_STREAMDATA:
                {
                    DWORD loadType = (token & D3DVSD_DATALOADTYPEMASK) >> D3DVSD_DATALOADTYPESHIFT;
                    if (loadType == 0) {
                        // Vertex data
                        DWORD vertexReg = (token & D3DVSD_VERTEXREGMASK) >> D3DVSD_VERTEXREGSHIFT;
                        DWORD dataType = (token & D3DVSD_DATATYPEMASK) >> D3DVSD_DATATYPESHIFT;

                        elements[elementCount].Stream = currentStream;
                        elements[elementCount].Offset = currentOffset;
                        elements[elementCount].Type = ConvertD3D8TypeToD3D9(dataType);
                        elements[elementCount].Method = D3DDECLMETHOD_DEFAULT;
                        ConvertD3D8RegisterToD3D9Usage(vertexReg, &elements[elementCount].Usage, &elements[elementCount].UsageIndex);

                        currentOffset += GetD3D8DataTypeSize(dataType);
                        elementCount++;
                    } else {
                        // Skip data (SKIP token)
                        DWORD skipCount = (token & D3DVSD_SKIPCOUNTMASK) >> D3DVSD_SKIPCOUNTSHIFT;
                        currentOffset += (WORD)(skipCount * 4);
                    }
                }
                break;

            case D3DVSD_TOKEN_TESSELLATOR:
                // Tessellator data - skip for now
                break;

            case D3DVSD_TOKEN_CONSTMEM:
                // Constant memory load - skip the constant data
                {
                    DWORD count = ((token >> 25) & 0x7F);
                    pToken += count * 4; // Skip constant values
                }
                break;

            case D3DVSD_TOKEN_EXT:
                // Extension token - skip
                {
                    DWORD count = ((token >> 24) & 0x1F);
                    pToken += count;
                }
                break;

            case D3DVSD_TOKEN_END:
                goto done;

            default:
                break;
        }
    }

done:
    // Add end element
    elements[elementCount].Stream = 0xFF;
    elements[elementCount].Offset = 0;
    elements[elementCount].Type = D3DDECLTYPE_UNUSED;
    elements[elementCount].Method = 0;
    elements[elementCount].Usage = 0;
    elements[elementCount].UsageIndex = 0;

    // Create D3D9 vertex declaration
    return IDirect3DDevice9_CreateVertexDeclaration(pDevice9, (const D3DVERTEXELEMENT9 *)elements, ppDecl9);
}

// Add vertex shader with info
static DWORD AddVertexShaderInfo(Direct3DDevice8 *self, VertexShaderInfo *pInfo)
{
    if (self->VertexShaderCount >= self->VertexShaderCapacity) {
        DWORD newCapacity = self->VertexShaderCapacity ? self->VertexShaderCapacity * 2 : 16;
        VertexShaderInfo **newHandles = (VertexShaderInfo **)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, newCapacity * sizeof(VertexShaderInfo *));
        if (!newHandles) return 0;
        if (self->VertexShaderHandles) {
            memcpy(newHandles, self->VertexShaderHandles, self->VertexShaderCount * sizeof(VertexShaderInfo *));
            HeapFree(GetProcessHeap(), 0, self->VertexShaderHandles);
        }
        self->VertexShaderHandles = newHandles;
        self->VertexShaderCapacity = newCapacity;
    }
    DWORD handle = self->VertexShaderCount + 1;
    self->VertexShaderHandles[self->VertexShaderCount++] = pInfo;
    return handle;
}

HRESULT WINAPI Direct3DDevice8_CreateVertexShader(IDirect3DDevice8 *This, const DWORD *pDeclaration, const DWORD *pFunction, DWORD *pHandle, DWORD Usage)
{
    Direct3DDevice8 *self = (Direct3DDevice8 *)This;
    (void)Usage;

    if (!pHandle) return D3DERR_INVALIDCALL;
    if (!pDeclaration) return D3DERR_INVALIDCALL;

    // Allocate shader info structure
    VertexShaderInfo *pInfo = (VertexShaderInfo *)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(VertexShaderInfo));
    if (!pInfo) return E_OUTOFMEMORY;

    // Convert D3D8 vertex declaration to D3D9
    HRESULT hr = ConvertVertexDeclaration8to9(self->pDevice9, pDeclaration, &pInfo->pDecl9);
    if (FAILED(hr)) {
        HeapFree(GetProcessHeap(), 0, pInfo);
        return hr;
    }

    // Create vertex shader if function is provided
    if (pFunction) {
        hr = IDirect3DDevice9_CreateVertexShader(self->pDevice9, pFunction, &pInfo->pShader9);
        if (FAILED(hr)) {
            if (pInfo->pDecl9) IDirect3DVertexDeclaration9_Release(pInfo->pDecl9);
            HeapFree(GetProcessHeap(), 0, pInfo);
            return hr;
        }
    }

    // Add to handle list
    *pHandle = AddVertexShaderInfo(self, pInfo);
    if (*pHandle == 0) {
        if (pInfo->pShader9) IDirect3DVertexShader9_Release(pInfo->pShader9);
        if (pInfo->pDecl9) IDirect3DVertexDeclaration9_Release(pInfo->pDecl9);
        HeapFree(GetProcessHeap(), 0, pInfo);
        return E_OUTOFMEMORY;
    }

    return D3D_OK;
}

HRESULT WINAPI Direct3DDevice8_SetVertexShader(IDirect3DDevice8 *This, DWORD Handle)
{
    Direct3DDevice8 *self = (Direct3DDevice8 *)This;

    self->CurrentVertexShaderHandle = Handle;

    // Handle == 0 means no shader/FVF
    if (Handle == 0) {
        IDirect3DDevice9_SetVertexShader(self->pDevice9, NULL);
        IDirect3DDevice9_SetVertexDeclaration(self->pDevice9, NULL);
        return D3D_OK;
    }

    // Check for FVF (high bit set)
    if (Handle & 0x80000000) {
        IDirect3DDevice9_SetVertexShader(self->pDevice9, NULL);
        IDirect3DDevice9_SetVertexDeclaration(self->pDevice9, NULL);
        return IDirect3DDevice9_SetFVF(self->pDevice9, Handle & ~0x80000000);
    }

    // Shader handle
    if (Handle > self->VertexShaderCount) return D3DERR_INVALIDCALL;
    VertexShaderInfo *pInfo = self->VertexShaderHandles[Handle - 1];
    if (!pInfo) return D3DERR_INVALIDCALL;

    // Set vertex declaration first
    if (pInfo->pDecl9) {
        IDirect3DDevice9_SetVertexDeclaration(self->pDevice9, pInfo->pDecl9);
    }

    // Set vertex shader (may be NULL if declaration-only)
    return IDirect3DDevice9_SetVertexShader(self->pDevice9, pInfo->pShader9);
}

HRESULT WINAPI Direct3DDevice8_GetVertexShader(IDirect3DDevice8 *This, DWORD *pHandle)
{
    Direct3DDevice8 *self = (Direct3DDevice8 *)This;
    if (!pHandle) return D3DERR_INVALIDCALL;
    *pHandle = self->CurrentVertexShaderHandle;
    return D3D_OK;
}

HRESULT WINAPI Direct3DDevice8_DeleteVertexShader(IDirect3DDevice8 *This, DWORD Handle)
{
    Direct3DDevice8 *self = (Direct3DDevice8 *)This;
    if (Handle == 0 || Handle > self->VertexShaderCount) return D3DERR_INVALIDCALL;

    VertexShaderInfo *pInfo = self->VertexShaderHandles[Handle - 1];
    if (pInfo) {
        if (pInfo->pShader9) IDirect3DVertexShader9_Release(pInfo->pShader9);
        if (pInfo->pDecl9) IDirect3DVertexDeclaration9_Release(pInfo->pDecl9);
        HeapFree(GetProcessHeap(), 0, pInfo);
        self->VertexShaderHandles[Handle - 1] = NULL;
    }
    return D3D_OK;
}

HRESULT WINAPI Direct3DDevice8_SetVertexShaderConstant(IDirect3DDevice8 *This, DWORD Register, const void *pConstantData, DWORD ConstantCount)
{
    Direct3DDevice8 *self = (Direct3DDevice8 *)This;
    return IDirect3DDevice9_SetVertexShaderConstantF(self->pDevice9, Register, (const float *)pConstantData, ConstantCount);
}

HRESULT WINAPI Direct3DDevice8_GetVertexShaderConstant(IDirect3DDevice8 *This, DWORD Register, void *pConstantData, DWORD ConstantCount)
{
    Direct3DDevice8 *self = (Direct3DDevice8 *)This;
    return IDirect3DDevice9_GetVertexShaderConstantF(self->pDevice9, Register, (float *)pConstantData, ConstantCount);
}

HRESULT WINAPI Direct3DDevice8_GetVertexShaderDeclaration(IDirect3DDevice8 *This, DWORD Handle, void *pData, DWORD *pSizeOfData)
{
    (void)This; (void)Handle; (void)pData; (void)pSizeOfData;
    // Not directly supported in D3D9
    return E_NOTIMPL;
}

HRESULT WINAPI Direct3DDevice8_GetVertexShaderFunction(IDirect3DDevice8 *This, DWORD Handle, void *pData, DWORD *pSizeOfData)
{
    Direct3DDevice8 *self = (Direct3DDevice8 *)This;
    if (Handle == 0 || Handle > self->VertexShaderCount) return D3DERR_INVALIDCALL;

    VertexShaderInfo *pInfo = self->VertexShaderHandles[Handle - 1];
    if (!pInfo || !pInfo->pShader9) return D3DERR_INVALIDCALL;

    return IDirect3DVertexShader9_GetFunction(pInfo->pShader9, pData, (UINT *)pSizeOfData);
}

HRESULT WINAPI Direct3DDevice8_SetStreamSource(IDirect3DDevice8 *This, UINT StreamNumber, IDirect3DVertexBuffer8 *pStreamData, UINT Stride)
{
    Direct3DDevice8 *self = (Direct3DDevice8 *)This;
    Direct3DVertexBuffer8 *pVB8 = (Direct3DVertexBuffer8 *)pStreamData;
    return IDirect3DDevice9_SetStreamSource(self->pDevice9, StreamNumber, pVB8 ? pVB8->pVB9 : NULL, 0, Stride);
}

HRESULT WINAPI Direct3DDevice8_GetStreamSource(IDirect3DDevice8 *This, UINT StreamNumber, IDirect3DVertexBuffer8 **ppStreamData, UINT *pStride)
{
    Direct3DDevice8 *self = (Direct3DDevice8 *)This;
    if (!ppStreamData) return D3DERR_INVALIDCALL;

    IDirect3DVertexBuffer9 *pVB9 = NULL;
    UINT offset;
    HRESULT hr = IDirect3DDevice9_GetStreamSource(self->pDevice9, StreamNumber, &pVB9, &offset, pStride);

    if (SUCCEEDED(hr) && pVB9) {
        hr = CreateDirect3DVertexBuffer8(self, pVB9, (Direct3DVertexBuffer8 **)ppStreamData);
        if (FAILED(hr)) {
            IDirect3DVertexBuffer9_Release(pVB9);
        }
    } else {
        *ppStreamData = NULL;
    }
    return hr;
}

HRESULT WINAPI Direct3DDevice8_SetIndices(IDirect3DDevice8 *This, IDirect3DIndexBuffer8 *pIndexData, UINT BaseVertexIndex)
{
    Direct3DDevice8 *self = (Direct3DDevice8 *)This;
    Direct3DIndexBuffer8 *pIB8 = (Direct3DIndexBuffer8 *)pIndexData;
    self->BaseVertexIndex = BaseVertexIndex;
    return IDirect3DDevice9_SetIndices(self->pDevice9, pIB8 ? pIB8->pIB9 : NULL);
}

HRESULT WINAPI Direct3DDevice8_GetIndices(IDirect3DDevice8 *This, IDirect3DIndexBuffer8 **ppIndexData, UINT *pBaseVertexIndex)
{
    Direct3DDevice8 *self = (Direct3DDevice8 *)This;
    if (!ppIndexData) return D3DERR_INVALIDCALL;

    if (pBaseVertexIndex) {
        *pBaseVertexIndex = self->BaseVertexIndex;
    }

    IDirect3DIndexBuffer9 *pIB9 = NULL;
    HRESULT hr = IDirect3DDevice9_GetIndices(self->pDevice9, &pIB9);

    if (SUCCEEDED(hr) && pIB9) {
        hr = CreateDirect3DIndexBuffer8(self, pIB9, (Direct3DIndexBuffer8 **)ppIndexData);
        if (FAILED(hr)) {
            IDirect3DIndexBuffer9_Release(pIB9);
        }
    } else {
        *ppIndexData = NULL;
    }
    return hr;
}

HRESULT WINAPI Direct3DDevice8_CreatePixelShader(IDirect3DDevice8 *This, const DWORD *pFunction, DWORD *pHandle)
{
    Direct3DDevice8 *self = (Direct3DDevice8 *)This;
    if (!pHandle || !pFunction) return D3DERR_INVALIDCALL;

    IDirect3DPixelShader9 *pShader9 = NULL;
    HRESULT hr = IDirect3DDevice9_CreatePixelShader(self->pDevice9, pFunction, &pShader9);

    if (SUCCEEDED(hr)) {
        *pHandle = AddPixelShaderHandle(self, pShader9);
        if (*pHandle == 0) {
            IDirect3DPixelShader9_Release(pShader9);
            return E_OUTOFMEMORY;
        }
    }
    return hr;
}

HRESULT WINAPI Direct3DDevice8_SetPixelShader(IDirect3DDevice8 *This, DWORD Handle)
{
    Direct3DDevice8 *self = (Direct3DDevice8 *)This;

    self->CurrentPixelShaderHandle = Handle;

    if (Handle == 0) {
        return IDirect3DDevice9_SetPixelShader(self->pDevice9, NULL);
    }

    if (Handle > self->PixelShaderCount) return D3DERR_INVALIDCALL;
    IDirect3DPixelShader9 *pShader = (IDirect3DPixelShader9 *)self->PixelShaderHandles[Handle - 1];
    return IDirect3DDevice9_SetPixelShader(self->pDevice9, pShader);
}

HRESULT WINAPI Direct3DDevice8_GetPixelShader(IDirect3DDevice8 *This, DWORD *pHandle)
{
    Direct3DDevice8 *self = (Direct3DDevice8 *)This;
    if (!pHandle) return D3DERR_INVALIDCALL;
    *pHandle = self->CurrentPixelShaderHandle;
    return D3D_OK;
}

HRESULT WINAPI Direct3DDevice8_DeletePixelShader(IDirect3DDevice8 *This, DWORD Handle)
{
    Direct3DDevice8 *self = (Direct3DDevice8 *)This;
    if (Handle == 0 || Handle > self->PixelShaderCount) return D3DERR_INVALIDCALL;

    IDirect3DPixelShader9 *pShader = (IDirect3DPixelShader9 *)self->PixelShaderHandles[Handle - 1];
    if (pShader) {
        IDirect3DPixelShader9_Release(pShader);
        self->PixelShaderHandles[Handle - 1] = NULL;
    }
    return D3D_OK;
}

HRESULT WINAPI Direct3DDevice8_SetPixelShaderConstant(IDirect3DDevice8 *This, DWORD Register, const void *pConstantData, DWORD ConstantCount)
{
    Direct3DDevice8 *self = (Direct3DDevice8 *)This;
    return IDirect3DDevice9_SetPixelShaderConstantF(self->pDevice9, Register, (const float *)pConstantData, ConstantCount);
}

HRESULT WINAPI Direct3DDevice8_GetPixelShaderConstant(IDirect3DDevice8 *This, DWORD Register, void *pConstantData, DWORD ConstantCount)
{
    Direct3DDevice8 *self = (Direct3DDevice8 *)This;
    return IDirect3DDevice9_GetPixelShaderConstantF(self->pDevice9, Register, (float *)pConstantData, ConstantCount);
}

HRESULT WINAPI Direct3DDevice8_GetPixelShaderFunction(IDirect3DDevice8 *This, DWORD Handle, void *pData, DWORD *pSizeOfData)
{
    Direct3DDevice8 *self = (Direct3DDevice8 *)This;
    if (Handle == 0 || Handle > self->PixelShaderCount) return D3DERR_INVALIDCALL;

    IDirect3DPixelShader9 *pShader = (IDirect3DPixelShader9 *)self->PixelShaderHandles[Handle - 1];
    if (!pShader) return D3DERR_INVALIDCALL;

    return IDirect3DPixelShader9_GetFunction(pShader, pData, (UINT *)pSizeOfData);
}

HRESULT WINAPI Direct3DDevice8_DrawRectPatch(IDirect3DDevice8 *This, UINT Handle, const float *pNumSegs, const void *pRectPatchInfo)
{
    Direct3DDevice8 *self = (Direct3DDevice8 *)This;
    return IDirect3DDevice9_DrawRectPatch(self->pDevice9, Handle, pNumSegs, (const D3DRECTPATCH_INFO *)pRectPatchInfo);
}

HRESULT WINAPI Direct3DDevice8_DrawTriPatch(IDirect3DDevice8 *This, UINT Handle, const float *pNumSegs, const void *pTriPatchInfo)
{
    Direct3DDevice8 *self = (Direct3DDevice8 *)This;
    return IDirect3DDevice9_DrawTriPatch(self->pDevice9, Handle, pNumSegs, (const D3DTRIPATCH_INFO *)pTriPatchInfo);
}

HRESULT WINAPI Direct3DDevice8_DeletePatch(IDirect3DDevice8 *This, UINT Handle)
{
    Direct3DDevice8 *self = (Direct3DDevice8 *)This;
    return IDirect3DDevice9_DeletePatch(self->pDevice9, Handle);
}

// IDirect3DDevice8 VTable - defined externally
IDirect3DDevice8Vtbl g_Direct3DDevice8_Vtbl = {
    Direct3DDevice8_QueryInterface,
    Direct3DDevice8_AddRef,
    Direct3DDevice8_Release,
    Direct3DDevice8_TestCooperativeLevel,
    Direct3DDevice8_GetAvailableTextureMem,
    Direct3DDevice8_ResourceManagerDiscardBytes,
    Direct3DDevice8_GetDirect3D,
    Direct3DDevice8_GetDeviceCaps,
    Direct3DDevice8_GetDisplayMode,
    Direct3DDevice8_GetCreationParameters,
    Direct3DDevice8_SetCursorProperties,
    Direct3DDevice8_SetCursorPosition,
    Direct3DDevice8_ShowCursor,
    Direct3DDevice8_CreateAdditionalSwapChain,
    Direct3DDevice8_Reset,
    Direct3DDevice8_Present,
    Direct3DDevice8_GetBackBuffer,
    Direct3DDevice8_GetRasterStatus,
    Direct3DDevice8_SetGammaRamp,
    Direct3DDevice8_GetGammaRamp,
    Direct3DDevice8_CreateTexture,
    Direct3DDevice8_CreateVolumeTexture,
    Direct3DDevice8_CreateCubeTexture,
    Direct3DDevice8_CreateVertexBuffer,
    Direct3DDevice8_CreateIndexBuffer,
    Direct3DDevice8_CreateRenderTarget,
    Direct3DDevice8_CreateDepthStencilSurface,
    Direct3DDevice8_CreateImageSurface,
    Direct3DDevice8_CopyRects,
    Direct3DDevice8_UpdateTexture,
    Direct3DDevice8_GetFrontBuffer,
    Direct3DDevice8_SetRenderTarget,
    Direct3DDevice8_GetRenderTarget,
    Direct3DDevice8_GetDepthStencilSurface,
    Direct3DDevice8_BeginScene,
    Direct3DDevice8_EndScene,
    Direct3DDevice8_Clear,
    Direct3DDevice8_SetTransform,
    Direct3DDevice8_GetTransform,
    Direct3DDevice8_MultiplyTransform,
    Direct3DDevice8_SetViewport,
    Direct3DDevice8_GetViewport,
    Direct3DDevice8_SetMaterial,
    Direct3DDevice8_GetMaterial,
    Direct3DDevice8_SetLight,
    Direct3DDevice8_GetLight,
    Direct3DDevice8_LightEnable,
    Direct3DDevice8_GetLightEnable,
    Direct3DDevice8_SetClipPlane,
    Direct3DDevice8_GetClipPlane,
    Direct3DDevice8_SetRenderState,
    Direct3DDevice8_GetRenderState,
    Direct3DDevice8_BeginStateBlock,
    Direct3DDevice8_EndStateBlock,
    Direct3DDevice8_ApplyStateBlock,
    Direct3DDevice8_CaptureStateBlock,
    Direct3DDevice8_DeleteStateBlock,
    Direct3DDevice8_CreateStateBlock,
    Direct3DDevice8_SetClipStatus,
    Direct3DDevice8_GetClipStatus,
    Direct3DDevice8_GetTexture,
    Direct3DDevice8_SetTexture,
    Direct3DDevice8_GetTextureStageState,
    Direct3DDevice8_SetTextureStageState,
    Direct3DDevice8_ValidateDevice,
    Direct3DDevice8_GetInfo,
    Direct3DDevice8_SetPaletteEntries,
    Direct3DDevice8_GetPaletteEntries,
    Direct3DDevice8_SetCurrentTexturePalette,
    Direct3DDevice8_GetCurrentTexturePalette,
    Direct3DDevice8_DrawPrimitive,
    Direct3DDevice8_DrawIndexedPrimitive,
    Direct3DDevice8_DrawPrimitiveUP,
    Direct3DDevice8_DrawIndexedPrimitiveUP,
    Direct3DDevice8_ProcessVertices,
    Direct3DDevice8_CreateVertexShader,
    Direct3DDevice8_SetVertexShader,
    Direct3DDevice8_GetVertexShader,
    Direct3DDevice8_DeleteVertexShader,
    Direct3DDevice8_SetVertexShaderConstant,
    Direct3DDevice8_GetVertexShaderConstant,
    Direct3DDevice8_GetVertexShaderDeclaration,
    Direct3DDevice8_GetVertexShaderFunction,
    Direct3DDevice8_SetStreamSource,
    Direct3DDevice8_GetStreamSource,
    Direct3DDevice8_SetIndices,
    Direct3DDevice8_GetIndices,
    Direct3DDevice8_CreatePixelShader,
    Direct3DDevice8_SetPixelShader,
    Direct3DDevice8_GetPixelShader,
    Direct3DDevice8_DeletePixelShader,
    Direct3DDevice8_SetPixelShaderConstant,
    Direct3DDevice8_GetPixelShaderConstant,
    Direct3DDevice8_GetPixelShaderFunction,
    Direct3DDevice8_DrawRectPatch,
    Direct3DDevice8_DrawTriPatch,
    Direct3DDevice8_DeletePatch
};

// Device creation helper
HRESULT CreateDirect3DDevice8(Direct3D8 *pD3D8, IDirect3DDevice9 *pDevice9, Direct3DDevice8 **ppDevice8)
{
    Direct3DDevice8 *pDevice8 = (Direct3DDevice8 *)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(Direct3DDevice8));
    if (!pDevice8) return E_OUTOFMEMORY;

    pDevice8->lpVtbl = &g_Direct3DDevice8_Vtbl;
    pDevice8->RefCount = 1;
    pDevice8->pDevice9 = pDevice9;
    pDevice8->pD3D8 = pD3D8;
    pDevice8->BaseVertexIndex = 0;

    // Initialize Z-bias scale for depth bias conversion
    pDevice8->ZBiasScale = -0.000005f;

    *ppDevice8 = pDevice8;
    return D3D_OK;
}
