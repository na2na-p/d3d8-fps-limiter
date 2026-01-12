cmake_minimum_required(VERSION 3.15)
project(d3d8to9 C)

set(CMAKE_C_STANDARD 99)

# 32-bit build required for D3D8 games
if(NOT CMAKE_SIZEOF_VOID_P EQUAL 4)
    message(WARNING "This should be built as 32-bit. Use -A Win32 or appropriate toolchain.")
endif()

# D3D8 to D3D9 Converter
add_library(d3d8 SHARED
    d3d8to9_d3d8.c
    d3d8to9_device.c
    d3d8to9_device2.c
    d3d8to9_resources.c
    d3d8.def
)

# Link against d3d9 (loaded dynamically at runtime)
# No additional libraries needed - we load d3d9.dll at runtime

# No lib prefix on Windows
set_target_properties(d3d8 PROPERTIES PREFIX "")

# Compiler flags
if(MSVC)
    target_compile_options(d3d8 PRIVATE /W4)
else()
    target_compile_options(d3d8 PRIVATE -Wall -Wextra)
endif()
