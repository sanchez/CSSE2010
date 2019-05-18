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

void init_buzzer(void);
void buzzer_startup(void);
void buzzer_gameover(void);
void buzzer_up(void);
void buzzer_move(void);
void buzzer_collide(void);
void task_buzzer(void);