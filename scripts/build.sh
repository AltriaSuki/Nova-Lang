#!/bin/bash
set -euo pipefail

echo "Building Nova..."
mkdir -p build
cd build
BUILD_TYPE="${BUILD_TYPE:-Debug}"
cmake -S .. -B . -DCMAKE_BUILD_TYPE="${BUILD_TYPE}"

# Cross-platform CPU count detection
if [[ "$OSTYPE" == "darwin"* ]]; then
    NPROC=$(sysctl -n hw.ncpu)
else
    NPROC=$(nproc)
fi

cmake --build . -j"$NPROC"
echo "Build complete!"
echo "Run: ./bin/nova"
echo "Run: ./bin/nova-repl"
