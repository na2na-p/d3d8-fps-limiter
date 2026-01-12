/**
 * D3D8 to D3D9 Converter - Resource Wrappers
 */

#include "d3d8to9_base.h"

// ============================================================================
// IDirect3DSurface8 Implementation
// ============================================================================

static HRESULT WINAPI Direct3DSurface8_QueryInterface(IDirect3DSurface8 *This, REFIID riid, void **ppvObject)
{
    Direct3DSurface8 *self = (Direct3DSurface8 *)This;
    if (!ppvObject) return E_POINTER;

    if (IsEqualGUID(riid, &IID_IUnknown) || IsEqualGUID(riid, &IID_IDirect3DSurface8)) {
        self->RefCount++;
        *ppvObject = This;
        return S_OK;
    }
    *ppvObject = NULL;
    return E_NOINTERFACE;
}

static ULONG WINAPI Direct3DSurface8_AddRef(IDirect3DSurface8 *This)
{
    Direct3DSurface8 *self = (Direct3DSurface8 *)This;
    return ++self->RefCount;
}

static ULONG WINAPI Direct3DSurface8_Release(IDirect3DSurface8 *This)
{
    Direct3DSurface8 *self = (Direct3DSurface8 *)This;
    ULONG ref = --self->RefCount;
    if (ref == 0) {
        if (self->pSurface9) IDirect3DSurface9_Release(self->pSurface9);
        HeapFree(GetProcessHeap(), 0, self);
    }
    return ref;
}

static HRESULT WINAPI Direct3DSurface8_GetDevice(IDirect3DSurface8 *This, IDirect3DDevice8 **ppDevice)
{
    Direct3DSurface8 *self = (Direct3DSurface8 *)This;
    if (!ppDevice) return D3DERR_INVALIDCALL;
    self->pDevice8->RefCount++;
    *ppDevice = (IDirect3DDevice8 *)self->pDevice8;
    return D3D_OK;
}

static HRESULT WINAPI Direct3DSurface8_SetPrivateData(IDirect3DSurface8 *This, REFGUID refguid, const void *pData, DWORD SizeOfData, DWORD Flags)
{
    Direct3DSurface8 *self = (Direct3DSurface8 *)This;
    return IDirect3DSurface9_SetPrivateData(self->pSurface9, refguid, pData, SizeOfData, Flags);
}

static HRESULT WINAPI Direct3DSurface8_GetPrivateData(IDirect3DSurface8 *This, REFGUID refguid, void *pData, DWORD *pSizeOfData)
{
    Direct3DSurface8 *self = (Direct3DSurface8 *)This;
    return IDirect3DSurface9_GetPrivateData(self->pSurface9, refguid, pData, pSizeOfData);
}

static HRESULT WINAPI Direct3DSurface8_FreePrivateData(IDirect3DSurface8 *This, REFGUID refguid)
{
    Direct3DSurface8 *self = (Direct3DSurface8 *)This;
    return IDirect3DSurface9_FreePrivateData(self->pSurface9, refguid);
}

static HRESULT WINAPI Direct3DSurface8_GetContainer(IDirect3DSurface8 *This, REFIID riid, void **ppContainer)
{
    Direct3DSurface8 *self = (Direct3DSurface8 *)This;
    return IDirect3DSurface9_GetContainer(self->pSurface9, riid, ppContainer);
}

static HRESULT WINAPI Direct3DSurface8_GetDesc(IDirect3DSurface8 *This, D3DSURFACE_DESC8 *pDesc)
{
    Direct3DSurface8 *self = (Direct3DSurface8 *)This;
    D3DSURFACE_DESC desc9;
    HRESULT hr = IDirect3DSurface9_GetDesc(self->pSurface9, &desc9);
    if (SUCCEEDED(hr) && pDesc) {
        ConvertSurfaceDesc9to8(&desc9, pDesc);
    }
    return hr;
}

static HRESULT WINAPI Direct3DSurface8_LockRect(IDirect3DSurface8 *This, D3DLOCKED_RECT *pLockedRect, const RECT *pRect, DWORD Flags)
{
    Direct3DSurface8 *self = (Direct3DSurface8 *)This;
    return IDirect3DSurface9_LockRect(self->pSurface9, pLockedRect, pRect, Flags);
}

