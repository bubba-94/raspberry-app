#!/bin/sh

# Variables
BUILD_DIR=build
X86_DIR=x86
SOURCE_DIR=$(pwd)

# Create build directory if it doesn't exist
if [ -d "$BUILD_DIR" ]; then
    echo "Build directory already available"
else
    echo "Created $BUILD_DIR directory"
    mkdir "$BUILD_DIR"
fi

# Build for x86
echo "Building for $X86_DIR"

cd "$BUILD_DIR"

# Configure CMake and build
cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DCMAKE_RUNTIME_OUTPUT_DIRECTORY="$SOURCE_DIR/bin/x86" "$SOURCE_DIR"
cmake --build .

echo "Building for $X86_DIR done"