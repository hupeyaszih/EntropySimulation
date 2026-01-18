cmake_minimum_required(VERSION 3.16)

project(EntropySimulation LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Create an option to switch between a system sdl library and a vendored SDL library
option(EntropySimulation_VENDORED "Use vendored libraries" OFF)

if(EntropySimulation_VENDORED)
    add_subdirectory(vendored/sdl EXCLUDE_FROM_ALL)
else()
    # 1. Look for a SDL2 package, 2. look for the SDL2 component and 3. fail if none can be found
    find_package(SDL2 REQUIRED CONFIG REQUIRED COMPONENTS SDL2)

    # 1. Look for a SDL2 package, 2. Look for the SDL2maincomponent and 3. DO NOT fail when SDL2main is not available
    find_package(SDL2 REQUIRED CONFIG COMPONENTS SDL2main)
endif()


add_executable(EntropySimulation
    src/main.cpp
    src/globals.cpp

    src/core/simulation.cpp
    src/core/map.cpp

    src/renderer/renderer.cpp
)

target_include_directories(EntropySimulation
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# SDL2::SDL2main may or may not be available. It is e.g. required by Windows GUI applications
if(TARGET SDL2::SDL2main)
    # It has an implicit dependency on SDL2 functions, so it MUST be added before SDL2::SDL2 (or SDL2::SDL2-static)
    target_link_libraries(EntropySimulation PRIVATE SDL2::SDL2main)
endif()

# Link to the actual SDL2 library. SDL2::SDL2 is the shared SDL library, SDL2::SDL2-static is the static SDL libarary.
target_link_libraries(EntropySimulation PRIVATE SDL2::SDL2)