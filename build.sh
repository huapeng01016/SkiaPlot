#!/bin/bash

# SkiaPlot Build Script
# This script helps build the SkiaPlot project

set -e

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

echo "======================================"
echo "  SkiaPlot Build Script"
echo "======================================"
echo

# Check if SKIA_DIR is set
if [ -z "$SKIA_DIR" ]; then
    echo -e "${YELLOW}Warning: SKIA_DIR environment variable is not set${NC}"
    echo "Please set SKIA_DIR to point to your Skia installation directory."
    echo "Example: export SKIA_DIR=/path/to/skia"
    echo
    echo "Would you like to continue anyway? (y/n)"
    read -r response
    if [[ ! "$response" =~ ^[Yy]$ ]]; then
        echo "Build cancelled."
        exit 1
    fi
else
    echo -e "${GREEN}Using Skia from: $SKIA_DIR${NC}"
    echo
fi

# Create build directory
if [ ! -d "build" ]; then
    echo "Creating build directory..."
    mkdir build
fi

cd build

# Run CMake
echo "Running CMake..."
cmake ..

# Check if Skia was found
if [ -z "$SKIA_DIR" ]; then
    echo
    echo -e "${YELLOW}Note: Examples will not be built without Skia library${NC}"
    echo "CMake configuration completed. The library interface is available."
    exit 0
fi

# Build the project
echo
echo "Building project..."
make -j$(nproc 2>/dev/null || echo 2)

if [ $? -eq 0 ]; then
    echo
    echo -e "${GREEN}Build successful!${NC}"
    echo
    echo "Example programs built:"
    echo "  - ./examples/simple_plot"
    echo "  - ./examples/sine_wave"
    echo "  - ./examples/multiple_series"
    echo
    echo "Run an example:"
    echo "  cd examples && ./simple_plot"
else
    echo
    echo -e "${RED}Build failed!${NC}"
    exit 1
fi
