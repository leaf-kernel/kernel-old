#!/bin/bash

if [ $# -lt 1 ]; then
    echo "Usage: $0 <target> [qemu args]"
    exit 1
fi

TARGET=$1
shift

rm -rf build-$TARGET; ./compile.sh $TARGET; cd arch/$TARGET; ./gen.sh; cd ../../
qemu-system-$TARGET -name "Leaf $TARGET" -drive file="release/Leaf-$TARGET-$(date +%B-%Y).iso",index=0,format=raw "$@"