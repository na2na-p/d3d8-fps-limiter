/**
 * D3D8 to D3D9 Converter - IDirect3DDevice8 Implementation
 */

#include "d3d8to9_base.h"

// Forward declarations for resource wrappers
HRESULT CreateDirect3DSurface8(Direct3DDevice8 *pDevice8, IDirect3DSurface9 *pSurface9, Direct3DSurface8 **ppSurface8);
HRESULT CreateDirect3DTexture8(Direct3DDevice8 *pDevice8, IDirect3DTexture9 *pTexture9, Direct3DTexture8 **ppTexture8);
HRESULT CreateDirect3DVertexBuffer8(Direct3DDevice8 *pDevice8, IDirect3DVertexBuffer9 *pVB9, Direct3DVertexBuffer8 **ppVB8);
HRESULT CreateDirect3DIndexBuffer8(Direct3DDevice8 *pDevice8, IDirect3DIndexBuffer9 *pIB9, Direct3DIndexBuffer8 **ppIB8);
HRESULT CreateDirect3DSwapChain8(Direct3DDevice8 *pDevice8, IDirect3DSwapChain9 *pSwapChain9, Direct3DSwapChain8 **ppSwapChain8);
HRESULT CreateDirect3DCubeTexture8(Direct3DDevice8 *pDevice8, IDirect3DCubeTexture9 *pCubeTexture9, Direct3DCubeTexture8 **ppCubeTexture8);
HRESULT CreateDirect3DVolumeTexture8(Direct3DDevice8 *pDevice8, IDirect3DVolumeTexture9 *pVolumeTexture9, Direct3DVolumeTexture8 **ppVolumeTexture8);

extern IDirect3D8Vtbl g_Direct3D8_Vtbl;

// ============================================================================
// IDirect3DDevice8 Implementation
// ============================================================================

static HRESULT WINAPI Direct3DDevice8_QueryInterface(IDirect3DDevice8 *This, REFIID riid, void **ppvObject)
{
    Direct3DDevice8 *self = (Direct3DDevice8 *)This;
    if (ppvObject == NULL) return E_POINTER;

    if (IsEqualGUID(riid, &IID_IUnknown) || IsEqualGUID(riid, &IID_IDirect3DDevice8)) {
        self->RefCount++;
        *ppvObject = This;
        return S_OK;
    }

    *ppvObject = NULL;
    return E_NOINTERFACE;
}

static ULONG WINAPI Direct3DDevice8_AddRef(IDirect3DDevice8 *This)
{
    Direct3DDevice8 *self = (Direct3DDevice8 *)This;
    return ++self->RefCount;
}

static ULONG WINAPI Direct3DDevice8_Release(IDirect3DDevice8 *This)
{
    Direct3DDevice8 *self = (Direct3DDevice8 *)This;
    ULONG ref = --self->RefCount;
    if (ref == 0) {
        if (self->VertexShaderHandles) {
            HeapFree(GetProcessHeap(), 0, self->VertexShaderHandles);
        }
        if (self->PixelShaderHandles) {
            HeapFree(GetProcessHeap(), 0, self->PixelShaderHandles);
        }
        if (self->pDevice9) {
            IDirect3DDevice9_Release(self->pDevice9);
        }
        HeapFree(GetProcessHeap(), 0, self);
    }
    return ref;
}

static HRESULT WINAPI Direct3DDevice8_TestCooperativeLevel(IDirect3DDevice8 *This)
{
    Direct3DDevice8 *self = (Direct3DDevice8 *)This;
    return IDirect3DDevice9_TestCooperativeLevel(self->pDevice9);
}

static UINT WINAPI Direct3DDevice8_GetAvailableTextureMem(IDirect3DDevice8 *This)
{
    Direct3DDevice8 *self = (Direct3DDevice8 *)This;
    return IDirect3DDevice9_GetAvailableTextureMem(self->pDevice9);
}

