#!/bin/bash
set -e

echo "Building Nova..."
mkdir -p build
cd build
cmake ..
make -j$(nproc)
echo "Build complete!"
echo "Run: ./tools/nova/nova ../examples/hello_world.nova"
