#!/bin/bash

# Exit immediately if a command exits with a non-zero status
set -e

echo "=== Cleaning Android build directory ==="
if [ -d "build_android" ]; then
    rm -rf build_android
fi

echo "=== Creating Android build directory ==="
mkdir build_android
cd build_android

echo "=== Configuring with CMake for Android (arm64-v8a) ==="
cmake -DCMAKE_TOOLCHAIN_FILE=/home/emilio/Qt/6.10.1/android_arm64_v8a/lib/cmake/Qt6/qt.toolchain.cmake \
      -DANDROID_ABI=arm64-v8a \
      -DANDROID_NDK_ROOT=/home/emilio/Android/Sdk/ndk/27.2.12479018 \
      -DANDROID_SDK_ROOT=/home/emilio/Android/Sdk \
      -DQT_HOST_PATH=/home/emilio/Qt/6.10.1/gcc_64 \
      ..

echo "=== Compiling Android project ==="
# Use all available CPU cores for faster build
make -j$(nproc)

# Optionally build APK if the target exists
# make apk

echo "=== Android Build completed successfully ==="
cd ..