static HRESULT WINAPI Direct3DDevice8_ResourceManagerDiscardBytes(IDirect3DDevice8 *This, DWORD Bytes)
{
    (void)This; (void)Bytes;
    // D3D9 doesn't have this method
    return D3D_OK;
}

static HRESULT WINAPI Direct3DDevice8_GetDirect3D(IDirect3DDevice8 *This, IDirect3D8 **ppD3D8)
{
    Direct3DDevice8 *self = (Direct3DDevice8 *)This;
    if (ppD3D8 == NULL) return D3DERR_INVALIDCALL;

    self->pD3D8->RefCount++;
    *ppD3D8 = (IDirect3D8 *)self->pD3D8;
    return D3D_OK;
}

static HRESULT WINAPI Direct3DDevice8_GetDeviceCaps(IDirect3DDevice8 *This, D3DCAPS8 *pCaps)
{
    Direct3DDevice8 *self = (Direct3DDevice8 *)This;
    D3DCAPS9 caps9;
    HRESULT hr = IDirect3DDevice9_GetDeviceCaps(self->pDevice9, &caps9);
    if (SUCCEEDED(hr) && pCaps) {
        ConvertCaps9to8(&caps9, pCaps);
    }
    return hr;
}

static HRESULT WINAPI Direct3DDevice8_GetDisplayMode(IDirect3DDevice8 *This, D3DDISPLAYMODE *pMode)
{
    Direct3DDevice8 *self = (Direct3DDevice8 *)This;
    return IDirect3DDevice9_GetDisplayMode(self->pDevice9, 0, pMode);
}

static HRESULT WINAPI Direct3DDevice8_GetCreationParameters(IDirect3DDevice8 *This, void *pParameters)
{
    Direct3DDevice8 *self = (Direct3DDevice8 *)This;
    return IDirect3DDevice9_GetCreationParameters(self->pDevice9, (D3DDEVICE_CREATION_PARAMETERS *)pParameters);
}

static HRESULT WINAPI Direct3DDevice8_SetCursorProperties(IDirect3DDevice8 *This, UINT XHotSpot, UINT YHotSpot, IDirect3DSurface8 *pCursorBitmap)
{
    Direct3DDevice8 *self = (Direct3DDevice8 *)This;
    Direct3DSurface8 *pSurf8 = (Direct3DSurface8 *)pCursorBitmap;
    return IDirect3DDevice9_SetCursorProperties(self->pDevice9, XHotSpot, YHotSpot, pSurf8 ? pSurf8->pSurface9 : NULL);
}

static void WINAPI Direct3DDevice8_SetCursorPosition(IDirect3DDevice8 *This, int X, int Y, DWORD Flags)
{
    Direct3DDevice8 *self = (Direct3DDevice8 *)This;
    IDirect3DDevice9_SetCursorPosition(self->pDevice9, X, Y, Flags);
}

static BOOL WINAPI Direct3DDevice8_ShowCursor(IDirect3DDevice8 *This, BOOL bShow)
{
    Direct3DDevice8 *self = (Direct3DDevice8 *)This;
    return IDirect3DDevice9_ShowCursor(self->pDevice9, bShow);
}

static HRESULT WINAPI Direct3DDevice8_CreateAdditionalSwapChain(IDirect3DDevice8 *This, D3DPRESENT_PARAMETERS8 *pPresentationParameters, IDirect3DSwapChain8 **ppSwapChain)
{
    Direct3DDevice8 *self = (Direct3DDevice8 *)This;
    if (!pPresentationParameters || !ppSwapChain) return D3DERR_INVALIDCALL;

    D3DPRESENT_PARAMETERS params9;
    ConvertPresentParameters8to9(pPresentationParameters, &params9);

    // Apply VSync setting from config
    ApplyVSyncSetting(&params9);

    IDirect3DSwapChain9 *pSwapChain9 = NULL;
    HRESULT hr = IDirect3DDevice9_CreateAdditionalSwapChain(self->pDevice9, &params9, &pSwapChain9);

    if (SUCCEEDED(hr)) {
        ConvertPresentParameters9to8(&params9, pPresentationParameters);
        hr = CreateDirect3DSwapChain8(self, pSwapChain9, (Direct3DSwapChain8 **)ppSwapChain);
        if (FAILED(hr)) {
            IDirect3DSwapChain9_Release(pSwapChain9);
        }
    }
    return hr;
}

