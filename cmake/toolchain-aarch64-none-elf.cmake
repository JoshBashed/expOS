set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR aarch64)

set(CROSS_PREFIX "aarch64-none-elf")

set(CMAKE_C_COMPILER "${CROSS_PREFIX}-gcc")
set(CMAKE_ASM_COMPILER "${CROSS_PREFIX}-gcc")
set(CMAKE_AR "${CROSS_PREFIX}-ar")
set(CMAKE_OBJCOPY "${CROSS_PREFIX}-objcopy")
set(CMAKE_OBJDUMP "${CROSS_PREFIX}-objdump")

set(COMMON_FLAGS "-ffreestanding -nostdlib -nostartfiles -Wall -Wextra -O2 -g")

set(CMAKE_C_FLAGS "${COMMON_FLAGS}")
set(CMAKE_ASM_FLAGS "${COMMON_FLAGS}")

set(CMAKE_EXE_LINKER_FLAGS "-T ${CMAKE_SOURCE_DIR}/src/kernel/linker.ld")
