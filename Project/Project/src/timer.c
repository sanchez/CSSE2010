#include "timer.h"

static volatile uint32_t clockTicks;

ISR(TIMER0_COMPA_vect) {
	clockTicks++;
}

void init_timers() {
	clockTicks = 0;
	
	TCNT0 = 0;
	OCR0A = 124;
	TCCR0A = (1 << WGM01);
	TCCR0B = (1 << CS01) | (1 << CS00);
	TIMSK0 |= (1 << OCIE0A);
	TIFR0 &= (1 << OCF0A);
}

uint32_t millis() {
	uint32_t rt;
	cli();
	rt = clockTicks;
	sei();
	return rt;
}