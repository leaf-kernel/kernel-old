cmake_minimum_required(VERSION 3.15)
project(Leaf LANGUAGES C ASM)

set(ARCH_DIR "${CMAKE_CURRENT_SOURCE_DIR}/arch/${LEAF_TARGET_ARCH}")

if(NOT EXISTS "${ARCH_DIR}")
    message(FATAL_ERROR "The ${LEAF_TARGET_ARCH} architecture isn't supported by Leaf (${ARCH_DIR})!")
endif()

include("${ARCH_DIR}/CMakeLists.txt")

set(KERNEL_DIR "${CMAKE_CURRENT_SOURCE_DIR}/kernel")

if(NOT EXISTS "${KERNEL_DIR}")
    message(FATAL_ERROR "Directory '${KERNEL_DIR}' not found!")
endif()

add_subdirectory(${KERNEL_DIR})

set(CMAKE_ASM_COMPILER ${TARGET_ASM})
set(CMAKE_C_COMPILER ${TARGET_CC})
set(CMAKE_LINKER ${TARGET_LD})