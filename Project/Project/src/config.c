#include "config.h"

void init_config() {
	DDRA &= ~0xf0;
	DDRD |= (1 << 2) | (1 << 3);
}

uint8_t config_get(uint8_t c) {
	switch (c) {
		case CONFIG_BUZZER_ENABLE:
			return !!(PINA & (1 << 7));
		case CONFIG_DEBUG_ENABLE:
			return !!(PINA & (1 << 4));
		default: return 0;
	}
}

void config_set(uint8_t c, uint8_t status) {
	switch(c) {
		case CONFIG_TASK_WARNING:
			if (!!status) {
				PORTD |= (1 << 3);
			} else {
				PORTD &= ~(1 << 3);
			}
			return;
	}
}