static HRESULT WINAPI Direct3DSurface8_UnlockRect(IDirect3DSurface8 *This)
{
    Direct3DSurface8 *self = (Direct3DSurface8 *)This;
    return IDirect3DSurface9_UnlockRect(self->pSurface9);
}

static IDirect3DSurface8Vtbl g_Direct3DSurface8_Vtbl = {
    Direct3DSurface8_QueryInterface,
    Direct3DSurface8_AddRef,
    Direct3DSurface8_Release,
    Direct3DSurface8_GetDevice,
    Direct3DSurface8_SetPrivateData,
    Direct3DSurface8_GetPrivateData,
    Direct3DSurface8_FreePrivateData,
    Direct3DSurface8_GetContainer,
    Direct3DSurface8_GetDesc,
    Direct3DSurface8_LockRect,
    Direct3DSurface8_UnlockRect
};

HRESULT CreateDirect3DSurface8(Direct3DDevice8 *pDevice8, IDirect3DSurface9 *pSurface9, Direct3DSurface8 **ppSurface8)
{
    Direct3DSurface8 *pSurface8 = (Direct3DSurface8 *)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(Direct3DSurface8));
    if (!pSurface8) return E_OUTOFMEMORY;

    pSurface8->lpVtbl = &g_Direct3DSurface8_Vtbl;
    pSurface8->RefCount = 1;
    pSurface8->pSurface9 = pSurface9;
    pSurface8->pDevice8 = pDevice8;

    *ppSurface8 = pSurface8;
    return D3D_OK;
}

// ============================================================================
// IDirect3DTexture8 Implementation
// ============================================================================

static HRESULT WINAPI Direct3DTexture8_QueryInterface(IDirect3DTexture8 *This, REFIID riid, void **ppvObject)
{
    Direct3DTexture8 *self = (Direct3DTexture8 *)This;
    if (!ppvObject) return E_POINTER;

    if (IsEqualGUID(riid, &IID_IUnknown) || IsEqualGUID(riid, &IID_IDirect3DTexture8) ||
        IsEqualGUID(riid, &IID_IDirect3DBaseTexture8) || IsEqualGUID(riid, &IID_IDirect3DResource8)) {
        self->RefCount++;
        *ppvObject = This;
        return S_OK;
    }
    *ppvObject = NULL;
    return E_NOINTERFACE;
}

static ULONG WINAPI Direct3DTexture8_AddRef(IDirect3DTexture8 *This)
{
    Direct3DTexture8 *self = (Direct3DTexture8 *)This;
    return ++self->RefCount;
}

static ULONG WINAPI Direct3DTexture8_Release(IDirect3DTexture8 *This)
{
    Direct3DTexture8 *self = (Direct3DTexture8 *)This;
    ULONG ref = --self->RefCount;
    if (ref == 0) {
        if (self->pTexture9) IDirect3DTexture9_Release(self->pTexture9);
        HeapFree(GetProcessHeap(), 0, self);
    }
    return ref;
}

static HRESULT WINAPI Direct3DTexture8_GetDevice(IDirect3DTexture8 *This, IDirect3DDevice8 **ppDevice)
{
    Direct3DTexture8 *self = (Direct3DTexture8 *)This;
    if (!ppDevice) return D3DERR_INVALIDCALL;
    self->pDevice8->RefCount++;
    *ppDevice = (IDirect3DDevice8 *)self->pDevice8;
    return D3D_OK;
}

static HRESULT WINAPI Direct3DTexture8_SetPrivateData(IDirect3DTexture8 *This, REFGUID refguid, const void *pData, DWORD SizeOfData, DWORD Flags)
{
    Direct3DTexture8 *self = (Direct3DTexture8 *)This;
    return IDirect3DTexture9_SetPrivateData(self->pTexture9, refguid, pData, SizeOfData, Flags);
}

static HRESULT WINAPI Direct3DTexture8_GetPrivateData(IDirect3DTexture8 *This, REFGUID refguid, void *pData, DWORD *pSizeOfData)
{
    Direct3DTexture8 *self = (Direct3DTexture8 *)This;
    return IDirect3DTexture9_GetPrivateData(self->pTexture9, refguid, pData, pSizeOfData);
}

