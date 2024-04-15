cmake_minimum_required(VERSION 3.15)

# Target Tools
set(TARGET_CC   x86_64-elf-gcc)
set(TARGET_LD   x86_64-elf-ld)
set(TARGET_ASM  nasm)

# Target Tools Check
find_program(TARGET_CC x86_64-elf-gcc)
if(NOT TARGET_CC)
    message(FATAL_ERROR "You need x86_64-elf-gcc to build Leaf for x86_64.")
endif()

find_program(TARGET_LD x86_64-elf-ld)
if(NOT TARGET_LD)
    message(FATAL_ERROR "You need x86_64-elf-binutils to build Leaf for x86_64.")
endif()

find_program(TARGET_ASM nasm)
if(NOT TARGET_ASM)
    message(FATAL_ERROR "You need nasm to build Leaf for x86_64.")
endif()


set(TARGET_CFLAGS
    -g
    -O2
    -pipe
    -O0
    -Wall
    -Wextra
    -Werror
    -std=gnu11
    -ffreestanding
    -fno-stack-protector
    -fno-stack-check
    -fno-lto
    -fno-PIE
    -fno-PIC
    -m64
    -march=x86-64
    -mabi=sysv
    -mcmodel=kernel
    -mno-80387
    -mno-red-zone
    -msse
    -msse2
    -Wimplicit-function-declaration
    -Wdiv-by-zero
    -Wunused-variable 
)

set(TARGET_LDFLAGS
    -nostdlib
    -static
    -m elf_x86_64
    -z max-page-size=0x1000
    -T linker.ld
)

set(TARGET_ASFLAGS
    -felf64
)
