#pragma once

#include <avr/io.h>
#include <stdint.h>
#include "timer.h"
#include "config.h"
#include "serial.h"

typedef struct {
	uint8_t tone;
	uint16_t duration;
	uint32_t start;
} BuzzerValue;

void init_buzzer();
void buzzer_startup();
void buzzer_gameover();
void buzzer_up();
void buzzer_move();
void task_buzzer();