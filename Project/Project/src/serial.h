#pragma once

#include <stdio.h>
#include <stdint.h>
#include <avr/interrupt.h>
#include "defs.h"

void init_uart(unsigned long baudrate);
void uart_putchar(char c, FILE *stream);
char uart_getchar(FILE *stream);
uint8_t uart_available();