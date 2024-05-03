#!/bin/bash

TARGET=${1:-x86_64}
BOOTLOADER=${2:-limine}
shift
shift

ISO_FILE="release/Leaf-$TARGET-$(date +%B-%Y).iso"

./env/configure "$TARGET" "$BOOTLOADER"
./env/build "$TARGET" "$BOOTLOADER"

qemu-system-"$TARGET" -name "Leaf $TARGET" -drive file="$ISO_FILE",index=0,format=raw -machine q35 "$@"