static HRESULT WINAPI Direct3DTexture8_FreePrivateData(IDirect3DTexture8 *This, REFGUID refguid)
{
    Direct3DTexture8 *self = (Direct3DTexture8 *)This;
    return IDirect3DTexture9_FreePrivateData(self->pTexture9, refguid);
}

static DWORD WINAPI Direct3DTexture8_SetPriority(IDirect3DTexture8 *This, DWORD PriorityNew)
{
    Direct3DTexture8 *self = (Direct3DTexture8 *)This;
    return IDirect3DTexture9_SetPriority(self->pTexture9, PriorityNew);
}

static DWORD WINAPI Direct3DTexture8_GetPriority(IDirect3DTexture8 *This)
{
    Direct3DTexture8 *self = (Direct3DTexture8 *)This;
    return IDirect3DTexture9_GetPriority(self->pTexture9);
}

static void WINAPI Direct3DTexture8_PreLoad(IDirect3DTexture8 *This)
{
    Direct3DTexture8 *self = (Direct3DTexture8 *)This;
    IDirect3DTexture9_PreLoad(self->pTexture9);
}

static D3DRESOURCETYPE WINAPI Direct3DTexture8_GetType(IDirect3DTexture8 *This)
{
    Direct3DTexture8 *self = (Direct3DTexture8 *)This;
    return IDirect3DTexture9_GetType(self->pTexture9);
}

static DWORD WINAPI Direct3DTexture8_SetLOD(IDirect3DTexture8 *This, DWORD LODNew)
{
    Direct3DTexture8 *self = (Direct3DTexture8 *)This;
    return IDirect3DTexture9_SetLOD(self->pTexture9, LODNew);
}

static DWORD WINAPI Direct3DTexture8_GetLOD(IDirect3DTexture8 *This)
{
    Direct3DTexture8 *self = (Direct3DTexture8 *)This;
    return IDirect3DTexture9_GetLOD(self->pTexture9);
}

static DWORD WINAPI Direct3DTexture8_GetLevelCount(IDirect3DTexture8 *This)
{
    Direct3DTexture8 *self = (Direct3DTexture8 *)This;
    return IDirect3DTexture9_GetLevelCount(self->pTexture9);
}

static HRESULT WINAPI Direct3DTexture8_GetLevelDesc(IDirect3DTexture8 *This, UINT Level, D3DSURFACE_DESC8 *pDesc)
{
    Direct3DTexture8 *self = (Direct3DTexture8 *)This;
    D3DSURFACE_DESC desc9;
    HRESULT hr = IDirect3DTexture9_GetLevelDesc(self->pTexture9, Level, &desc9);
    if (SUCCEEDED(hr) && pDesc) {
        ConvertSurfaceDesc9to8(&desc9, pDesc);
    }
    return hr;
}

static HRESULT WINAPI Direct3DTexture8_GetSurfaceLevel(IDirect3DTexture8 *This, UINT Level, IDirect3DSurface8 **ppSurfaceLevel)
{
    Direct3DTexture8 *self = (Direct3DTexture8 *)This;
    if (!ppSurfaceLevel) return D3DERR_INVALIDCALL;

    IDirect3DSurface9 *pSurface9 = NULL;
    HRESULT hr = IDirect3DTexture9_GetSurfaceLevel(self->pTexture9, Level, &pSurface9);

    if (SUCCEEDED(hr)) {
        hr = CreateDirect3DSurface8(self->pDevice8, pSurface9, (Direct3DSurface8 **)ppSurfaceLevel);
        if (FAILED(hr)) {
            IDirect3DSurface9_Release(pSurface9);
        }
    }
    return hr;
}

static HRESULT WINAPI Direct3DTexture8_LockRect(IDirect3DTexture8 *This, UINT Level, D3DLOCKED_RECT *pLockedRect, const RECT *pRect, DWORD Flags)
{
    Direct3DTexture8 *self = (Direct3DTexture8 *)This;
    return IDirect3DTexture9_LockRect(self->pTexture9, Level, pLockedRect, pRect, Flags);
}

static HRESULT WINAPI Direct3DTexture8_UnlockRect(IDirect3DTexture8 *This, UINT Level)
{
    Direct3DTexture8 *self = (Direct3DTexture8 *)This;
    return IDirect3DTexture9_UnlockRect(self->pTexture9, Level);
}

