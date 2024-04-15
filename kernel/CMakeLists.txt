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


set(CMAKE_ASM_COMPILER ${TARGET_ASM})
set(CMAKE_C_COMPILER ${TARGET_CC})
set(CMAKE_LINKER ${TARGET_LD})

add_executable(${LEAF_TARGET} ${CC_FILES} ${ASM_FILES})

target_include_directories(${LEAF_TARGET} PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/../arch/${LEAF_TARGET_ARCH}"
)

target_compile_options(${LEAF_TARGET} PRIVATE
    ${TARGET_CFLAGS}
)

target_link_options(${LEAF_TARGET} PRIVATE 
    ${TARGET_LDFLAGS}
)

target_compile_options(${LEAF_TARGET} PRIVATE
    $<$<COMPILE_LANGUAGE:ASM>:${TARGET_ASFLAGS}>
)