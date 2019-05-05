#pragma once

#include <avr/io.h>
#include <stdint.h>
#include "timer.h"
#include "queue.h"

typedef struct {
	uint8_t tone;
	uint8_t duration;
	uint32_t start;
} BuzzerValue;

void init_buzzer();
void buzzer_startup();
void buzzer_gameover();
void task_buzzer();