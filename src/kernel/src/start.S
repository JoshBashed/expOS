.section .text
.align 4
.globl _start

_start:
    // Privilege level
    mrs x0, CurrentEL // x0 = CurrentEL

    // if (x0 == EL3) el3_to_el1()
    cmp x0, #0xC
    b.eq el3_to_el1

    // if (x0 == EL2) el2_to_el1()
    cmp x0, #0x8
    b.eq el2_to_el1

    // else el1_entry()
    b el1_to_el1

el3_to_el1:
    // Enable FP/SIMD at EL1
    // cpacr_el1 = cpacr_el1 | (3 << 20)
    mov x0, #3 << 20
    msr cpacr_el1, x0
    isb

    // Disable traps for FP/SIMD instructions to EL3
    mov x0, xzr // x0 = 0
    msr cptr_el3, x0 // cptr_el3 = x0
    isb

    // Setup System Control Register for EL1
    // http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.ddi0500d/CIHDIEBD.html
    mov x0, xzr // x0 = 0
    orr x0, x0, #(1 << 29) // RES1
    orr x0, x0, #(1 << 28) // RES1
    orr x0, x0, #(1 << 23) // RES1
    orr x0, x0, #(1 << 22) // RES1
    orr x0, x0, #(1 << 20) // RES1
    orr x0, x0, #(1 << 11) // RES1
    msr sctlr_el1, x0 // sctlr_el1 = x0
    isb

    // Setup Secure Configuration Register for EL1
    mrs x0, scr_el3 // x0 = scr_el3
    orr x0, x0, #(1 << 10) // RW = 1 (AArch64)
    orr x0, x0, #(1 << 0)  // NS = 1 (Non-secure)
    msr scr_el3, x0 // scr_el3 = x0
    isb

    // Set stack pointer for EL1
    ldr x0, =__stack_top
    msr sp_el1, x0 // sp_el1 = __stack_top
    // Point EL1 vector base to our vectors
    ldr x0, =vectors_stub
    msr vbar_el1, x0
    isb

    // Setup Saved Program Status Register for EL1
    mov x0, xzr // x0 = 0
    mov x0, #0b00101 // M[3:0] = EL1h
    orr x0, x0, #(1 << 8) // SError mask
    orr x0, x0, #(1 << 7) // IRQ mask
    orr x0, x0, #(1 << 6) // FIQ mask
    msr spsr_el3, x0 // spsr_el3 = x0
    isb

    // Drop to EL1
    adr x0, el1_entry
    msr elr_el3, x0 // elr_el3 = &el1_entry
    eret

el2_to_el1:
    // EL1 = AArch64
    mov x0, #(1 << 31) // x0 = RW
    msr hcr_el2, x0 // hcr_el2 = x0
    isb

    // Enable FP/SIMD timers at EL1
    // Don't trap FP/SIMD instructions to EL2
    mov x0, xzr // x0 = 0
    msr cptr_el2, x0 // cptr_el2 = x0
    // Allow EL1 access to physical counter/timer
    mov x0, xzr // x0 = 0
    orr x0, x0, #(1 << 0) // EL1PCTEN
    orr x0, x0, #(1 << 1) // EL1PCEN
    msr cnthctl_el2, x0 // cnthctl_el2 = x0
    // Zero the virtual offset
    msr cntvoff_el2, xzr
    isb

    // Setup EL1 control
    // Set stack pointer for EL1
    ldr x0, =__stack_top
    msr sp_el1, x0 // sp_el1 = __stack_top
    // Point EL1 vector base to our vectors
    ldr x0, =vectors_stub
    msr vbar_el1, x0
    isb

    // Setup System Control Register for EL1
    // http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.ddi0500d/CIHDIEBD.html
    mov x0, xzr // x0 = 0
    orr x0, x0, #(1 << 29) // RES1
    orr x0, x0, #(1 << 28) // RES1
    orr x0, x0, #(1 << 23) // RES1
    orr x0, x0, #(1 << 22) // RES1
    orr x0, x0, #(1 << 20) // RES1
    orr x0, x0, #(1 << 11) // RES1
    msr sctlr_el1, x0 // sctlr_el1 = x0
    isb

    // Enable FP/SIMD at EL1
    // cpacr_el1 = cpacr_el1 | (3 << 20)
    mov x0, #3 << 20
    msr cpacr_el1, x0
    isb

    // Setup Saved Program Status Register for EL1
    mov x0, xzr // x0 = 0
    mov x0, #0b00101 // M[3:0] = EL1h
    orr x0, x0, #(1 << 8) // SError mask
    orr x0, x0, #(1 << 7) // IRQ mask
    orr x0, x0, #(1 << 6) // FIQ mask
    msr spsr_el2, x0 // spsr_el2 = x0
    isb

    // Drop to EL1
    adr x0, el1_entry
    msr elr_el2, x0 // elr_el2 = &el1_entry
    eret

