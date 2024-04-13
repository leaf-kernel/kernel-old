#!/bin/bash

if [ $# -lt 1 ]; then
    echo "Usage: $0 <target> [qemu args]"
    exit 1
fi

TARGET=$1
shift

if make TARGET_ARCH="$TARGET" > /dev/null; then
    qemu-system-$TARGET -name "Leaf $TARGET" -drive file="release/Leaf-$TARGET-$(date +%B-%Y).iso",index=0,format=raw "$@"
else
    echo "Error: Failed to compile Leaf ($TARGET)"
    exit 1
fi

