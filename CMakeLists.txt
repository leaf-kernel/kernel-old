cmake_minimum_required(VERSION 3.15)
project(Leaf LANGUAGES C ASM)

set(ARCH_DIR "${CMAKE_CURRENT_SOURCE_DIR}/arch/${LEAF_TARGET_ARCH}")

if(NOT EXISTS "${ARCH_DIR}")
    message(FATAL_ERROR "The ${LEAF_TARGET_ARCH} architecture isn't supported by Leaf (${ARCH_DIR})!")
endif()

add_subdirectory(${ARCH_DIR})

set(KERNEL_DIR "${CMAKE_CURRENT_SOURCE_DIR}/kernel")

if(NOT EXISTS "${KERNEL_DIR}")
    message(FATAL_ERROR "Directory '${KERNEL_DIR}' not found!")
endif()

add_subdirectory(${KERNEL_DIR})
