#include "timer.h"

static volatile uint32_t clockTicks;

ISR(TIMER0_COMPA_vect) {
	clockTicks++;
}

void init_timers() {
	clockTicks = 0;
	
	TCNT0 = 0;
	OCR0A = 10; // 124;
	TCCR0A = (1 << WGM01);
	TCCR0B = (1 << CS01) | (0 << CS00);
	TIMSK0 |= (1 << OCIE0A);
	TIFR0 &= (1 << OCF0A);
}

uint32_t millis() {
	return ticks() / 100;
}

uint32_t ticks() {
	uint32_t rt;
	cli();
	rt = clockTicks;
	sei();
	return rt;
}

uint32_t millis_to_ticks(uint32_t mills) {
	return mills * 100;
}