static HRESULT WINAPI Direct3DDevice8_Reset(IDirect3DDevice8 *This, D3DPRESENT_PARAMETERS8 *pPresentationParameters)
{
    Direct3DDevice8 *self = (Direct3DDevice8 *)This;
    if (!pPresentationParameters) return D3DERR_INVALIDCALL;

    D3DPRESENT_PARAMETERS params9;
    ConvertPresentParameters8to9(pPresentationParameters, &params9);

    // Apply VSync setting from config
    ApplyVSyncSetting(&params9);

    HRESULT hr = IDirect3DDevice9_Reset(self->pDevice9, &params9);

    if (SUCCEEDED(hr)) {
        ConvertPresentParameters9to8(&params9, pPresentationParameters);
    }
    return hr;
}

static HRESULT WINAPI Direct3DDevice8_Present(IDirect3DDevice8 *This, const RECT *pSourceRect, const RECT *pDestRect, HWND hDestWindowOverride, const RGNDATA *pDirtyRegion)
{
    Direct3DDevice8 *self = (Direct3DDevice8 *)This;
    return IDirect3DDevice9_Present(self->pDevice9, pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);
}

static HRESULT WINAPI Direct3DDevice8_GetBackBuffer(IDirect3DDevice8 *This, UINT BackBuffer, D3DBACKBUFFER_TYPE Type, IDirect3DSurface8 **ppBackBuffer)
{
    Direct3DDevice8 *self = (Direct3DDevice8 *)This;
    if (!ppBackBuffer) return D3DERR_INVALIDCALL;

    IDirect3DSurface9 *pSurface9 = NULL;
    HRESULT hr = IDirect3DDevice9_GetBackBuffer(self->pDevice9, 0, BackBuffer, Type, &pSurface9);

    if (SUCCEEDED(hr)) {
        hr = CreateDirect3DSurface8(self, pSurface9, (Direct3DSurface8 **)ppBackBuffer);
        if (FAILED(hr)) {
            IDirect3DSurface9_Release(pSurface9);
        }
    }
    return hr;
}

static HRESULT WINAPI Direct3DDevice8_GetRasterStatus(IDirect3DDevice8 *This, D3DRASTER_STATUS *pRasterStatus)
{
    Direct3DDevice8 *self = (Direct3DDevice8 *)This;
    return IDirect3DDevice9_GetRasterStatus(self->pDevice9, 0, pRasterStatus);
}

static void WINAPI Direct3DDevice8_SetGammaRamp(IDirect3DDevice8 *This, DWORD Flags, const D3DGAMMARAMP *pRamp)
{
    Direct3DDevice8 *self = (Direct3DDevice8 *)This;
    IDirect3DDevice9_SetGammaRamp(self->pDevice9, 0, Flags, pRamp);
}

static void WINAPI Direct3DDevice8_GetGammaRamp(IDirect3DDevice8 *This, D3DGAMMARAMP *pRamp)
{
    Direct3DDevice8 *self = (Direct3DDevice8 *)This;
    IDirect3DDevice9_GetGammaRamp(self->pDevice9, 0, pRamp);
}

static HRESULT WINAPI Direct3DDevice8_CreateTexture(IDirect3DDevice8 *This, UINT Width, UINT Height, UINT Levels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DTexture8 **ppTexture)
{
    Direct3DDevice8 *self = (Direct3DDevice8 *)This;
    if (!ppTexture) return D3DERR_INVALIDCALL;

    IDirect3DTexture9 *pTexture9 = NULL;
    HRESULT hr = IDirect3DDevice9_CreateTexture(self->pDevice9, Width, Height, Levels, Usage, Format, Pool, &pTexture9, NULL);

    if (SUCCEEDED(hr)) {
        hr = CreateDirect3DTexture8(self, pTexture9, (Direct3DTexture8 **)ppTexture);
        if (FAILED(hr)) {
            IDirect3DTexture9_Release(pTexture9);
        }
    }
    return hr;
}

