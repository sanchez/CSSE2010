#pragma once

#include <stdio.h>
#include <stdint.h>
#include <avr/interrupt.h>
#include "defs.h"

#define BLACK_FOREGROUND 30
#define RED_FOREGROUND 31
#define GREEN_FOREGROUND 32
#define YELLOW_FOREGROUND 33
#define BLUE_FOREGROUND 34
#define PURPLE_FOREGROUND 35
#define TEAL_FOREGROUND 36
#define WHITE_FOREGROUND 37
#define BLACK_BACKGROUND 40
#define RED_BACKGROUND 41
#define GREEN_BACKGROUND 42
#define YELLOW_BACKGROUND 43
#define BLUE_BACKGROUND 44
#define PURPLE_BACKGROUND 45
#define TEAL_BACKGROUND 46
#define WHITE_BACKGROUND 47

void init_uart(unsigned long baudrate);
void uart_putchar(char c, FILE *stream);
int uart_getchar(FILE *stream);
uint8_t uart_available();

void serial_set_color(uint8_t color);
void serial_bold();
void serial_reset();
void serial_clear();
void serial_move(uint16_t x, uint16_t y);