static HRESULT WINAPI Direct3DTexture8_AddDirtyRect(IDirect3DTexture8 *This, const RECT *pDirtyRect)
{
    Direct3DTexture8 *self = (Direct3DTexture8 *)This;
    return IDirect3DTexture9_AddDirtyRect(self->pTexture9, pDirtyRect);
}

static IDirect3DTexture8Vtbl g_Direct3DTexture8_Vtbl = {
    Direct3DTexture8_QueryInterface,
    Direct3DTexture8_AddRef,
    Direct3DTexture8_Release,
    Direct3DTexture8_GetDevice,
    Direct3DTexture8_SetPrivateData,
    Direct3DTexture8_GetPrivateData,
    Direct3DTexture8_FreePrivateData,
    Direct3DTexture8_SetPriority,
    Direct3DTexture8_GetPriority,
    Direct3DTexture8_PreLoad,
    Direct3DTexture8_GetType,
    Direct3DTexture8_SetLOD,
    Direct3DTexture8_GetLOD,
    Direct3DTexture8_GetLevelCount,
    Direct3DTexture8_GetLevelDesc,
    Direct3DTexture8_GetSurfaceLevel,
    Direct3DTexture8_LockRect,
    Direct3DTexture8_UnlockRect,
    Direct3DTexture8_AddDirtyRect
};

HRESULT CreateDirect3DTexture8(Direct3DDevice8 *pDevice8, IDirect3DTexture9 *pTexture9, Direct3DTexture8 **ppTexture8)
{
    Direct3DTexture8 *pTexture8 = (Direct3DTexture8 *)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(Direct3DTexture8));
    if (!pTexture8) return E_OUTOFMEMORY;

    pTexture8->lpVtbl = &g_Direct3DTexture8_Vtbl;
    pTexture8->RefCount = 1;
    pTexture8->pTexture9 = pTexture9;
    pTexture8->pDevice8 = pDevice8;

    *ppTexture8 = pTexture8;
    return D3D_OK;
}

// ============================================================================
// IDirect3DVertexBuffer8 Implementation
// ============================================================================

static HRESULT WINAPI Direct3DVertexBuffer8_QueryInterface(IDirect3DVertexBuffer8 *This, REFIID riid, void **ppvObject)
{
    Direct3DVertexBuffer8 *self = (Direct3DVertexBuffer8 *)This;
    if (!ppvObject) return E_POINTER;

    if (IsEqualGUID(riid, &IID_IUnknown) || IsEqualGUID(riid, &IID_IDirect3DVertexBuffer8) ||
        IsEqualGUID(riid, &IID_IDirect3DResource8)) {
        self->RefCount++;
        *ppvObject = This;
        return S_OK;
    }
    *ppvObject = NULL;
    return E_NOINTERFACE;
}

static ULONG WINAPI Direct3DVertexBuffer8_AddRef(IDirect3DVertexBuffer8 *This)
{
    Direct3DVertexBuffer8 *self = (Direct3DVertexBuffer8 *)This;
    return ++self->RefCount;
}

static ULONG WINAPI Direct3DVertexBuffer8_Release(IDirect3DVertexBuffer8 *This)
{
    Direct3DVertexBuffer8 *self = (Direct3DVertexBuffer8 *)This;
    ULONG ref = --self->RefCount;
    if (ref == 0) {
        if (self->pVB9) IDirect3DVertexBuffer9_Release(self->pVB9);
        HeapFree(GetProcessHeap(), 0, self);
    }
    return ref;
}

static HRESULT WINAPI Direct3DVertexBuffer8_GetDevice(IDirect3DVertexBuffer8 *This, IDirect3DDevice8 **ppDevice)
{
    Direct3DVertexBuffer8 *self = (Direct3DVertexBuffer8 *)This;
    if (!ppDevice) return D3DERR_INVALIDCALL;
    self->pDevice8->RefCount++;
    *ppDevice = (IDirect3DDevice8 *)self->pDevice8;
    return D3D_OK;
}