static HRESULT WINAPI Direct3DDevice8_CreateVolumeTexture(IDirect3DDevice8 *This, UINT Width, UINT Height, UINT Depth, UINT Levels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DVolumeTexture8 **ppVolumeTexture)
{
    Direct3DDevice8 *self = (Direct3DDevice8 *)This;
    if (!ppVolumeTexture) return D3DERR_INVALIDCALL;

    IDirect3DVolumeTexture9 *pVolumeTexture9 = NULL;
    HRESULT hr = IDirect3DDevice9_CreateVolumeTexture(self->pDevice9, Width, Height, Depth, Levels, Usage, Format, Pool, &pVolumeTexture9, NULL);

    if (SUCCEEDED(hr)) {
        hr = CreateDirect3DVolumeTexture8(self, pVolumeTexture9, (Direct3DVolumeTexture8 **)ppVolumeTexture);
        if (FAILED(hr)) {
            IDirect3DVolumeTexture9_Release(pVolumeTexture9);
        }
    }
    return hr;
}

static HRESULT WINAPI Direct3DDevice8_CreateCubeTexture(IDirect3DDevice8 *This, UINT EdgeLength, UINT Levels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DCubeTexture8 **ppCubeTexture)
{
    Direct3DDevice8 *self = (Direct3DDevice8 *)This;
    if (!ppCubeTexture) return D3DERR_INVALIDCALL;

    IDirect3DCubeTexture9 *pCubeTexture9 = NULL;
    HRESULT hr = IDirect3DDevice9_CreateCubeTexture(self->pDevice9, EdgeLength, Levels, Usage, Format, Pool, &pCubeTexture9, NULL);

    if (SUCCEEDED(hr)) {
        hr = CreateDirect3DCubeTexture8(self, pCubeTexture9, (Direct3DCubeTexture8 **)ppCubeTexture);
        if (FAILED(hr)) {
            IDirect3DCubeTexture9_Release(pCubeTexture9);
        }
    }
    return hr;
}

static HRESULT WINAPI Direct3DDevice8_CreateVertexBuffer(IDirect3DDevice8 *This, UINT Length, DWORD Usage, DWORD FVF, D3DPOOL Pool, IDirect3DVertexBuffer8 **ppVertexBuffer)
{
    Direct3DDevice8 *self = (Direct3DDevice8 *)This;
    if (!ppVertexBuffer) return D3DERR_INVALIDCALL;

    IDirect3DVertexBuffer9 *pVB9 = NULL;
    HRESULT hr = IDirect3DDevice9_CreateVertexBuffer(self->pDevice9, Length, Usage, FVF, Pool, &pVB9, NULL);

    if (SUCCEEDED(hr)) {
        hr = CreateDirect3DVertexBuffer8(self, pVB9, (Direct3DVertexBuffer8 **)ppVertexBuffer);
        if (FAILED(hr)) {
            IDirect3DVertexBuffer9_Release(pVB9);
        }
    }
    return hr;
}

static HRESULT WINAPI Direct3DDevice8_CreateIndexBuffer(IDirect3DDevice8 *This, UINT Length, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DIndexBuffer8 **ppIndexBuffer)
{
    Direct3DDevice8 *self = (Direct3DDevice8 *)This;
    if (!ppIndexBuffer) return D3DERR_INVALIDCALL;

    IDirect3DIndexBuffer9 *pIB9 = NULL;
    HRESULT hr = IDirect3DDevice9_CreateIndexBuffer(self->pDevice9, Length, Usage, Format, Pool, &pIB9, NULL);

    if (SUCCEEDED(hr)) {
        hr = CreateDirect3DIndexBuffer8(self, pIB9, (Direct3DIndexBuffer8 **)ppIndexBuffer);
        if (FAILED(hr)) {
            IDirect3DIndexBuffer9_Release(pIB9);
        }
    }
    return hr;
}

