#!/bin/bash
if [ $# -lt 1 ]; then
    echo "Usage: $0 <target>"
    exit 1
fi

export TARGET=$1; cmake . -DTARGET_ARCH=$TARGET -DCMAKE_TOOLCHAIN_FILE=arch/$TARGET/toolchain.cmake -B build-$TARGET; cmake --build build-$TARGET; cd arch/$TARGET; ./gen.sh; cd ../../