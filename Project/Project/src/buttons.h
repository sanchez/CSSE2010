#pragma once

#include <avr/io.h>

#define BUTTON_RIGHT 0
#define BUTTON_UP 1
#define BUTTON_DOWN 2
#define BUTTON_LEFT 3
#define BUTTON_ALL 4

void init_buttons();
uint8_t buttons_get(uint8_t b);