static HRESULT WINAPI Direct3DDevice8_CreateRenderTarget(IDirect3DDevice8 *This, UINT Width, UINT Height, D3DFORMAT Format, D3DMULTISAMPLE_TYPE MultiSample, BOOL Lockable, IDirect3DSurface8 **ppSurface)
{
    Direct3DDevice8 *self = (Direct3DDevice8 *)This;
    if (!ppSurface) return D3DERR_INVALIDCALL;

    IDirect3DSurface9 *pSurface9 = NULL;
    HRESULT hr = IDirect3DDevice9_CreateRenderTarget(self->pDevice9, Width, Height, Format, MultiSample, 0, Lockable, &pSurface9, NULL);

    if (SUCCEEDED(hr)) {
        hr = CreateDirect3DSurface8(self, pSurface9, (Direct3DSurface8 **)ppSurface);
        if (FAILED(hr)) {
            IDirect3DSurface9_Release(pSurface9);
        }
    }
    return hr;
}

static HRESULT WINAPI Direct3DDevice8_CreateDepthStencilSurface(IDirect3DDevice8 *This, UINT Width, UINT Height, D3DFORMAT Format, D3DMULTISAMPLE_TYPE MultiSample, IDirect3DSurface8 **ppSurface)
{
    Direct3DDevice8 *self = (Direct3DDevice8 *)This;
    if (!ppSurface) return D3DERR_INVALIDCALL;

    IDirect3DSurface9 *pSurface9 = NULL;
    HRESULT hr = IDirect3DDevice9_CreateDepthStencilSurface(self->pDevice9, Width, Height, Format, MultiSample, 0, FALSE, &pSurface9, NULL);

    if (SUCCEEDED(hr)) {
        hr = CreateDirect3DSurface8(self, pSurface9, (Direct3DSurface8 **)ppSurface);
        if (FAILED(hr)) {
            IDirect3DSurface9_Release(pSurface9);
        }
    }
    return hr;
}

static HRESULT WINAPI Direct3DDevice8_CreateImageSurface(IDirect3DDevice8 *This, UINT Width, UINT Height, D3DFORMAT Format, IDirect3DSurface8 **ppSurface)
{
    Direct3DDevice8 *self = (Direct3DDevice8 *)This;
    if (!ppSurface) return D3DERR_INVALIDCALL;

    IDirect3DSurface9 *pSurface9 = NULL;
    HRESULT hr = IDirect3DDevice9_CreateOffscreenPlainSurface(self->pDevice9, Width, Height, Format, D3DPOOL_SYSTEMMEM, &pSurface9, NULL);

    if (SUCCEEDED(hr)) {
        hr = CreateDirect3DSurface8(self, pSurface9, (Direct3DSurface8 **)ppSurface);
        if (FAILED(hr)) {
            IDirect3DSurface9_Release(pSurface9);
        }
    }
    return hr;
}

static HRESULT WINAPI Direct3DDevice8_CopyRects(IDirect3DDevice8 *This, IDirect3DSurface8 *pSourceSurface, const RECT *pSourceRectsArray, UINT cRects, IDirect3DSurface8 *pDestinationSurface, const POINT *pDestPointsArray)
{
    Direct3DDevice8 *self = (Direct3DDevice8 *)This;
    Direct3DSurface8 *pSrc = (Direct3DSurface8 *)pSourceSurface;
    Direct3DSurface8 *pDst = (Direct3DSurface8 *)pDestinationSurface;

    if (!pSrc || !pDst) return D3DERR_INVALIDCALL;

    // D3D9 uses UpdateSurface or StretchRect instead
    if (cRects == 0) {
        return IDirect3DDevice9_UpdateSurface(self->pDevice9, pSrc->pSurface9, NULL, pDst->pSurface9, NULL);
    }

    HRESULT hr = D3D_OK;
    for (UINT i = 0; i < cRects && SUCCEEDED(hr); i++) {
        POINT destPoint = pDestPointsArray ? pDestPointsArray[i] : (POINT){pSourceRectsArray[i].left, pSourceRectsArray[i].top};
        hr = IDirect3DDevice9_UpdateSurface(self->pDevice9, pSrc->pSurface9, &pSourceRectsArray[i], pDst->pSurface9, &destPoint);
    }
    return hr;
}

