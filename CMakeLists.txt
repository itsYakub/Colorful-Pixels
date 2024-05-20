# ------------------------------------------------------------------------------
# RaylibGameTemplate
# https://github.com/itsYakub/RaylibGameTemplate.git
# ------------------------------------------------------------------------------
# Author:
# https://github.com/itsYakub
# ------------------------------------------------------------------------------
# Naming convention:
# - *.cpp - source file extension
# - *.hpp - header file extension
# - src/ - source files directory
# - lib/ - libraries directory
# - include/ - header files direcotry
# - res/ - resources / assets directory
# - bin/ - binary / final exec. directory
# ------------------------------------------------------------------------------
# LICENCE (MIT):
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
# 
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
# 
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
# EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
# MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
# IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
# DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
# OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE
# OR OTHER DEALINGS IN THE SOFTWARE.
# ------------------------------------------------------------------------------
# Most parts of this CMakeLists.txt file comes from this source:
# https://github.com/raysan5/raylib/tree/master/projects/CMake
# ------------------------------------------------------------------------------

# ----------------------
# Section: Project setup
# ----------------------

cmake_minimum_required(VERSION 3.14)

project(
    colorful_pixels 
    VERSION 1.0
    DESCRIPTION "A simple and easy-to-use pixel-art editor." 
    HOMEPAGE_URL "https://github.com/itsYakub/pixelart-editor"
    LANGUAGES CXX C
)

# Generate compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(raylib_VERBOSE 1)

# ---------------------
# Section: Dependencies
# ---------------------

include(FetchContent)

FetchContent_Declare(
    raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib.git
    GIT_TAG 5.0
)

FetchContent_MakeAvailable(raylib)

# ---------------------------------
# Section: Source files and Headers
# ---------------------------------

# SOURCES: Source files
file(
    GLOB SOURCES 

    ${CMAKE_SOURCE_DIR}/src/*.cpp
    ${CMAKE_SOURCE_DIR}/lib/rlImGui/*.cpp
    ${CMAKE_SOURCE_DIR}/lib/imgui/*.cpp
    ${CMAKE_SOURCE_DIR}/lib/libtinyfiledialogs/*.c
)

# INCLUDE_DIRECTORIES: Header file directories
set(
    INCLUDE_DIRECTORIES
    ${CMAKE_SOURCE_DIR}/include # default project include directory
    ${CMAKE_SOURCE_DIR}/lib/raylib/src # raysan5/raylib
    ${CMAKE_SOURCE_DIR}/lib/imgui # ocornut/imgui
    ${CMAKE_SOURCE_DIR}/lib/rlImGui # raylib-extras/rlImGui
    ${CMAKE_SOURCE_DIR}/lib/json/single_include # nlohmann/json
    ${CMAKE_SOURCE_DIR}/lib/libtinyfiledialogs # native-toolkit/libtinyfiledialogs
    ${CMAKE_SOURCE_DIR}/vendor # vendor files
)

# ---------------------
# Section: Target setup
# ---------------------

add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} raylib)
target_include_directories(${PROJECT_NAME} PUBLIC ${INCLUDE_DIRECTORIES})

# ----------------------------------
# Section: Compiler & Linker options
# ----------------------------------

if (${PLATFORM} STREQUAL "Web")

    # WEB: Telling the Emscirpten how to link our final program.
    # -s USE_GLFW=3 - application uses GLFW 3.x, so link to it.
    # -s WASM=1 - compile to webassembly.
    # -s TOTAL_MEMORY=67108864 - specify a heap size in bytes (default: 67108864 -> 64mb)
    # -s FORCE_FILESYSTEM=1 - force filesystem creation to load/save files data.
    # --shell-file ../lib/raylib/src/minshell.html - all WASM applications need a shell. Here we're providing a minimal shell that comes with raylib (can be found in: raylib/src/minshell.html).
    # --preload-file ../res - load an assets from the specified path and store them as ".data" file (default: ../res).
    # --profiling - enables code profiling for your project.
    # (Sources: https://github.com/raysan5/raylib/wiki/Working-for-Web-(HTML5)#5-compile-raylib-game-for-web, https://emscripten.org/index.html)
    set(
        CMAKE_EXE_LINKER_FLAGS 
        "${CMAKE_EXE_LINKER_FLAGS} -s USE_GLFW=3 -s WASM=1 -s TOTAL_MEMORY=67108864 -s FORCE_FILESYSTEM=1 -Os -Wall -Wextra --shell-file ../lib/raylib/src/minshell.html --preload-file ../res"
    )

    set_target_properties(${PROJECT_NAME} PROPERTIES SUFFIX ".html")

endif()

if (LINUX)

    # LINUX: Telling the linker to statically link the libgcc and libstdc++ to out project.
    # (Source: https://gcc.gnu.org/onlinedocs/gcc/Link-Options.html)
    target_link_options(${PROJECT_NAME} PRIVATE -static-libgcc -static-libstdc++)

endif()

if (WIN32)

    # WINDOWS: Telling the compiler to build a window application (If you want to make a console application, change this option to: `-mconsole`).
    # (Source: https://gcc.gnu.org/onlinedocs/gcc/Cygwin-and-MinGW-Options.html)
    target_compile_options(${PROJECT_NAME} PRIVATE -mwindows)

    # WINDOWS: Telling the linker to statically link our application.
    # (Source: https://gcc.gnu.org/onlinedocs/gcc/Link-Options.html)
    target_link_options(${PROJECT_NAME} PRIVATE -static)

endif()

if (APPLE)

    # APPLE: Telling the linker to link to: IOKit - Access hardware devices and drivers from your apps and services.
    # (Source: https://developer.apple.com/documentation/iokit)
    target_link_libraries(${PROJECT_NAME} "-framework IOKit")

    # APPLE: Telling the linker to link to: Cocoa - native object-oriented application programming interface.
    # (Source: https://en.wikipedia.org/wiki/Cocoa_(API))
    target_link_libraries(${PROJECT_NAME} "-framework Cocoa")

    # APPLE: Telling the linker to link to: OpenGL - cross-language, cross-platform application programming interface for rendering 2D and 3D vector graphics.
    # (Source: https://en.wikipedia.org/wiki/OpenGL)
    target_link_libraries(${PROJECT_NAME} "-framework OpenGL")

endif()