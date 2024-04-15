set(CMAKE_C_COMPILER   x86_64-elf-gcc)
set(CMAKE_ASM_COMPILER nasm)

find_program(CMAKE_C_COMPILER_TARGET x86_64-elf-gcc)
if(NOT CMAKE_C_COMPILER_TARGET)
    message(FATAL_ERROR "You need x86_64-elf-gcc to build Leaf for x86_64!")
endif()

find_program(CMAKE_ASM_COMPILER_TARGET nasm)
if(NOT CMAKE_ASM_COMPILER_TARGET)
    message(FATAL_ERROR "You need nasm to assemble Leaf for x86_64!")
endif()

set(CMAKE_C_FLAGS "-g -O2 -pipe -O0 -Wall -Wextra -Werror -std=gnu11 -ffreestanding -fno-stack-protector -fno-stack-check -fno-lto -fno-PIE -fno-PIC -m64 -march=x86-64 -mabi=sysv -mcmodel=kernel -mno-80387 -mno-red-zone -msse -msse2 -Wimplicit-function-declaration -Wdiv-by-zero -Wunused-variable")
set(CMAKE_ASM_NASM_FLAGS "-felf64")
set(CMAKE_EXE_LINKER_FLAGS "-nostdlib -static -z max-page-size=0x1000 -T linker.ld")
