#!/bin/bash

TARGET=${1:-x86_64}
BOOTLOADER=${2:-limine}
shift
shift

ISO_FILE="release/Leaf-$TARGET-$(date +%B-%Y).iso"

./env/configure "$TARGET" "$BOOTLOADER"
./env/build "$TARGET" "$BOOTLOADER"

qemu-system-"$TARGET" -m 2G -drive if=pflash,format=raw,readonly=on,file=arch/$TARGET/bios/ovmf.fd -drive if=ide,format=raw,file=$ISO_FILE -net none -machine q35 "$@"