#include "buttons.h"

uint8_t buttonDown[4];
uint8_t buttonPressed[4];

void init_buttons() {
	DDRB &= ~(0x0f);
}

uint8_t buttons_get(uint8_t b) {
	if (b == BUTTON_ALL) {
		return !!(PINB & (0x0f));
	}
	return !!(PINB & (1 << b));
}

uint8_t button_pressed(uint8_t b) {
	uint8_t rt = 0;
	if (b == BUTTON_ALL) {
		for (uint8_t i = 0; i < 4; i++) {
			if (buttonPressed[i]) {
				rt = 1;
				buttonPressed[i] = 0;
			}
		}
	} else {
		rt = buttonPressed[b];
		buttonPressed[b] = 0;
	}
	return rt;
}

void task_button() {
	for (uint8_t i = BUTTON_RIGHT; i <= BUTTON_LEFT; i++) {
		uint8_t down = buttons_get(i);
		if (!buttonDown[i] && down) {
			buttonPressed[i] = 1;
		}
		buttonDown[i] = down;
	}
}