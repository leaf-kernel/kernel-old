cmake_minimum_required(VERSION 3.15)
project(Leaf LANGUAGES C ASM_NASM)

# Check if the project has been configured
if(NOT DEFINED TARGET_ARCH)
    message(FATAL_ERROR "Please run ./configure to configure this project.")
endif()

# Check for kernel directory
set(KERNEL_DIR "${CMAKE_CURRENT_SOURCE_DIR}/kernel")

if(NOT EXISTS "${KERNEL_DIR}")
    message(FATAL_ERROR "Directory '${KERNEL_DIR}' not found!")
endif()

add_subdirectory(${KERNEL_DIR})
