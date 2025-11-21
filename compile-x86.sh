#!/bin/sh

# Variables
BUILD_DIR=build
X86_DIR=x86
SOURCE_DIR=$(pwd)
OUTPUT_DIR="$SOURCE_DIR/bin/$X86_DIR"

# Create build directory if it doesn't exist
if [ ! -d "$BUILD_DIR" ]; then
    echo "Creating build directory: $BUILD_DIR"
    mkdir "$BUILD_DIR"
else
    echo "Build directory already exists: $BUILD_DIR"
fi

# Optionally clean CMake cache
if [ -f "$BUILD_DIR/CMakeCache.txt" ]; then
    echo "Cleaning CMake cache..."
    rm -rf "$BUILD_DIR/CMakeCache.txt" "$BUILD_DIR/CMakeFiles"
fi

# Ensure output directory exists
mkdir -p "$OUTPUT_DIR"

# Build for x86
echo "Configuring and building for $X86_DIR..."

cd "$BUILD_DIR" || exit 1

# Configure CMake
# - Export compile commands for clang-tidy or other tools
# - Set runtime, archive, and library output directories
cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON ..

# Build
cmake --build .

echo "Build for $X86_DIR done."