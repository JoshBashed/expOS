#include "driver_display_ramfb.h"
#include "driver_uart.h"
#include "logging.h"
#include "shared.h"

#define FB_W 800
#define FB_H 600
__attribute__((aligned(4096))) static u8 fb[FB_W * FB_H * 4];

void kernel_main(void) {
    driver_uart_init();
    logging_set_log_fn(driver_uart_put_char_for_logger);
    LOG_INFO("Starting kernel...");

    LOG_INFO("Initializing RAMFB driver...");

    display_driver_ramfb_QemuRamFBCfg cfg = {0};
    cfg.address = (u64)(usize)fb;
    cfg.fourcc = DISPLAY_DRIVER_RAMFB_FORMAT_XRGB32;
    cfg.width = FB_W;
    cfg.height = FB_H;
    cfg.stride = FB_W * 4;

    driver_display_ramfb_init(&cfg);
    LOG_INFO("RAMFB initialized.");

    while (1) {
        __asm__ volatile("wfe");
    }
}

shared_StaticString esr_class_name(u32 esr) {
    u32 ec = (esr >> 26) & 0x3F;
    switch (ec) {
    case 0x00:
        return STATIC_STRING("Unknown");
    case 0x01:
        return STATIC_STRING("Trapped WFI/WFE");
    case 0x03:
        return STATIC_STRING("MCR/MRC (AArch32)");
    case 0x04:
        return STATIC_STRING("MCRR/MRRC (AArch32)");
    case 0x05:
        return STATIC_STRING("MCR/MRC (coproc, AArch32)");
    case 0x06:
        return STATIC_STRING("STC/ATC (AArch32)");
    case 0x07:
        return STATIC_STRING("SIMD/FP (AArch32)");
    case 0x08:
        return STATIC_STRING("VMRS (AArch32)");
    case 0x0E:
        return STATIC_STRING("Illegal Execution");
    case 0x15:
        return STATIC_STRING("SVC (AArch32)");
    case 0x18:
        return STATIC_STRING("SVC (AArch64)");
    case 0x20:
        return STATIC_STRING("Instruction Abort, lower EL");
    case 0x21:
        return STATIC_STRING("Instruction Abort, same EL");
    case 0x22:
        return STATIC_STRING("PC alignment fault");
    case 0x24:
        return STATIC_STRING("Data Abort, lower EL");
    case 0x25:
        return STATIC_STRING("Data Abort, same EL");
    case 0x26:
        return STATIC_STRING("SP alignment fault");
    case 0x28:
        return STATIC_STRING("Trapped FP (AArch64)");
    case 0x2C:
        return STATIC_STRING("Breakpoint, lower EL");
    case 0x2D:
        return STATIC_STRING("Breakpoint, same EL");
    case 0x30:
        return STATIC_STRING("Software Step, lower EL");
    case 0x31:
        return STATIC_STRING("Software Step, same EL");
    case 0x32:
        return STATIC_STRING("Watchpoint, lower EL");
    case 0x33:
        return STATIC_STRING("Watchpoint, same EL");
    case 0x34:
        return STATIC_STRING("BKPT (AArch32)");
    case 0x35:
        return STATIC_STRING("Vector catch (AArch32)");
    case 0x36:
        return STATIC_STRING("BRK (AArch64)");
    default:
        return STATIC_STRING("Unknown/Reserved");
    }
}

void kernel_panic(u64 src, u64 esr, u64 elr, u64 far, u64 spsr) {
    LOG_PANIC("--- KERNEL PANIC ---");
    LOG_PANIC("ESR: {StaticString}", esr_class_name((u32)esr));
    LOG_PANIC("ELR: 0x{u64:HEX}", elr);
    LOG_PANIC("FAR: 0x{u64:HEX}", far);
    LOG_PANIC("SPSR: 0x{u64:HEX}", spsr);
    LOG_PANIC("Source: {StaticString}",
        src == 0 ? STATIC_STRING("Current EL") : STATIC_STRING("Lower EL"));
    LOG_PANIC("--------------------");

    while (1) {
        __asm__ volatile("wfe");
    }
}