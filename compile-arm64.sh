#!/bin/sh

# Variables
BUILD_DIR=build
ARM_DIR=arm64
SOURCE_DIR=$(pwd)
OUTPUT_DIR="$SOURCE_DIR/bin/$ARM_DIR"

# Create build directory if it doesn't exist
if [ ! -d "$BUILD_DIR" ]; then
    echo "Creating build directory: $BUILD_DIR"
    mkdir "$BUILD_DIR"
else
    echo "Build directory already exists: $BUILD_DIR"
fi


if [ -f "$BUILD_DIR/CMakeCache.txt" ]; then
    echo "Cleaning CMake cache..."
    rm -rf "$BUILD_DIR/CMakeCache.txt" "$BUILD_DIR/CMakeFiles"
fi

# Ensure output directory exists
mkdir -p "$OUTPUT_DIR"

# Build for arm64
echo "Configuring and building for $ARM_DIR..."

cd "$BUILD_DIR" || exit 1
cmake -DCMAKE_TOOLCHAIN_FILE=toolchain-arm64.cmake -DCMAKE_BUILD-TYPE=Debug ..
cmake --build .
