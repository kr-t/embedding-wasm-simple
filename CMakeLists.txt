cmake_minimum_required(VERSION 3.13)
project(app C)

set(WAMR_BUILD_PLATFORM "linux")
set(WAMR_BUILD_TARGET "X86_64")
set(WAMR_BUILD_INTERP 1)
set(WAMR_BUILD_FAST_INTERP 1)
set(WAMR_BUILD_AOT 1)
set(WAMR_BUILD_LIBC_BUILTIN 1)
set(WAMR_BUILD_LIBC_WASI 1)
set(WAMR_BUILD_SIMD 1)
set(WAMR_BUILD_REF_TYPES 1)

set(WAMR_ROOT_DIR wasm-micro-runtime)
set(SHARED_DIR ${WAMR_ROOT_DIR}/core/shared)

include(${WAMR_ROOT_DIR}/build-scripts/runtime_lib.cmake)
add_library(vmlib ${WAMR_RUNTIME_LIB_SOURCE})

include(${SHARED_DIR}/utils/uncommon/shared_uncommon.cmake)

add_executable(app main.c ${UNCOMMON_SHARED_SOURCE})
target_link_libraries(app vmlib m pthread)

set_target_properties(app PROPERTIES
    C_STANDARD 99
    C_STANDARD_REQUIRED ON
)

if (CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(app PRIVATE -Wall -Wextra -Wno-unused-parameter)
endif()
