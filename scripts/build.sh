#!/bin/bash
set -e

echo "Building Nova..."
mkdir -p build
cd build
cmake ..

# Cross-platform CPU count detection
if [[ "$OSTYPE" == "darwin"* ]]; then
    NPROC=$(sysctl -n hw.ncpu)
else
    NPROC=$(nproc)
fi

make -j"$NPROC"
echo "Build complete!"
echo "Run: ./tools/nova/nova ../examples/hello_world.nova"
