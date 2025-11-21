cmake_minimum_required(VERSION 3.15)
project(SDL2APP LANGUAGES CXX)

# Config for cmake
include(config.cmake)

# Find SDL2 (creates INCLUDE_DIRS if found)
find_package(SDL2 REQUIRED)

# Include src/CMakeLists.txt
add_subdirectory(${SRC_DIR} sdl2-archive)

# Executable
add_executable(sdl2-x86 main.cpp)

# Link the library defined in src/CMakeLists.txt
target_link_libraries(sdl2-x86 PRIVATE ${ARCHIVE}) 
