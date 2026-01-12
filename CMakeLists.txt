cmake_minimum_required(VERSION 3.15)
project(d3d8_wrapper C)

set(CMAKE_C_STANDARD 99)

# 32-bit build required for D3D8 games
if(NOT CMAKE_SIZEOF_VOID_P EQUAL 4)
    message(WARNING "This should be built as 32-bit. Use -A Win32 or appropriate toolchain.")
endif()

add_library(d3d8 SHARED
    main.c
    d3d8_wrapper.def
)

target_link_libraries(d3d8 PRIVATE
    winmm
)

# No lib prefix on Windows
set_target_properties(d3d8 PROPERTIES PREFIX "")
