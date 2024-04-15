# Leaf Kernel
A x86_64 kernel.

## Building
First you need to build the kernel to do so simply run:
```bash
export TARGET=x86_64; cmake . -DTARGET_ARCH=$TARGET -DCMAKE_TOOLCHAIN_FILE=arch/$TARGET/toolchain.cmake -B build-$TARGET; cmake --build build-$TARGET
```

Now this might look a bit much (or even scary). But all this is doing is setting the TARGET to what target we are compiling to, in this case x86_64. Then generating the build file using cmake. Then to generate an iso run:
```bash
export TARGET=x86_64; cd arch/$TARGET; ./gen.sh; cd ../../
```

To do all this in one you can do:
```bash
export TARGET=x86_64; cmake . -DTARGET_ARCH=$TARGET -DCMAKE_TOOLCHAIN_FILE=arch/$TARGET/toolchain.cmake -B build-$TARGET; cmake --build build-$TARGET; cd arch/$TARGET; ./gen.sh; cd ../../
```

There even exists script to do this:
```bash
./compile.sh <target>
```

To run with qemu you can use the run script:
```bash
./run.sh <target> [qemu args]
```

## Features
- IDT
- PIC (8259)
- PIT
- PMM
- KHEAP
- CPUID
- Serial I/O
- Nighterm
- printf
- Ramdisk (ish only TAR for now. Still usable)

## Work in progress
We are currently working on APIC (IOAPIC and LAPIC).

## Target List
- x86_64

## Requirements
- nasm
- Crosscompiler and binutils for target (x86_64-elf)
- cmake
- xorriso

## Architectures
| Architecture 	| Status 	|
|--------------	|--------	|
| x86-64       	| ✅        |
| x86          	| ❌        |
