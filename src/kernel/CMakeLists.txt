cmake_minimum_required(VERSION 3.20)
project(kernel C ASM)

# Sources
file(GLOB SRCS
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.S
)

add_executable(kernel.elf ${SRCS})

# Libraries
target_link_libraries(kernel.elf PRIVATE shared)
target_link_libraries(kernel.elf PRIVATE logging)
target_link_libraries(kernel.elf PRIVATE driver_uart)
target_link_libraries(kernel.elf PRIVATE driver_display_ramfb)

# Produce flat binary from the actual linked path
add_custom_command(
    TARGET kernel.elf POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O binary "$<TARGET_FILE:kernel.elf>" "$<TARGET_FILE_DIR:kernel.elf>/kernel.bin"
    COMMENT "Generating flat binary kernel.bin from kernel.elf"
)

# Disassembly
add_custom_target(dump
    COMMAND ${CMAKE_OBJDUMP} -D "$<TARGET_FILE:kernel.elf>" > "$<TARGET_FILE_DIR:kernel.elf>/kernel.dis"
    DEPENDS kernel.elf
    COMMENT "Generating kernel.dis disassembly"
)


# Run QEMU using the actual target path
add_custom_target(run
    COMMAND qemu-system-aarch64 -M virt -cpu cortex-a53 -m 512M -monitor stdio -device ramfb -kernel "$<TARGET_FILE:kernel.elf>"
    DEPENDS kernel.elf
    COMMENT "Running QEMU (Ctrl-A X to quit)"
)

add_custom_target(debug
    COMMAND qemu-system-aarch64 -M virt -cpu cortex-a53 -m 512M -monitor stdio -device ramfb -S -s -kernel "$<TARGET_FILE:kernel.elf>"
    DEPENDS kernel.elf
    COMMENT "Running QEMU with GDB (target remote :1234) (Ctrl-A X to quit)"
)
