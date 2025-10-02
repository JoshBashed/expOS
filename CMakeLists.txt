cmake_minimum_required(VERSION 3.20)
project(expOS C ASM)

# Make logs not be so noisy
add_compile_options(-fmacro-prefix-map=${CMAKE_SOURCE_DIR}=.)

add_subdirectory(src/shared)
add_subdirectory(src/logging)
add_subdirectory(src/kernel)
add_subdirectory(src/driver_uart)
add_subdirectory(src/driver_display_ramfb)
