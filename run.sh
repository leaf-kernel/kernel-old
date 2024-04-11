#!/bin/bash

if [ $# -lt 1 ]; then
    echo "Usage: $0 <target> [qemu args]"
    exit 1
fi

TARGET=$1
shift

make clean
make TARGET="$TARGET"

qemu-system-$TARGET --serial stdio -name "Leaf $TARGET" -cdrom "release/Leaf-$TARGET-$(date +%s).iso" "$@"
