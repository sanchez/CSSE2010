#include "config.h"

void init_config() {
	DDRA |= 0xf0;
	DDRD &= ~((1 << 2) | (1 << 3));
}

uint8_t config_get(uint8_t c) {
	switch (c) {
		case CONFIG_BUZZER_ENABLE:
			return !!(PIND & (1 << 3));
		case CONFIG_DEBUG_ENABLE:
			return !!(PIND & (1 << 2));
		default: return 0;
	}
}

void config_set(uint8_t c, uint8_t status) {
	switch(c) {
		case CONFIG_TASK_WARNING:
			return;
		case CONFIG_GAME_LIVES:
			PORTA &= 0x0f;
			for (uint8_t i = 0; i < status; i++) {
				PORTA |= (1 << (i + 4));
			}
	}
}