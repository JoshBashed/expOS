#pragma once
#include "shared.h"

void driver_uart_init(void);
void driver_uart_put_char(u8 c);
void driver_uart_put_char_for_logger(u8 c);
void driver_uart_put_string(shared_StaticString s);