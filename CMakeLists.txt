cmake_minimum_required(VERSION 3.16)

project(matrix LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Create an option to switch between a system sdl library and a vendored SDL library
option(MATRIX_VENDORED "Use vendored libraries" OFF)

if(MATRIX_VENDORED)
    # This assumes you have added SDL as a submodule in vendored/SDL
    add_subdirectory(vendored/SDL EXCLUDE_FROM_ALL)
else()
    # 1. Look for a SDL3 package,
    # 2. look for the SDL3-shared component, and
    # 3. fail if the shared component cannot be found.
    find_package(SDL3 REQUIRED CONFIG REQUIRED COMPONENTS SDL3-shared)
endif()

add_executable(matrix
    src/main.cpp
    src/globals.cpp

    src/core/simulation.cpp

    src/renderer/renderer.cpp
)

target_include_directories(matrix
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(matrix PRIVATE SDL3::SDL3)