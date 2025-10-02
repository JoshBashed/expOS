ENTRY(_start)

SECTIONS
{
    /* QEMU virt loads -kernel at 0x40000000 */
    . = 0x40000000;
    __start = .;

    .text : {
        KEEP(*(.text._start))
        *(.text*)
    }

    .rodata : {
        *(.rodata*)
    }

    .data : {
        *(.data*)
    }

    .bss (NOLOAD) : {
        __bss_start = .;
        *(.bss*)
        *(COMMON)
        . = ALIGN(8);
        __bss_end = .;
    }

    . = ALIGN(16);
    .stack (NOLOAD) : {
        . = . + 0x00010000;
        __stack_top = .;
    }
}
