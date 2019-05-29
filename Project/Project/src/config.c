#include "config.h"

void config_set_lives(uint8_t lives);

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

void config_set_lives(uint8_t lives) {
	switch (lives) {
		case 0:
			PORTA |= 0;
			break;
		case 1:
			PORTA |= 0b01000000;
			break;
		case 2:
			PORTA |= 0b01100000;
			break;
		case 3:
			PORTA |= 0b01110000;
			break;
		case 4:
			PORTA |= 0b11110000;
			break;
		default:
			break;
	}
}

void config_set(uint8_t c, uint8_t status) {
	switch(c) {
		case CONFIG_TASK_WARNING:
			return;
		case CONFIG_GAME_LIVES:
			PORTA &= 0x0f;
			config_set_lives(status);
	}
}