static HRESULT WINAPI Direct3DVertexBuffer8_SetPrivateData(IDirect3DVertexBuffer8 *This, REFGUID refguid, const void *pData, DWORD SizeOfData, DWORD Flags)
{
    Direct3DVertexBuffer8 *self = (Direct3DVertexBuffer8 *)This;
    return IDirect3DVertexBuffer9_SetPrivateData(self->pVB9, refguid, pData, SizeOfData, Flags);
}

static HRESULT WINAPI Direct3DVertexBuffer8_GetPrivateData(IDirect3DVertexBuffer8 *This, REFGUID refguid, void *pData, DWORD *pSizeOfData)
{
    Direct3DVertexBuffer8 *self = (Direct3DVertexBuffer8 *)This;
    return IDirect3DVertexBuffer9_GetPrivateData(self->pVB9, refguid, pData, pSizeOfData);
}

static HRESULT WINAPI Direct3DVertexBuffer8_FreePrivateData(IDirect3DVertexBuffer8 *This, REFGUID refguid)
{
    Direct3DVertexBuffer8 *self = (Direct3DVertexBuffer8 *)This;
    return IDirect3DVertexBuffer9_FreePrivateData(self->pVB9, refguid);
}

static DWORD WINAPI Direct3DVertexBuffer8_SetPriority(IDirect3DVertexBuffer8 *This, DWORD PriorityNew)
{
    Direct3DVertexBuffer8 *self = (Direct3DVertexBuffer8 *)This;
    return IDirect3DVertexBuffer9_SetPriority(self->pVB9, PriorityNew);
}

static DWORD WINAPI Direct3DVertexBuffer8_GetPriority(IDirect3DVertexBuffer8 *This)
{
    Direct3DVertexBuffer8 *self = (Direct3DVertexBuffer8 *)This;
    return IDirect3DVertexBuffer9_GetPriority(self->pVB9);
}

static void WINAPI Direct3DVertexBuffer8_PreLoad(IDirect3DVertexBuffer8 *This)
{
    Direct3DVertexBuffer8 *self = (Direct3DVertexBuffer8 *)This;
    IDirect3DVertexBuffer9_PreLoad(self->pVB9);
}

static D3DRESOURCETYPE WINAPI Direct3DVertexBuffer8_GetType(IDirect3DVertexBuffer8 *This)
{
    Direct3DVertexBuffer8 *self = (Direct3DVertexBuffer8 *)This;
    return IDirect3DVertexBuffer9_GetType(self->pVB9);
}

static HRESULT WINAPI Direct3DVertexBuffer8_Lock(IDirect3DVertexBuffer8 *This, UINT OffsetToLock, UINT SizeToLock, BYTE **ppbData, DWORD Flags)
{
    Direct3DVertexBuffer8 *self = (Direct3DVertexBuffer8 *)This;
    return IDirect3DVertexBuffer9_Lock(self->pVB9, OffsetToLock, SizeToLock, (void **)ppbData, Flags);
}

static HRESULT WINAPI Direct3DVertexBuffer8_Unlock(IDirect3DVertexBuffer8 *This)
{
    Direct3DVertexBuffer8 *self = (Direct3DVertexBuffer8 *)This;
    return IDirect3DVertexBuffer9_Unlock(self->pVB9);
}

static HRESULT WINAPI Direct3DVertexBuffer8_GetDesc(IDirect3DVertexBuffer8 *This, D3DVERTEXBUFFER_DESC *pDesc)
{
    Direct3DVertexBuffer8 *self = (Direct3DVertexBuffer8 *)This;
    return IDirect3DVertexBuffer9_GetDesc(self->pVB9, pDesc);
}

static IDirect3DVertexBuffer8Vtbl g_Direct3DVertexBuffer8_Vtbl = {
    Direct3DVertexBuffer8_QueryInterface,
    Direct3DVertexBuffer8_AddRef,
    Direct3DVertexBuffer8_Release,
    Direct3DVertexBuffer8_GetDevice,
    Direct3DVertexBuffer8_SetPrivateData,
    Direct3DVertexBuffer8_GetPrivateData,
    Direct3DVertexBuffer8_FreePrivateData,
    Direct3DVertexBuffer8_SetPriority,
    Direct3DVertexBuffer8_GetPriority,
    Direct3DVertexBuffer8_PreLoad,
    Direct3DVertexBuffer8_GetType,
    Direct3DVertexBuffer8_Lock,
    Direct3DVertexBuffer8_Unlock,
    Direct3DVertexBuffer8_GetDesc
};

