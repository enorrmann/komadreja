#!/bin/bash

# Exit immediately if a command exits with a non-zero status
set -e

echo "=== Cleaning build directory ==="
if [ -d "build" ]; then
    rm -rf build
fi

echo "=== Creating build directory ==="
mkdir build
cd build

echo "=== Configuring with CMake ==="
cmake -DCMAKE_PREFIX_PATH=/home/emilio/Qt/6.10.1/gcc_64 ..

echo "=== Compiling project ==="
# Use all available CPU cores for faster build
make -j$(nproc)

echo "=== Build completed successfully ==="
cd ..
