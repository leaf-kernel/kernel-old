#!/bin/bash

if [ $# -lt 1 ]; then
    echo "Usage: $0 <target> [qemu args]"
    exit 1
fi

TARGET=$1
shift

make TARGET_ARCH="$TARGET" > /dev/null

qemu-system-$TARGET -name "Leaf $TARGET" -cdrom "release/Leaf-$TARGET-$(date +%B-%Y).iso" "$@"
