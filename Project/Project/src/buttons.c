#include "buttons.h"

void init_buttons() {
	DDRB &= ~(0x0f);
}

uint8_t buttons_get(uint8_t b) {
	if (b == BUTTON_ALL) {
		return !!(PINB & (0x0f));
	}
	return !!(PINB & (1 << b));
}