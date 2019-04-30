#pragma once

#include "defs.h"

#include <avr/io.h>
#include <avr/interrupt.h>

volatile unsigned long clock_ticks;

unsigned long millis();
void init_timers();