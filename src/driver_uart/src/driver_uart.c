#include "driver_uart.h"

#define UART0_BASE 0x09000000UL
#define UART0_DR (*(volatile unsigned int *)(UART0_BASE + 0x00))
#define UART0_FR (*(volatile unsigned int *)(UART0_BASE + 0x18))

void driver_uart_init(void) {}

void driver_uart_put_char(u8 c) {
    while (UART0_FR & (1 << 5)) {
    } // TXFF bit (FIFO full)
    UART0_DR = (unsigned int)c;
}

void driver_uart_put_char_for_logger(u8 c) {
    if (c == '\n')
        driver_uart_put_char('\r');
    driver_uart_put_char(c);
}

void driver_uart_put_string(shared_StaticString s) {
    for (usize i = 0; i < s.length; i++) {
        if (s.str[i] == '\n')
            driver_uart_put_char('\r');
        driver_uart_put_char(s.str[i]);
    }
}
