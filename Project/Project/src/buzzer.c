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
	buzzer_add_tone(1, 200);
	buzzer_add_tone(0, 200);
	buzzer_add_tone(2, 200);
	buzzer_add_tone(0, 200);
	buzzer_add_tone(3, 200);
	buzzer_add_tone(0, 200);
	buzzer_add_tone(4, 200);
	buzzer_add_tone(0, 200);
	buzzer_add_tone(10, 200);
}

uint32_t buzzerCounter = 0;
void task_buzzer() {
	if (playingPos >= BUZZER_MAX_TONES) return;
	BuzzerValue* t = tones + playingPos;	
	if (t->start == 0) {
		t->start = millis();
	}
	
	uint8_t diff = millis() - t->start;
	if (diff >= t->duration) {
		t->tone = 0;
		playingPos++;
		PORTA &= ~(1 << 0);
		return;
	}
	
	uint8_t tone = t->tone;
	
	buzzerCounter++;
	if (tone != 0 && (buzzerCounter % tone) == 0) {
		PORTA ^= (1 << 0);
	}
}