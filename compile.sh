#!/bin/bash
if [ $# -lt 1 ]; then
    echo "Usage: $0 <target>"
    exit 1
fi

TARGET=$1
cmake . -DLEAF_TARGET_ARCH=$TARGET -DCMAKE_TOOLCHAIN_FILE=arch/$TARGET/toolchain.cmake -B build-$TARGET
make -C build-$TARGET