static HRESULT WINAPI Direct3DDevice8_UpdateTexture(IDirect3DDevice8 *This, IDirect3DBaseTexture8 *pSourceTexture, IDirect3DBaseTexture8 *pDestinationTexture)
{
    Direct3DDevice8 *self = (Direct3DDevice8 *)This;
    Direct3DTexture8 *pSrc = (Direct3DTexture8 *)pSourceTexture;
    Direct3DTexture8 *pDst = (Direct3DTexture8 *)pDestinationTexture;

    if (!pSrc || !pDst) return D3DERR_INVALIDCALL;

    return IDirect3DDevice9_UpdateTexture(self->pDevice9, (IDirect3DBaseTexture9 *)pSrc->pTexture9, (IDirect3DBaseTexture9 *)pDst->pTexture9);
}

static HRESULT WINAPI Direct3DDevice8_GetFrontBuffer(IDirect3DDevice8 *This, IDirect3DSurface8 *pDestSurface)
{
    Direct3DDevice8 *self = (Direct3DDevice8 *)This;
    Direct3DSurface8 *pDst = (Direct3DSurface8 *)pDestSurface;
    if (!pDst) return D3DERR_INVALIDCALL;

    return IDirect3DDevice9_GetFrontBufferData(self->pDevice9, 0, pDst->pSurface9);
}

static HRESULT WINAPI Direct3DDevice8_SetRenderTarget(IDirect3DDevice8 *This, IDirect3DSurface8 *pRenderTarget, IDirect3DSurface8 *pNewZStencil)
{
    Direct3DDevice8 *self = (Direct3DDevice8 *)This;
    Direct3DSurface8 *pRT = (Direct3DSurface8 *)pRenderTarget;
    Direct3DSurface8 *pDS = (Direct3DSurface8 *)pNewZStencil;

    HRESULT hr = D3D_OK;
    if (pRT) {
        hr = IDirect3DDevice9_SetRenderTarget(self->pDevice9, 0, pRT->pSurface9);
    }
    if (SUCCEEDED(hr)) {
        hr = IDirect3DDevice9_SetDepthStencilSurface(self->pDevice9, pDS ? pDS->pSurface9 : NULL);
    }
    return hr;
}

static HRESULT WINAPI Direct3DDevice8_GetRenderTarget(IDirect3DDevice8 *This, IDirect3DSurface8 **ppRenderTarget)
{
    Direct3DDevice8 *self = (Direct3DDevice8 *)This;
    if (!ppRenderTarget) return D3DERR_INVALIDCALL;

    IDirect3DSurface9 *pSurface9 = NULL;
    HRESULT hr = IDirect3DDevice9_GetRenderTarget(self->pDevice9, 0, &pSurface9);

    if (SUCCEEDED(hr)) {
        hr = CreateDirect3DSurface8(self, pSurface9, (Direct3DSurface8 **)ppRenderTarget);
        if (FAILED(hr)) {
            IDirect3DSurface9_Release(pSurface9);
        }
    }
    return hr;
}

static HRESULT WINAPI Direct3DDevice8_GetDepthStencilSurface(IDirect3DDevice8 *This, IDirect3DSurface8 **ppZStencilSurface)
{
    Direct3DDevice8 *self = (Direct3DDevice8 *)This;
    if (!ppZStencilSurface) return D3DERR_INVALIDCALL;

    IDirect3DSurface9 *pSurface9 = NULL;
    HRESULT hr = IDirect3DDevice9_GetDepthStencilSurface(self->pDevice9, &pSurface9);

    if (SUCCEEDED(hr)) {
        hr = CreateDirect3DSurface8(self, pSurface9, (Direct3DSurface8 **)ppZStencilSurface);
        if (FAILED(hr)) {
            IDirect3DSurface9_Release(pSurface9);
        }
    }
    return hr;
}

