set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_CROSSCOMPILING TRUE)
set(CMAKE_C_COMPILER_WORKS TRUE)
set(CMAKE_CXX_COMPILER_WORKS TRUE)
set(CMAKE_ASM_COMPILER_WORKS TRUE)
set(CMAKE_C_COMPILER   x86_64-elf-gcc)
set(CMAKE_CXX_COMPILER x86_64-elf-g++)
set(CMAKE_LINKER  x86_64-elf-ld)
set(CMAKE_ASM_NASM_COMPILER nasm)

if(CMAKE_ASM_NASM_COMPILER)
    set(CMAKE_ASM_NASM_FLAGS "-f elf64")
    set(CMAKE_ASM_NASM_OBJECT_FORMAT "elf64")
else()
    message(FATAL_ERROR "NASM compiler not found!")
endif()

set(COMMON_FLAGS "-std=c99 -ffreestanding -fno-stack-protector -fno-stack-check -fno-omit-frame-pointer -fno-lto -fPIE -m64 -march=x86-64 -mabi=sysv -mno-80387 -mno-mmx -msse -msse2 -mno-red-zone -MP")

set(CMAKE_C_FLAGS "${COMMON_FLAGS} -std=gnu11")
set(CMAKE_CXX_FLAGS "${COMMON_FLAGS} -std=gnu++11")

set(CMAKE_EXE_LINKER_FLAGS "-nostdlib -static -z max-page-size=0x1000 -T ${CMAKE_SOURCE_DIR}/arch/${TARGET_ARCH}/linker.ld")

set(TARGET_BOOTLOADER "limine")
set(TARGET_VERSION "v0.0.1")

set(TARGET_DEFINITIONS
    LEAF_LIMINE
    LEAF_X64
    LEAF_DEBUG
    LEAF_ARCH="x86_64"
    LEAF_OFFSET="${LEAF_OFFSET}"
    LEAF_ARCH_RAW=x86_64
    __LEAF_DEBUG_WRAPPERS__
)