HRESULT CreateDirect3DVertexBuffer8(Direct3DDevice8 *pDevice8, IDirect3DVertexBuffer9 *pVB9, Direct3DVertexBuffer8 **ppVB8)
{
    Direct3DVertexBuffer8 *pVB8 = (Direct3DVertexBuffer8 *)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(Direct3DVertexBuffer8));
    if (!pVB8) return E_OUTOFMEMORY;

    pVB8->lpVtbl = &g_Direct3DVertexBuffer8_Vtbl;
    pVB8->RefCount = 1;
    pVB8->pVB9 = pVB9;
    pVB8->pDevice8 = pDevice8;

    *ppVB8 = pVB8;
    return D3D_OK;
}

// ============================================================================
// IDirect3DIndexBuffer8 Implementation
// ============================================================================

static HRESULT WINAPI Direct3DIndexBuffer8_QueryInterface(IDirect3DIndexBuffer8 *This, REFIID riid, void **ppvObject)
{
    Direct3DIndexBuffer8 *self = (Direct3DIndexBuffer8 *)This;
    if (!ppvObject) return E_POINTER;

    if (IsEqualGUID(riid, &IID_IUnknown) || IsEqualGUID(riid, &IID_IDirect3DIndexBuffer8) ||
        IsEqualGUID(riid, &IID_IDirect3DResource8)) {
        self->RefCount++;
        *ppvObject = This;
        return S_OK;
    }
    *ppvObject = NULL;
    return E_NOINTERFACE;
}

static ULONG WINAPI Direct3DIndexBuffer8_AddRef(IDirect3DIndexBuffer8 *This)
{
    Direct3DIndexBuffer8 *self = (Direct3DIndexBuffer8 *)This;
    return ++self->RefCount;
}

static ULONG WINAPI Direct3DIndexBuffer8_Release(IDirect3DIndexBuffer8 *This)
{
    Direct3DIndexBuffer8 *self = (Direct3DIndexBuffer8 *)This;
    ULONG ref = --self->RefCount;
    if (ref == 0) {
        if (self->pIB9) IDirect3DIndexBuffer9_Release(self->pIB9);
        HeapFree(GetProcessHeap(), 0, self);
    }
    return ref;
}

static HRESULT WINAPI Direct3DIndexBuffer8_GetDevice(IDirect3DIndexBuffer8 *This, IDirect3DDevice8 **ppDevice)
{
    Direct3DIndexBuffer8 *self = (Direct3DIndexBuffer8 *)This;
    if (!ppDevice) return D3DERR_INVALIDCALL;
    self->pDevice8->RefCount++;
    *ppDevice = (IDirect3DDevice8 *)self->pDevice8;
    return D3D_OK;
}

static HRESULT WINAPI Direct3DIndexBuffer8_SetPrivateData(IDirect3DIndexBuffer8 *This, REFGUID refguid, const void *pData, DWORD SizeOfData, DWORD Flags)
{
    Direct3DIndexBuffer8 *self = (Direct3DIndexBuffer8 *)This;
    return IDirect3DIndexBuffer9_SetPrivateData(self->pIB9, refguid, pData, SizeOfData, Flags);
}

static HRESULT WINAPI Direct3DIndexBuffer8_GetPrivateData(IDirect3DIndexBuffer8 *This, REFGUID refguid, void *pData, DWORD *pSizeOfData)
{
    Direct3DIndexBuffer8 *self = (Direct3DIndexBuffer8 *)This;
    return IDirect3DIndexBuffer9_GetPrivateData(self->pIB9, refguid, pData, pSizeOfData);
}

static HRESULT WINAPI Direct3DIndexBuffer8_FreePrivateData(IDirect3DIndexBuffer8 *This, REFGUID refguid)
{
    Direct3DIndexBuffer8 *self = (Direct3DIndexBuffer8 *)This;
    return IDirect3DIndexBuffer9_FreePrivateData(self->pIB9, refguid);
}

static DWORD WINAPI Direct3DIndexBuffer8_SetPriority(IDirect3DIndexBuffer8 *This, DWORD PriorityNew)
{
    Direct3DIndexBuffer8 *self = (Direct3DIndexBuffer8 *)This;
    return IDirect3DIndexBuffer9_SetPriority(self->pIB9, PriorityNew);
}

