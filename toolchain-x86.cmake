# Configuratio for bulding just testing


set(CMAKE_CXX_COMPILER g++)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -g")

# Source directory to navigate from
set(ARCHIVE _sdl2x86)

# Directories
set(OUTPUT_DIR_X86 "${CMAKE_CURRENT_SOURCE_DIR}/bin/x86")
set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")
set(INFO_SHOWN FALSE)

# Target output
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIR_X86})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${OUTPUT_DIR_X86})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${OUTPUT_DIR_X86})

set(CMAKE_COLOR_DIAGNOSTICS ON)
string(ASCII 27 Esc)
set(YELLOW      "${Esc}[1;33m")
set(RESET       "${Esc}[0m")
set(STARTCOLOR  "${YELLOW}------------------------------")
set(ENDCOLOR    "------------------------------${RESET}")

message(STATUS ${STARTCOLOR})
message(STATUS "x86 Toolchain File")
message(STATUS "C++ compiler            = ${CMAKE_CXX_COMPILER}")
message(STATUS "C++ standard            = ${CMAKE_CXX_STANDARD}")
message(STATUS "Compiler flags          = ${CMAKE_CXX_FLAGS}")
message(STATUS "Static library filename = lib${ARCHIVE}.a")
message(STATUS "Binary output directory = ${OUTPUT_DIR_X86}")
message(STATUS "Source file directory   = ${SRC_DIR}")
message(STATUS "Header file diretory    = ${INCLUDE_DIR}")
message(STATUS ${ENDCOLOR})


set(CMAKE_COLOR_DIAGNOSTICS OFF)
