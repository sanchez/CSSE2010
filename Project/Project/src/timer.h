#pragma once

#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>

void init_timers();
uint32_t millis();