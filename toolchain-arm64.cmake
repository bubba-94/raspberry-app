# sysrootcpy-pi5/toolchain.cmake

# Set to 1 because Linux systems are pretty generic???
set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR aarch64)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -g")

# Path to your sysroot
set(SYSROOT "${CMAKE_CURRENT_LIST_DIR}/sysrootcpy-pi5")
set(ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR})

# Cross compiler
set(CMAKE_C_COMPILER aarch64-linux-gnu-gcc)
set(CMAKE_CXX_COMPILER aarch64-linux-gnu-g++)

# Find libraries in the sysroot
set(CMAKE_FIND_ROOT_PATH 
    "${SYSROOT}/usr; ${SYSROOT}/lib"
)

# Search only in sysroot for programs, libraries, includes
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

# Source files directoies 
set(OUTPUT_DIR_ARM "${ROOT_DIR}/bin/arm64")
set(SRC_DIR "${ROOT_DIR}/src")
set(INCLUDE_DIR "${ROOT_DIR}/include")

# Coloring for status
set(CMAKE_COLOR_DIAGNOSTICS ON)
string(ASCII 27 Esc)
set(GREEN   "${Esc}[1;32m")
set(RESET       "${Esc}[0m")
set(STARTCOLOR  "${GREEN}------------------------------")
set(ENDCOLOR    "------------------------------${RESET}")

message(STATUS ${STARTCOLOR})
message(STATUS "SETTING ARM CMAKE VARIABLES")
message(STATUS "C++ compiler            = ${CMAKE_CXX_COMPILER}")
message(STATUS "C++ standard            = ${CMAKE_CXX_STANDARD}")
message(STATUS "Compiler flags          = ${CMAKE_CXX_FLAGS}")
message(STATUS "Target OS               = ${CMAKE_SYSTEM_NAME}")
message(STATUS "Target processor        = ${CMAKE_SYSTEM_PROCESSOR}")
message(STATUS "Target root path        = ${CMAKE_FIND_ROOT_PATH}")
message(STATUS "Source files directory  = ${SRC_DIR}" )
message(STATUS "Headers directory       = ${INCLUDE_DIR}")
message(STATUS "Output location         = ${OUTPUT_DIR_ARM}")
message(STATUS ${ENDCOLOR})