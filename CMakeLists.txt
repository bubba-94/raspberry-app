cmake_minimum_required(VERSION 3.15)
project(SDLApp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17-Wall -Wextra -g)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -g")

# Directories
set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")
message(STATUS "CMAKE_CURRENT_SOURCE_DIR = ${CMAKE_CURRENT_SOURCE_DIR}")

# Find SDL2 (creates INCLUDE_DIRS if found)
find_package(SDL2 REQUIRED)

# SDL Window Wrapper library 
message(STATUS "Adding library WindowLib")
add_library(WindowLib 
    STATIC 
        ${SRC_DIR}/Window.cpp
)
target_include_directories(WindowLib 
    PUBLIC 
        ${INCLUDE_DIR}
)
target_link_libraries(WindowLib 
    PUBLIC 
        SDL2::SDL2
)
# SDL Event wrapper library
message(STATUS "Adding library EventManagerLib")
add_library(EventManagerLib 
    STATIC 
        ${SRC_DIR}/EventManager.cpp
)
target_include_directories(EventManagerLib 
    PUBLIC 
        ${INCLUDE_DIR}
)
target_link_libraries(EventManagerLib 
    PUBLIC 
        SDL2::SDL2
)
message(STATUS "Adding library RendererLib")
add_library(RendererLib 
    STATIC 
        ${SRC_DIR}/Renderer.cpp
)
target_include_directories(RendererLib 
    PUBLIC 
        ${INCLUDE_DIR}
)
target_link_libraries(RendererLib 
    PUBLIC 
        SDL2::SDL2
)

message(STATUS "Adding executables")

# Executable
add_executable(sdl2-x86 main.cpp)
target_link_libraries(sdl2-x86 
    PRIVATE 
        WindowLib
        EventManagerLib
        RendererLib
) 

# Set the output directory for this executable
set_target_properties(sdl2-x86 PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/x86"
)