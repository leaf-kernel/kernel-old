cmake_minimum_required(VERSION 3.15)
project(Leaf LANGUAGES C ASM)

set(LEAF_TARGET "Leaf-${LEAF_TARGET_ARCH}-raw.img")

file(GLOB_RECURSE CC_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/*.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
)

file(GLOB_RECURSE ASM_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/*.asm"
)

add_executable(${LEAF_TARGET} ${CC_FILES} ${ASM_FILES})

target_compile_options(${LEAF_TARGET} PRIVATE
    ${TARGET_CFLAGS}
)

set_target_properties(${LEAF_TARGET} PROPERTIES
    COMPILE_FLAGS "${TARGET_ASFLAGS}"
)

set_target_properties(${LEAF_TARGET} PROPERTIES
    LINK_FLAGS "${TARGET_LDFLAGS}"
)

target_include_directories(${LEAF_TARGET} PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/../arch/${TARGET_ARCH}"
)

target_compile_definitions(${LEAF_TARGET}
    PUBLIC LEAF_ARCH="${TARGET_ARCH}"
    PUBLIC LEAF_BOOTLOADER="${TARGET_BOOTLOADER}"
    PUBLIC LEAF_VERSION="${TARGET_VER}"
    PUBLIC LEAF_LIMINE
)