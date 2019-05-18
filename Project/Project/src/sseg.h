#pragma once

#include <stdio.h>
#include <avr/io.h>

void init_sseg(void);
void sseg_set(uint8_t val);
void task_sseg(void);