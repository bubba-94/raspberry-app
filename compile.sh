#! /bin/sh

PI_DIR=build-aarch_64
PC_DIR=build-x86_64

ROOT_DIR=$(pwd)

# Build for x86 first

if [ ! -d "$PC_DIR" ]; then
    echo "Creating build directory: $PC_DIR"
    mkdir "$PC_DIR"
else
    echo "Build directory already exists: $PC_DIR"
fi

if [ -f "$PC_DIR/CMakeCache.txt" ]; then
    echo "Cleaning x86 CMake cache..."
    rm -rf "$PC_DIR/CMakeCache.txt" "$PC_DIR/CMakeFiles"
fi

# Make sure dir exists
mkdir -p "$PC_DIR"

# Find directory and build for x86
echo "Configuring and building for x86..."
cd "$PC_DIR" || exit 1
cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DCMAKE_TOOLCHAIN_FILE=toolchain-x86.cmake ..
cmake --build .
echo "Build for x86 done."

# Build for ARM second

if [ ! -d "$PI_DIR" ]; then 
    echo "Creating build directory: $PI_DIR"
    mkdir "$PI_DIR"
else 
    echo "Build directory alreadyexists: $PI_DIR"
fi

if [ -f "$PI_DIR/CMakeCache.txt" ]; then
    echo "Cleaning arm64 CMake cache"
    rm -rf "$PI_DIR/CMakeCache.txt" "$PI_DIR/CMakeFiles"
fi

# Make sure dir exists 

mkdir -p "$PI_DIR"

# Confgiure script for building arm64
echo "Configuring and building for arm64..."
cd "$PI_DIR" || exit 1
cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DCMAKE_TOOLCHAIN_FILE=toolchain-arm64.cmake ..
cmake --build .
echo "Build for arm64 done."


