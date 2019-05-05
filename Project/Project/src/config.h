#pragma once

#include "avr/io.h"

#define CONFIG_BUZZER_ENABLE 1

void init_config();
uint8_t config_get(uint8_t c);