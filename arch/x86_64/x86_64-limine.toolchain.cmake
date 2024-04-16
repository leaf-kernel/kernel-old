set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_CROSSCOMPILING TRUE)
set(CMAKE_C_COMPILER_WORKS TRUE)
set(CMAKE_ASM_COMPILER_WORKS TRUE)
set(CMAKE_C_COMPILER   x86_64-elf-gcc)
set(CMAKE_LINKER  x86_64-elf-ld)
set(CMAKE_ASM_NASM_COMPILER nasm)

if(CMAKE_ASM_NASM_COMPILER)
  set(CMAKE_ASM_NASM_FLAGS "-f elf64") 
  set(CMAKE_ASM_NASM_OBJECT_FORMAT "elf64")
else()
  message(FATAL_ERROR "NASM compiler not found!")
endif()

set(CMAKE_C_FLAGS "-g -O2 -pipe -O0 -Wall -Wextra -Werror -std=gnu11 -ffreestanding -fno-stack-protector -fno-stack-check -fno-lto -fno-PIE -fno-PIC -m64 -march=x86-64 -mabi=sysv -mcmodel=kernel -mno-80387 -mno-red-zone -msse -msse2 -Wimplicit-function-declaration -Wdiv-by-zero -Wunused-variable")
set(CMAKE_EXE_LINKER_FLAGS "-nostdlib -static -z max-page-size=0x1000 -T ${CMAKE_SOURCE_DIR}/arch/${TARGET_ARCH}/linker.ld")
set(TARGET_BOOTLOADER "limine")
set(TARGET_VERSION "v0.0.1")
set(TARGET_DEFINITIONS
		LEAF_LIMINE
		LEAF_X64
)