// QEMU seems to always start in EL1, so we just need to set up things
// properly and jump to el1_entry.
el1_to_el1:
    // Enable FP/SIMD at EL1
    mrs x0, cpacr_el1
    mov x1, #(3 << 20)      // FPEN=0b11
    orr x0, x0, x1
    msr cpacr_el1, x0
    isb

    // Optional: clear FPCR/FPSR
    msr fpcr, xzr
    msr fpsr, xzr
    isb

    // Point EL1 vector base to our vectors
    ldr x0, =vectors_stub
    msr vbar_el1, x0
    isb

    // Minimal SCTLR_EL1 baseline: RES1 bits set, MMU/caches off for now
    mov x0, xzr
    orr x0, x0, #(1 << 29) // RES1
    orr x0, x0, #(1 << 28) // RES1
    orr x0, x0, #(1 << 23) // RES1
    orr x0, x0, #(1 << 22) // RES1
    orr x0, x0, #(1 << 20) // RES1
    orr x0, x0, #(1 << 11) // RES1
    msr sctlr_el1, x0
    isb

    // el1_entry()
    b el1_entry

el1_entry:
    // Set the stack pointer
    ldr x0, =__stack_top
    and x0, x0, #~15  // Ensure 16-byte alignment
    mov sp, x0

    // Zero .bss
    ldr x1, =__bss_start
    ldr x2, =__bss_end
0:
    cmp x1, x2
    b.ge 1f
    strb wzr, [x1], #1
    b 0b
1:
    // kernel_main()
    bl kernel_main

.align 11
vectors_stub:
    // Synchronous EL1t
    b vec_sync_el1t
    .balign 128
    // IRQ EL1t
    b vec_irq_el1t
    .balign 128
    // FIQ EL1t
    b vec_fiq_el1t
    .balign 128
    // SError EL1t
    b vec_serr_el1t
    .balign 128
    // Synchronous EL1h
    b vec_sync_el1h
    .balign 128
    // IRQ EL1h
    b vec_irq_el1h
    .balign 128
    // FIQ EL1h
    b vec_fiq_el1h
    .balign 128
    // SError EL1h
    b vec_serr_el1h
    .balign 128
    // Synchronous 64-bit EL0
    b vec_sync_el0_64
    .balign 128
    // IRQ EL0
    b vec_irq_el0_64
    .balign 128
    // FIQ EL0
    b vec_fiq_el0_64
    .balign 128
    // SError EL0
    b vec_serr_el0_64
    .balign 128
    // Synchronous 32-bit EL0
    b vec_sync_el0_32
    .balign 128
    // IRQ 32-bit EL0
    b vec_irq_el0_32
    .balign 128
    // FIQ 32-bit EL0
    b vec_fiq_el0_32
    .balign 128
    // SError 32-bit EL0
    b vec_serr_el0_32
    .balign 128

// Per-slot labels set x0=source id and branch to common handler.
// Source IDs are arbitrary; pick stable values for decoding.
vec_sync_el1t:     mov x0, #0x01; b exception_to_panic
vec_irq_el1t:      mov x0, #0x02; b exception_to_panic
vec_fiq_el1t:      mov x0, #0x03; b exception_to_panic
vec_serr_el1t:     mov x0, #0x04; b exception_to_panic

vec_sync_el1h:     mov x0, #0x11; b exception_to_panic
vec_irq_el1h:      mov x0, #0x12; b exception_to_panic
vec_fiq_el1h:      mov x0, #0x13; b exception_to_panic
vec_serr_el1h:     mov x0, #0x14; b exception_to_panic

vec_sync_el0_64:   mov x0, #0x21; b exception_to_panic
vec_irq_el0_64:    mov x0, #0x22; b exception_to_panic
vec_fiq_el0_64:    mov x0, #0x23; b exception_to_panic
vec_serr_el0_64:   mov x0, #0x24; b exception_to_panic

vec_sync_el0_32:   mov x0, #0x31; b exception_to_panic
vec_irq_el0_32:    mov x0, #0x32; b exception_to_panic
vec_fiq_el0_32:    mov x0, #0x33; b exception_to_panic
vec_serr_el0_32:   mov x0, #0x34; b exception_to_panic

exception_to_panic:
    // Read syndrome and fault information
    mrs x1, esr_el1       // Exception Syndrome
    mrs x2, elr_el1       // Return address
    mrs x4, spsr_el1      // Saved PSTATE
    // FAR_ELx is valid for data aborts and some sync exceptions; read it unconditionally
    mrs x3, far_el1

    // Call kernel_panic(source_id, esr, elr, far, spsr)
    bl kernel_panic

    // Halt
1:  wfe
    b 1b