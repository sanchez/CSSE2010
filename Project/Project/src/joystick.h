#pragma once

#include <avr/io.h>
#include <avr/interrupt.h>
#include "serial.h"

#define JOYSTICK_X 3
#define JOYSTICK_Y 2

void init_joystick();
uint8_t joystick_read(uint8_t direction);
void task_joystick();