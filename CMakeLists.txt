# This cmake file does not actually build anything. It is only for CLion to understand imports.

cmake_minimum_required(VERSION 3.27)
project(game1)

set(CMAKE_CXX_STANDARD 14)

include_directories(
        ../../..
        ../nest-libs/macos/SDL2/include/SDL2
        ../nest-libs/macos/freetype/include
        ../nest-libs/macos/glm/include
        ../nest-libs/macos/harfbuzz/include
        ../nest-libs/macos/libogg/include
        ../nest-libs/macos/libopus/include
        ../nest-libs/macos/libopusenc/include
        ../nest-libs/macos/libpng/include
        ../nest-libs/macos/opus-tools/include
        ../nest-libs/macos/opusfile/include
        ../nest-libs/macos/zlib/include
)

add_executable(game1
        data_path.cpp
        data_path.hpp
        GL.cpp
        GL.hpp
        gl_compile_program.cpp
        gl_compile_program.hpp
        gl_errors.hpp
        glcorearb.h
        Load.cpp
        Load.hpp
        load_save_png.cpp
        load_save_png.hpp
        main.cpp
        Mode.cpp
        Mode.hpp
        PlayMode.cpp
        PlayMode.hpp
        PPU466.cpp
        PPU466.hpp
        read_write_chunk.hpp)