static HRESULT WINAPI Direct3DDevice8_BeginScene(IDirect3DDevice8 *This)
{
    Direct3DDevice8 *self = (Direct3DDevice8 *)This;
    return IDirect3DDevice9_BeginScene(self->pDevice9);
}

static HRESULT WINAPI Direct3DDevice8_EndScene(IDirect3DDevice8 *This)
{
    Direct3DDevice8 *self = (Direct3DDevice8 *)This;
    return IDirect3DDevice9_EndScene(self->pDevice9);
}

static HRESULT WINAPI Direct3DDevice8_Clear(IDirect3DDevice8 *This, DWORD Count, const D3DRECT *pRects, DWORD Flags, D3DCOLOR Color, float Z, DWORD Stencil)
{
    Direct3DDevice8 *self = (Direct3DDevice8 *)This;
    return IDirect3DDevice9_Clear(self->pDevice9, Count, pRects, Flags, Color, Z, Stencil);
}

static HRESULT WINAPI Direct3DDevice8_SetTransform(IDirect3DDevice8 *This, D3DTRANSFORMSTATETYPE State, const D3DMATRIX *pMatrix)
{
    Direct3DDevice8 *self = (Direct3DDevice8 *)This;
    return IDirect3DDevice9_SetTransform(self->pDevice9, State, pMatrix);
}

static HRESULT WINAPI Direct3DDevice8_GetTransform(IDirect3DDevice8 *This, D3DTRANSFORMSTATETYPE State, D3DMATRIX *pMatrix)
{
    Direct3DDevice8 *self = (Direct3DDevice8 *)This;
    return IDirect3DDevice9_GetTransform(self->pDevice9, State, pMatrix);
}

static HRESULT WINAPI Direct3DDevice8_MultiplyTransform(IDirect3DDevice8 *This, D3DTRANSFORMSTATETYPE State, const D3DMATRIX *pMatrix)
{
    Direct3DDevice8 *self = (Direct3DDevice8 *)This;
    return IDirect3DDevice9_MultiplyTransform(self->pDevice9, State, pMatrix);
}

static HRESULT WINAPI Direct3DDevice8_SetViewport(IDirect3DDevice8 *This, const D3DVIEWPORT8 *pViewport)
{
    Direct3DDevice8 *self = (Direct3DDevice8 *)This;
    return IDirect3DDevice9_SetViewport(self->pDevice9, (const D3DVIEWPORT9 *)pViewport);
}

static HRESULT WINAPI Direct3DDevice8_GetViewport(IDirect3DDevice8 *This, D3DVIEWPORT8 *pViewport)
{
    Direct3DDevice8 *self = (Direct3DDevice8 *)This;
    return IDirect3DDevice9_GetViewport(self->pDevice9, (D3DVIEWPORT9 *)pViewport);
}

static HRESULT WINAPI Direct3DDevice8_SetMaterial(IDirect3DDevice8 *This, const D3DMATERIAL8 *pMaterial)
{
    Direct3DDevice8 *self = (Direct3DDevice8 *)This;
    return IDirect3DDevice9_SetMaterial(self->pDevice9, (const D3DMATERIAL9 *)pMaterial);
}

static HRESULT WINAPI Direct3DDevice8_GetMaterial(IDirect3DDevice8 *This, D3DMATERIAL8 *pMaterial)
{
    Direct3DDevice8 *self = (Direct3DDevice8 *)This;
    return IDirect3DDevice9_GetMaterial(self->pDevice9, (D3DMATERIAL9 *)pMaterial);
}

