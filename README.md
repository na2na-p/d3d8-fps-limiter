# D3D8 to D3D9 Converter

A DirectX 8 to DirectX 9 wrapper that allows old D3D8 games to run using the D3D9 API internally.

## Features

- Full D3D8 API translation to D3D9
- Works with any D3D8 game
- Transparent drop-in replacement
- Support for:
  - Textures (2D, Cube, Volume)
  - Surfaces
  - Vertex/Index Buffers
  - Render Targets and Depth Stencils
  - Vertex and Pixel Shaders (SM 1.x)
  - State Blocks
  - All render states with proper D3D8→D3D9 mapping

## Why Use This?

- Better driver support: Modern GPUs have better D3D9 support than D3D8
- Compatibility: Some modern systems have issues with native D3D8
- Feature enhancement: Enables use of D3D9-based tools (ReShade, etc.)
- Performance: D3D9 drivers are typically more optimized

## Usage

1. Copy `d3d8.dll` to your game folder (next to the game .exe)
2. Launch the game

The wrapper will automatically load the system's D3D9 and translate all D3D8 calls.

## Building

### Windows (MSVC)

```cmd
mkdir build
cd build
cmake .. -A Win32
cmake --build . --config Release
```

### Windows (MinGW)

```cmd
mkdir build
cd build
cmake .. -G "MinGW Makefiles" -DCMAKE_C_FLAGS="-m32"
cmake --build .
```

**Note:** Must be built as 32-bit to work with D3D8 games.

## Technical Details

### D3D8 to D3D9 API Differences Handled

| D3D8 | D3D9 Equivalent |
|------|-----------------|
| `D3DSWAPEFFECT_COPY_VSYNC` | `D3DSWAPEFFECT_COPY` |
| `D3DRS_ZBIAS` | `D3DRS_DEPTHBIAS` (with scale conversion) |
| `D3DRS_SOFTWAREVERTEXPROCESSING` | `SetSoftwareVertexProcessing()` |
| Texture Stage Address/Filter states | Sampler States |
| `CopyRects()` | `UpdateSurface()` / `StretchRect()` |
| `CreateImageSurface()` | `CreateOffscreenPlainSurface()` |
| `GetFrontBuffer()` | `GetFrontBufferData()` |
| State block tokens (DWORD) | `IDirect3DStateBlock9` objects |
| Shader handles (DWORD) | `IDirect3DVertexShader9` / `IDirect3DPixelShader9` |

### Shader Version Clamping

D3D9 reports higher shader versions than D3D8 supports. The wrapper clamps:
- Vertex Shader: Max 1.1 (`0xFFFE0101`)
- Pixel Shader: Max 1.4 (`0xFFFF0104`)

### Architecture

```
Game (D3D8 API calls)
        │
        ▼
   d3d8.dll (this wrapper)
        │
        ├── Direct3D8 wrapper → IDirect3D9
        ├── Device8 wrapper   → IDirect3DDevice9
        ├── Surface8 wrapper  → IDirect3DSurface9
        ├── Texture8 wrapper  → IDirect3DTexture9
        ├── Buffer wrappers   → IDirect3DBuffer9
        └── ...
        │
        ▼
   System d3d9.dll (loaded at runtime)
```

## File Structure

```
d3d8.h              - D3D8 interface definitions
d3d8types.h         - D3D8 type definitions
d3d8to9_base.h      - Wrapper structure definitions and conversion helpers
d3d8to9_d3d8.c      - IDirect3D8 implementation
d3d8to9_device.c    - IDirect3DDevice8 implementation (part 1)
d3d8to9_device2.c   - IDirect3DDevice8 implementation (part 2)
d3d8to9_resources.c - Surface, Texture, Buffer wrappers
d3d8.def            - DLL export definitions
CMakeLists.txt      - Build configuration
```

## Known Limitations

- Cube textures and volume textures have stub implementations
- `GetInfo()` is not supported (D3D9 removed this method)
- `GetVertexShaderDeclaration()` is not fully implemented
- Some D3D8-only render states are silently ignored

## License

Public Domain / CC0
