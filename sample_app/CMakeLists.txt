cmake_minimum_required(VERSION 3.20.0)

# Set the WASI SDK toolchain file
set(CMAKE_TOOLCHAIN_FILE /opt/wasi-sdk/share/cmake/wasi-sdk.cmake)

# Set the project name (needs to come after the toolchain file) 
set(APPNAME sockets)
project(${APPNAME})

# Include the WAMR socket library CMake file (adjust path as needed)
include(../wasm-micro-runtime/core/iwasm/libraries/lib-socket/lib_socket_wasi.cmake)

# Set the linker flags 
set(CMAKE_EXE_LINKER_FLAGS "-Wl,--strip-all -Wl,--allow-undefined")

# Set compilation flags
add_compile_options(
    -O0 -Wno-unknown-attributes
    -O3
    -Wall
    -Wextra
    -Wno-unused-parameter
)

# Create the executable target
add_executable(${APPNAME}.wasm main.c)
target_include_directories(${APPNAME}.wasm PRIVATE
    ../wasm-micro-runtime/core/iwasm/libraries/lib-socket
)
target_link_libraries(${APPNAME}.wasm socket_wasi_ext)
