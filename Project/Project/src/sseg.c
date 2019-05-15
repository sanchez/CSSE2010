#include "sseg.h"

uint8_t currentVal = 0;
uint8_t leftSide = 0;
uint8_t ssegVals[10] = {63,6,91,79,102,109,125,7,127,111};

void init_sseg() {
	DDRC = 0xff;
	DDRA |= (1 << 1);
}

void sseg_set(uint8_t val) {
	currentVal = val;
	if (currentVal > 99) currentVal = 99;
}

void task_sseg() {
	if (leftSide) {
		PORTA |= (1 << 1);
		uint8_t v = currentVal / 10;
		if (v == 0) {
			PORTC = 0;
		} else if (v >= 10) {
			PORTC = ssegVals[0];
		} else {
			PORTC = ssegVals[v];
		}
		leftSide = 0;
	} else {
		PORTA &= ~(1 << 1);
		uint8_t v = currentVal % 10;
		PORTC = ssegVals[v];
		leftSide = 1;
	}
}