static DWORD WINAPI Direct3DIndexBuffer8_GetPriority(IDirect3DIndexBuffer8 *This)
{
    Direct3DIndexBuffer8 *self = (Direct3DIndexBuffer8 *)This;
    return IDirect3DIndexBuffer9_GetPriority(self->pIB9);
}

static void WINAPI Direct3DIndexBuffer8_PreLoad(IDirect3DIndexBuffer8 *This)
{
    Direct3DIndexBuffer8 *self = (Direct3DIndexBuffer8 *)This;
    IDirect3DIndexBuffer9_PreLoad(self->pIB9);
}

static D3DRESOURCETYPE WINAPI Direct3DIndexBuffer8_GetType(IDirect3DIndexBuffer8 *This)
{
    Direct3DIndexBuffer8 *self = (Direct3DIndexBuffer8 *)This;
    return IDirect3DIndexBuffer9_GetType(self->pIB9);
}

static HRESULT WINAPI Direct3DIndexBuffer8_Lock(IDirect3DIndexBuffer8 *This, UINT OffsetToLock, UINT SizeToLock, BYTE **ppbData, DWORD Flags)
{
    Direct3DIndexBuffer8 *self = (Direct3DIndexBuffer8 *)This;
    return IDirect3DIndexBuffer9_Lock(self->pIB9, OffsetToLock, SizeToLock, (void **)ppbData, Flags);
}

static HRESULT WINAPI Direct3DIndexBuffer8_Unlock(IDirect3DIndexBuffer8 *This)
{
    Direct3DIndexBuffer8 *self = (Direct3DIndexBuffer8 *)This;
    return IDirect3DIndexBuffer9_Unlock(self->pIB9);
}

static HRESULT WINAPI Direct3DIndexBuffer8_GetDesc(IDirect3DIndexBuffer8 *This, D3DINDEXBUFFER_DESC *pDesc)
{
    Direct3DIndexBuffer8 *self = (Direct3DIndexBuffer8 *)This;
    return IDirect3DIndexBuffer9_GetDesc(self->pIB9, pDesc);
}

static IDirect3DIndexBuffer8Vtbl g_Direct3DIndexBuffer8_Vtbl = {
    Direct3DIndexBuffer8_QueryInterface,
    Direct3DIndexBuffer8_AddRef,
    Direct3DIndexBuffer8_Release,
    Direct3DIndexBuffer8_GetDevice,
    Direct3DIndexBuffer8_SetPrivateData,
    Direct3DIndexBuffer8_GetPrivateData,
    Direct3DIndexBuffer8_FreePrivateData,
    Direct3DIndexBuffer8_SetPriority,
    Direct3DIndexBuffer8_GetPriority,
    Direct3DIndexBuffer8_PreLoad,
    Direct3DIndexBuffer8_GetType,
    Direct3DIndexBuffer8_Lock,
    Direct3DIndexBuffer8_Unlock,
    Direct3DIndexBuffer8_GetDesc
};

HRESULT CreateDirect3DIndexBuffer8(Direct3DDevice8 *pDevice8, IDirect3DIndexBuffer9 *pIB9, Direct3DIndexBuffer8 **ppIB8)
{
    Direct3DIndexBuffer8 *pIB8 = (Direct3DIndexBuffer8 *)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(Direct3DIndexBuffer8));
    if (!pIB8) return E_OUTOFMEMORY;

    pIB8->lpVtbl = &g_Direct3DIndexBuffer8_Vtbl;
    pIB8->RefCount = 1;
    pIB8->pIB9 = pIB9;
    pIB8->pDevice8 = pDevice8;

    *ppIB8 = pIB8;
    return D3D_OK;
}

// ============================================================================
// IDirect3DSwapChain8 Implementation
// ============================================================================

static HRESULT WINAPI Direct3DSwapChain8_QueryInterface(IDirect3DSwapChain8 *This, REFIID riid, void **ppvObject)
{
    Direct3DSwapChain8 *self = (Direct3DSwapChain8 *)This;
    if (!ppvObject) return E_POINTER;

    if (IsEqualGUID(riid, &IID_IUnknown) || IsEqualGUID(riid, &IID_IDirect3DSwapChain8)) {
        self->RefCount++;
        *ppvObject = This;
        return S_OK;
    }
    *ppvObject = NULL;
    return E_NOINTERFACE;
}

