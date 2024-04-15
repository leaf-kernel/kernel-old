#!/bin/bash
tar -cvf modules/ramdisk ../../initrd/*
mkdir -p ../../release
mkdir -p iso
mkdir -p iso/boot
cp -v root/* iso
cp -v ../../bin/Leaf.bin iso/boot/leaf
make -C limine
cp -v limine/limine-bios.sys limine/limine-bios-cd.bin limine/limine-uefi-cd.bin iso
mkdir -p iso/EFI/BOOT
cp -v limine/BOOTX64.EFI iso/EFI/BOOT/
cp -v limine/BOOTIA32.EFI iso/EFI/BOOT/
cp -v modules/* iso
xorriso -as mkisofs -b limine-bios-cd.bin -no-emul-boot -boot-load-size 4 -boot-info-table --efi-boot limine-uefi-cd.bin -efi-boot-part --efi-boot-image --protective-msdos-label iso -o ../../release/Leaf-x86_64-$(date +%B-%Y).iso
limine/limine bios-install ../../release/Leaf-x86_64-$(date +%B-%Y).iso
rm -rf iso
