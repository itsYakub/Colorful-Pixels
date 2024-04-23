# This CMakeLists.txt file is a modification of already existing CMake file from the raylib's repository:
# https://github.com/raysan5/raylib/tree/master/projects/CMake

cmake_minimum_required(VERSION 3.14)

project(
    colorful_pixels 
    VERSION 1.0
    DESCRIPTION "A simple and easy-to-use pixel-art editor for simplicity enjoyers." 
    HOMEPAGE_URL "https://github.com/itsYakub/pixelart-editor"
    LANGUAGES CXX C
)

# Generate compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Dependencies
set(RAYLIB_VERSION 5.0)
include(FetchContent)
    FetchContent_Declare(
        raylib
        DOWNLOAD_EXTRACT_TIMESTAMP OFF
        URL https://github.com/raysan5/raylib/archive/refs/tags/${RAYLIB_VERSION}.tar.gz
    )
    FetchContent_GetProperties(raylib)

    if (NOT raylib_POPULATED) # Have we downloaded raylib yet?

        set(FETCHCONTENT_QUIET NO)
        FetchContent_Populate(raylib)
        set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # don't build the supplied examples
        add_subdirectory(${raylib_SOURCE_DIR} ${raylib_BINARY_DIR})
        
    endif()

# Our Project

# Project's source files:
# Edit this variable to include all the source files (*.c, *.cpp, *.cc etc.) that the project requires

file(
    GLOB SOURCES 

    ${CMAKE_SOURCE_DIR}/src/*.cpp
    ${CMAKE_SOURCE_DIR}/lib/rlImGui/*.cpp
    ${CMAKE_SOURCE_DIR}/lib/imgui/*.cpp
)

# Project's include directories:
# Edit this variable to include all the directories with necessary header files

set(
    INCLUDE_DIRECTORIES
    ${CMAKE_SOURCE_DIR}/include # default project include directory
    ${CMAKE_SOURCE_DIR}/lib/raylib/src # raysan5/raylib
    ${CMAKE_SOURCE_DIR}/lib/imgui
    ${CMAKE_SOURCE_DIR}/lib/rlImGui
)

add_executable(${PROJECT_NAME} ${SOURCES})
#set(raylib_VERBOSE 1)
target_link_libraries(${PROJECT_NAME} raylib)
target_include_directories(${PROJECT_NAME} PUBLIC ${INCLUDE_DIRECTORIES})

# Web Configurations
if (${PLATFORM} STREQUAL "Web")

    # Tell Emscripten to build a final executable
    set(
        CMAKE_EXE_LINKER_FLAGS 
        "${CMAKE_EXE_LINKER_FLAGS} -s USE_GLFW=3 -s WASM=1 -s TOTAL_MEMORY=67108864 -s FORCE_FILESYSTEM=1 -Os -Wall -Wextra --shell-file ../lib/raylib/src/minshell.html"
    )

    set_target_properties(${PROJECT_NAME} PROPERTIES SUFFIX ".html")

endif()

# Checks if OSX and links appropriate frameworks (Only required on MacOS)
if (APPLE)

    target_link_libraries(${PROJECT_NAME} "-framework IOKit")
    target_link_libraries(${PROJECT_NAME} "-framework Cocoa")
    target_link_libraries(${PROJECT_NAME} "-framework OpenGL")

endif()