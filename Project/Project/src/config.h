#pragma once

#include "avr/io.h"

#define CONFIG_BUZZER_ENABLE 1
#define CONFIG_DEBUG_ENABLE 2
#define CONFIG_TASK_WARNING 3
#define CONFIG_GAME_LIVES 4

void init_config();
uint8_t config_get(uint8_t c);
void config_set(uint8_t c, uint8_t status);