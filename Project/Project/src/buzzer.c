#include "buzzer.h"

#define BUZZER_MAX_TONES 10

BuzzerValue tones[BUZZER_MAX_TONES];
uint8_t addingPos = 0;
uint8_t playingPos = 0;

void init_buzzer() {
	DDRA |= (1 << 0);
	
	for (uint8_t i = 0; i < BUZZER_MAX_TONES; i++) {
		tones[i].tone = 0;
	}	

	DDRD |= (1 << 4) | (1 << 5);
	TCCR1A = (0 << COM1A1) | (1 << COM1A0) | (0 << COM1B1) | (0 << COM1B0);
	TCCR1B = (1 << WGM12) | (0 << CS12) | (1 << CS11) | (0 << CS10);
	OCR1A = 0;
}

void buzzer_add_tone(uint8_t t, uint8_t duration) {
	tones[addingPos].duration = duration;
	tones[addingPos].tone = t;
	tones[addingPos].start = 0;
	addingPos++;
	if (addingPos >= BUZZER_MAX_TONES) addingPos = 0;
}

void buzzer_startup() {
	addingPos = 0;
	playingPos = 0;
	buzzer_add_tone(5, 100);
	buzzer_add_tone(2, 100);
	buzzer_add_tone(5, 100);
	buzzer_add_tone(7, 300);
	buzzer_add_tone(1, 100);
	buzzer_add_tone(10, 300);
}

void buzzer_gameover() {
	addingPos = 0;
	playingPos = 0;
	buzzer_add_tone(10, 200);
	buzzer_add_tone(0, 200);
	buzzer_add_tone(4, 200);
	buzzer_add_tone(0, 200);
	buzzer_add_tone(3, 200);
	buzzer_add_tone(0, 200);
	buzzer_add_tone(2, 200);
	buzzer_add_tone(0, 200);
	buzzer_add_tone(1, 200);
}

void buzzer_up() {
	addingPos = 0;
	playingPos = 0;
	buzzer_add_tone(3, 10);
	buzzer_add_tone(2, 10);
	buzzer_add_tone(1, 10);
}

void task_buzzer() {
	if (!config_get(CONFIG_BUZZER_ENABLE)) {
		OCR1A = 0;
		return;
	}
	
	if (playingPos >= BUZZER_MAX_TONES) {
		OCR1A = 0;
		return;
	}
	BuzzerValue* t = tones + playingPos;	
	if (t->start == 0) {
		t->start = millis();
	}
	
	uint8_t diff = millis() - t->start;
	if (diff >= t->duration) {
		t->tone = 0;
		playingPos++;
		OCR1A = 0;
		return;
	}
	
	uint8_t tone = t->tone;

	OCR1A = tone * 10;
}