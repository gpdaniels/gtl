#!/bin/bash
set -e

# Default settings.
BUILD_TYPE=Debug

# Check for command line arguments.
if [ $# -gt 0 ]; then
    BUILD_TYPE=$1
fi

# Ensure we are in the correct directory.
cd $(cd "$(dirname "$0")"; pwd)

# Echo settings.
echo "Building for ${BUILD_TYPE}"

# Run the platform dependent build pipeline.
if [[ "$OSTYPE" == "linux-gnu" ]]; then
    # Linux.
    mkdir -p BUILD
    pushd BUILD
    cmake -DCMAKE_BUILD_TYPE=${BUILD_TYPE} -DBUILD_TESTING=ON -DBUILD_MEMCHECK=ON ..
    cmake --build . --config ${BUILD_TYPE} -- -j4
    cmake --build . --config ${BUILD_TYPE} --target test
    popd
elif [[ "$OSTYPE" == "darwin"* ]]; then
    # Mac OSX.
    mkdir -p BUILD
    pushd BUILD
    cmake -DCMAKE_BUILD_TYPE=${BUILD_TYPE} -DBUILD_TESTING=ON -DBUILD_MEMCHECK=ON  ..
    cmake --build . --config ${BUILD_TYPE} -- -j4
    cmake --build . --config ${BUILD_TYPE} --target test
    popd
elif [[ "$OSTYPE" == "msys" ]]; then
    # Lightweight shell and GNU utilities compiled for Windows (part of MinGW).
    mkdir -p BUILD
    pushd BUILD
    cmake -DCMAKE_GENERATOR_PLATFORM=x64 -DBUILD_TESTING=ON -DBUILD_MEMCHECK=ON  ..
    cmake --build . --config ${BUILD_TYPE} -- -nologo -verbosity:minimal -maxcpucount
    cmake --build . --config ${BUILD_TYPE} --target test
    popd
elif [[ "$OSTYPE" == "cygwin" ]]; then
    # POSIX compatibility layer and Linux environment emulation for Windows.
    (>&2 echo "Building on cgywin is unsupported.")
    exit 1
elif [[ "$OSTYPE" == "freebsd"* ]]; then
    # FreeBSD.
    (>&2 echo "Building on freebsd is unsupported.")
    exit 1
else
    # Unknown.
    (>&2 echo "Failed to detect OS platform.")
    exit 1
fi