static HRESULT WINAPI Direct3DDevice8_SetLight(IDirect3DDevice8 *This, DWORD Index, const D3DLIGHT8 *pLight)
{
    Direct3DDevice8 *self = (Direct3DDevice8 *)This;
    return IDirect3DDevice9_SetLight(self->pDevice9, Index, (const D3DLIGHT9 *)pLight);
}

static HRESULT WINAPI Direct3DDevice8_GetLight(IDirect3DDevice8 *This, DWORD Index, D3DLIGHT8 *pLight)
{
    Direct3DDevice8 *self = (Direct3DDevice8 *)This;
    return IDirect3DDevice9_GetLight(self->pDevice9, Index, (D3DLIGHT9 *)pLight);
}

static HRESULT WINAPI Direct3DDevice8_LightEnable(IDirect3DDevice8 *This, DWORD Index, BOOL Enable)
{
    Direct3DDevice8 *self = (Direct3DDevice8 *)This;
    return IDirect3DDevice9_LightEnable(self->pDevice9, Index, Enable);
}

static HRESULT WINAPI Direct3DDevice8_GetLightEnable(IDirect3DDevice8 *This, DWORD Index, BOOL *pEnable)
{
    Direct3DDevice8 *self = (Direct3DDevice8 *)This;
    return IDirect3DDevice9_GetLightEnable(self->pDevice9, Index, pEnable);
}

static HRESULT WINAPI Direct3DDevice8_SetClipPlane(IDirect3DDevice8 *This, DWORD Index, const float *pPlane)
{
    Direct3DDevice8 *self = (Direct3DDevice8 *)This;
    return IDirect3DDevice9_SetClipPlane(self->pDevice9, Index, pPlane);
}

static HRESULT WINAPI Direct3DDevice8_GetClipPlane(IDirect3DDevice8 *This, DWORD Index, float *pPlane)
{
    Direct3DDevice8 *self = (Direct3DDevice8 *)This;
    return IDirect3DDevice9_GetClipPlane(self->pDevice9, Index, pPlane);
}

static HRESULT WINAPI Direct3DDevice8_SetRenderState(IDirect3DDevice8 *This, D3DRENDERSTATETYPE State, DWORD Value)
{
    Direct3DDevice8 *self = (Direct3DDevice8 *)This;

    // Handle D3D8-only render states
    switch (State) {
        case D3DRS8_ZBIAS:
            // Convert ZBIAS to DEPTHBIAS
            {
                float bias = (float)Value * -0.000005f;
                return IDirect3DDevice9_SetRenderState(self->pDevice9, D3DRS_DEPTHBIAS, *(DWORD *)&bias);
            }
        case D3DRS8_SOFTWAREVERTEXPROCESSING:
            return IDirect3DDevice9_SetSoftwareVertexProcessing(self->pDevice9, Value);
        case D3DRS8_PATCHSEGMENTS:
            // Not supported in D3D9
            return D3D_OK;
        default:
            return IDirect3DDevice9_SetRenderState(self->pDevice9, State, Value);
    }
}

static HRESULT WINAPI Direct3DDevice8_GetRenderState(IDirect3DDevice8 *This, D3DRENDERSTATETYPE State, DWORD *pValue)
{
    Direct3DDevice8 *self = (Direct3DDevice8 *)This;

    switch (State) {
        case D3DRS8_ZBIAS:
            {
                DWORD bias;
                HRESULT hr = IDirect3DDevice9_GetRenderState(self->pDevice9, D3DRS_DEPTHBIAS, &bias);
                if (SUCCEEDED(hr) && pValue) {
                    float fBias = *(float *)&bias;
                    *pValue = (DWORD)(fBias / -0.000005f);
                }
                return hr;
            }
        case D3DRS8_SOFTWAREVERTEXPROCESSING:
            if (pValue) {
                *pValue = IDirect3DDevice9_GetSoftwareVertexProcessing(self->pDevice9);
            }
            return D3D_OK;
        default:
            return IDirect3DDevice9_GetRenderState(self->pDevice9, State, pValue);
    }
}

// Continue in next file due to size...
// This file is continued in d3d8to9_device2.c
