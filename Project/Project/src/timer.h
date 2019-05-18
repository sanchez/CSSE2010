#pragma once

#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>

void init_timers(void);
uint32_t millis(void);
uint32_t millis_to_ticks(uint32_t);
uint32_t ticks(void);