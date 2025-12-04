cmake_minimum_required(VERSION 3.15)
project(SDL2APP LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -g")

set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")

if(CMAKE_SYSTEM_PROCESSOR MATCHES "aarch64")
    set(ARCHIVE _sdl2pi5)
else()
    set(ARCHIVE _sdl2x86)
endif()

if (CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64")

    set(OUTPUT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/bin/x86")

    message(STATUS ${STARTYELLOW})
    
    if(NOT DEFINED TOOLCHAIN_PRINTED)
        # Prints when building for debugg
        message(STATUS "CMakeLists.txt output for x86")
        message(STATUS "C++ compiler            = ${CMAKE_CXX_COMPILER}")
        message(STATUS "C++ standard            = ${CMAKE_CXX_STANDARD}")
        message(STATUS "Compiler flags          = ${CMAKE_CXX_FLAGS}")
        message(STATUS "Static library filename = lib${ARCHIVE}.a")
        message(STATUS "Source file directory   = ${SRC_DIR}")
        message(STATUS "Header file diretory    = ${INCLUDE_DIR}")
        message(STATUS "Binary output directory = ${OUTPUT_DIR}")
        set(TOOLCHAIN_PRINTED TRUE CACHE INTERNAL "Prevent toolchain output twice")
    endif()

    find_package(SDL2 REQUIRED)
    find_package(SDL2_image REQUIRED)
    find_package(SDL2_ttf REQUIRED)


    message(STATUS ${ENDYELLOW})

elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "aarch64")

    # Locate _real_config.h
    find_path(SDL2_INCLUDE_DIR
        SDL.h
        HINTS 
        "${SYSROOT}/usr/include/SDL2/"
        "${SYSROOT}/usr/include/aarch64-linux-gnu/SDL2/"
    )

    find_library(SDL2_LIBRARY
        NAMES SDL2
        HINTS "${SYSROOT}/usr/lib"
    )
    
    find_path(SDL2_IMAGE_INCLUDE_DIR
        SDL_image.h
        HINTS 
        "${SYSROOT}/usr/include/SDL2/"
        "${SYSROOT}/usr/include/aarch64-linux-gnu/SDL2/"
    )

    find_library(SDL2_IMAGE_LIBRARY
        NAMES SDL2_image
        HINTS "${SYSROOT}/usr/lib"
    )

    # SDL2 TTF library for fonts
    find_path(SDL2_TTF_INCLUDE_DIR
        SDL_ttf.h
        HINTS 
        "${SYSROOT}/usr/include/SDL2/"
        "${SYSROOT}/usr/include/aarch64-linux-gnu/SDL2/"
    )

    find_library(SDL2_TTF_LIBRARY
        NAMES SDL2_ttf
        HINTS "${SYSROOT}/usr/lib"
    )

    # Create imported target
    add_library(SDL2::SDL2 STATIC IMPORTED)
    set_target_properties(SDL2::SDL2 PROPERTIES
        IMPORTED_LOCATION "${SDL2_LIBRARY}"
        INTERFACE_INCLUDE_DIRECTORIES "${SDL2_INCLUDE_DIR}"
    )

    add_library(SDL2_image::SDL2_image STATIC IMPORTED)
    set_target_properties(SDL2_image::SDL2_image PROPERTIES
        IMPORTED_LOCATION ${SDL2_IMAGE_LIBRARY}
        INTERFACE_INCLUDE_DIRECTORIES ${SDL2_IMAGE_INCLUDE_DIR}
    )

    add_library(SDL2_ttf::SDL2_ttf STATIC IMPORTED)
    set_target_properties(SDL2_ttf::SDL2_ttf PROPERTIES
        IMPORTED_LOCATION ${SDL2_TTF_LIBRARY}
        INTERFACE_INCLUDE_DIRECTORIES ${SDL2_TTF_INCLUDE_DIR}
    )
    
    set(OUTPUT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/bin/aarch64")
    
    message(STATUS ${STARTGREEN})
    
    if(NOT DEFINED TOOLCHAIN_PRINTED)

        # Prints when building for debugging
        message(STATUS "CMakeLists.txt output for aarch64")
        message(STATUS "C++ compiler            = ${CMAKE_CXX_COMPILER}")
        message(STATUS "C++ standard            = ${CMAKE_CXX_STANDARD}")
        message(STATUS "Compiler flags          = ${CMAKE_CXX_FLAGS}")
        message(STATUS "Target OS               = ${CMAKE_SYSTEM_NAME}")
        message(STATUS "Target processor        = ${CMAKE_SYSTEM_PROCESSOR}")
        message(STATUS "Sysroot                 = ${SYSROOT}")
        message(STATUS "Source files directory  = ${SRC_DIR}" )
        message(STATUS "Headers directory       = ${INCLUDE_DIR}")
        message(STATUS "Binary output directory = ${OUTPUT_DIR}")
        message(STATUS "SDL2 sysroot lib        = ${SDL2_LIBRARY}")
        message(STATUS "SDL2 include dir        = ${SDL2_INCLUDE_DIR}")
        
        set(TOOLCHAIN_PRINTED TRUE CACHE INTERNAL "Prevent toolchain print twice")
    endif()

    # Make CMake look in sysroot for SDL2
    set(CMAKE_PREFIX_PATH "${SYSROOT}/usr/lib/cmake/SDL2" ${CMAKE_PREFIX_PATH})

    message(STATUS ${ENDGREEN})
endif() 

# Target output
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${OUTPUT_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${OUTPUT_DIR})

# Include src/CMakeLists.txt
add_subdirectory(${SRC_DIR} sdl2-archive)

# Executable
add_executable(sdl2-app main.cpp)

# Link the library defined in src/CMakeLists.txt
target_link_libraries(sdl2-app PRIVATE ${ARCHIVE}) 