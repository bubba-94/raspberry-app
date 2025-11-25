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

# Find directory and build for x86
echo "Configuring and building for x86..."
cd "$PC_DIR" || exit 1
cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DCMAKE_TOOLCHAIN_FILE=toolchain-x86.cmake ..
cmake --build .
echo "Build for x86 done."

cd ..
# Build for ARM second

if [ ! -d "$PI_DIR" ]; then 
    echo "Creating build directory: $PI_DIR"
    mkdir "$PI_DIR"
else 
    echo "Build directory already exists: $PI_DIR"
fi

if [ -f "$PI_DIR/CMakeCache.txt" ]; then
    echo "Cleaning aarch64 CMake cache"
    rm -rf "$PI_DIR/CMakeCache.txt" "$PI_DIR/CMakeFiles"
fi

# Confgiure script for building aarch64
echo "Configuring and building for aarch64..."
cd "$PI_DIR" || exit 1c
cmake .. \
  -DCMAKE_TOOLCHAIN_FILE=../toolchain-aarch64.cmake \
  -DCMAKE_INSTALL_PREFIX=/usr

cmake --build .
echo "Build for aarch64 done."