static ULONG WINAPI Direct3DSwapChain8_AddRef(IDirect3DSwapChain8 *This)
{
    Direct3DSwapChain8 *self = (Direct3DSwapChain8 *)This;
    return ++self->RefCount;
}

static ULONG WINAPI Direct3DSwapChain8_Release(IDirect3DSwapChain8 *This)
{
    Direct3DSwapChain8 *self = (Direct3DSwapChain8 *)This;
    ULONG ref = --self->RefCount;
    if (ref == 0) {
        if (self->pSwapChain9) IDirect3DSwapChain9_Release(self->pSwapChain9);
        HeapFree(GetProcessHeap(), 0, self);
    }
    return ref;
}

static HRESULT WINAPI Direct3DSwapChain8_Present(IDirect3DSwapChain8 *This, const RECT *pSourceRect, const RECT *pDestRect, HWND hDestWindowOverride, const RGNDATA *pDirtyRegion)
{
    Direct3DSwapChain8 *self = (Direct3DSwapChain8 *)This;
    return IDirect3DSwapChain9_Present(self->pSwapChain9, pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion, 0);
}

static HRESULT WINAPI Direct3DSwapChain8_GetBackBuffer(IDirect3DSwapChain8 *This, UINT BackBuffer, D3DBACKBUFFER_TYPE Type, IDirect3DSurface8 **ppBackBuffer)
{
    Direct3DSwapChain8 *self = (Direct3DSwapChain8 *)This;
    if (!ppBackBuffer) return D3DERR_INVALIDCALL;

    IDirect3DSurface9 *pSurface9 = NULL;
    HRESULT hr = IDirect3DSwapChain9_GetBackBuffer(self->pSwapChain9, BackBuffer, Type, &pSurface9);

    if (SUCCEEDED(hr)) {
        hr = CreateDirect3DSurface8(self->pDevice8, pSurface9, (Direct3DSurface8 **)ppBackBuffer);
        if (FAILED(hr)) {
            IDirect3DSurface9_Release(pSurface9);
        }
    }
    return hr;
}

static IDirect3DSwapChain8Vtbl g_Direct3DSwapChain8_Vtbl = {
    Direct3DSwapChain8_QueryInterface,
    Direct3DSwapChain8_AddRef,
    Direct3DSwapChain8_Release,
    Direct3DSwapChain8_Present,
    Direct3DSwapChain8_GetBackBuffer
};

HRESULT CreateDirect3DSwapChain8(Direct3DDevice8 *pDevice8, IDirect3DSwapChain9 *pSwapChain9, Direct3DSwapChain8 **ppSwapChain8)
{
    Direct3DSwapChain8 *pSwapChain8 = (Direct3DSwapChain8 *)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(Direct3DSwapChain8));
    if (!pSwapChain8) return E_OUTOFMEMORY;

    pSwapChain8->lpVtbl = &g_Direct3DSwapChain8_Vtbl;
    pSwapChain8->RefCount = 1;
    pSwapChain8->pSwapChain9 = pSwapChain9;
    pSwapChain8->pDevice8 = pDevice8;

    *ppSwapChain8 = pSwapChain8;
    return D3D_OK;
}

// Stub implementations for CubeTexture and VolumeTexture
HRESULT CreateDirect3DCubeTexture8(Direct3DDevice8 *pDevice8, IDirect3DCubeTexture9 *pCubeTexture9, Direct3DCubeTexture8 **ppCubeTexture8)
{
    (void)pDevice8; (void)pCubeTexture9; (void)ppCubeTexture8;
    // TODO: Implement full cube texture wrapper
    return E_NOTIMPL;
}

HRESULT CreateDirect3DVolumeTexture8(Direct3DDevice8 *pDevice8, IDirect3DVolumeTexture9 *pVolumeTexture9, Direct3DVolumeTexture8 **ppVolumeTexture8)
{
    (void)pDevice8; (void)pVolumeTexture9; (void)ppVolumeTexture8;
    // TODO: Implement full volume texture wrapper
    return E_NOTIMPL;
}
