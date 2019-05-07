#include "joystick.h"

uint8_t joystick_reading_x = 0;
uint8_t joystick_reading_y = 0;

void init_joystick() {
	ADMUX = (1 << REFS0) | (JOYSTICK_Y & 0x0f);
	ADCSRA = (1 << ADEN) | (1 << ADSC) | (1 << ADPS2) | (1 << ADPS1) | (0 << ADPS0);
}

uint8_t joystick_read(uint8_t direction) {
	if (direction == JOYSTICK_X) {
		return joystick_reading_x;
	} else {
		return joystick_reading_y;
	}
}

uint8_t lastDirection = JOYSTICK_Y;
void task_joystick() {
	if ((ADCSRA & (1 << ADSC)) == 0) {
		uint16_t reading = ADC / 4;
		if (lastDirection == JOYSTICK_X) {
			joystick_reading_x = reading;
			ADMUX = (1 << REFS0) | (JOYSTICK_Y & 0x0f);
			lastDirection = JOYSTICK_Y;
		} else {
			joystick_reading_y = 255 - reading;
			ADMUX = (1 << REFS0) | (JOYSTICK_X & 0x0f);
			lastDirection = JOYSTICK_X;
		}
		ADCSRA |= (1 << ADSC);
	}
}