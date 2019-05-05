#include "config.h"

void init_config() {
	DDRA &= ~0xf0;
}

uint8_t config_get(uint8_t c) {
	switch (c) {
		case CONFIG_BUZZER_ENABLE:
			return !(PINA & (1 << 7));
		default: return 0;
	}
}