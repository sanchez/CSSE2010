#pragma once

#include <avr/io.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <avr/pgmspace.h>
#include <string.h>
#include "error.h"

#define LEDMATRIX_COLOR_BLACK 0x00
#define LEDMATRIX_COLOR_RED 0x0F
#define LEDMATRIX_COLOR_GREEN 0xF0
#define LEDMATRIX_COLOR_YELLOW 0xFF
#define LEDMATRIX_COLOR_ORANGE 0x3C
#define LEDMATRIX_COLOR_LIGHT_ORANGE 0x15
#define LEDMATRIX_COLOR_LIGHT_YELLOW 0x33
#define LEDMATRIX_COLOR_LIGHT_GREEN 0x21

#define LEDMATRIX_CMD_UPDATE_ALL 0x00
#define LEDMATRIX_CMD_UPDATE_PIXEL 0x01
#define LEDMATRIX_CMD_UPDATE_ROW 0x02
#define LEDMATRIX_CMD_UPDATE_COLUMN 0x03
#define LEDMATRIX_CMD_SHIFT_DISPLAY 0x04
#define LEDMATRIX_CMD_CLEAR_SCREEN 0x0F

#define LEDMATRIX_COLUMNS 16
#define LEDMATRIX_ROWS 8

// [column][row]
typedef uint8_t** LedMatrix;

void init_ledmatrix();
void ledmatrix_set_pixel(uint8_t x, uint8_t y, uint8_t color);
void ledmatrix_set_text_color(uint8_t color);
void ledmatrix_scroll_text(char* text);
void ledmatrix_stop_text();
void ledmatrix_clear();

LedMatrix ledmatrix_create();
void ledmatrix_set(LedMatrix display, uint8_t x, uint8_t y, uint8_t color);
void ledmatrix_set_active(LedMatrix display);
void ledmatrix_draw(LedMatrix display);

void task_ledmatrix();