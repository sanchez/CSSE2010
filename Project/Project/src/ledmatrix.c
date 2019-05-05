#include "ledmatrix.h"

const uint8_t LEDMATRIX_LETTER_A[] = {126, 144, 144, 127};
const uint8_t LEDMATRIX_LETTER_B[] = {254, 146, 146, 109};
const uint8_t LEDMATRIX_LETTER_C[] = {124, 130, 130, 69};
const uint8_t LEDMATRIX_LETTER_D[] = {254, 130, 130, 125};
const uint8_t LEDMATRIX_LETTER_E[] = {254, 146, 146, 131};
const uint8_t LEDMATRIX_LETTER_F[] = {254, 144, 144, 129};
const uint8_t LEDMATRIX_LETTER_G[] = {124, 130, 146, 93};
const uint8_t LEDMATRIX_LETTER_H[] = {254, 16, 16, 255};
const uint8_t LEDMATRIX_LETTER_I[] = {130, 254, 131};
const uint8_t LEDMATRIX_LETTER_J[] = {4, 2, 2, 253};
const uint8_t LEDMATRIX_LETTER_K[] = {254, 16, 40, 199};
const uint8_t LEDMATRIX_LETTER_L[] = {254, 2, 2, 3};
const uint8_t LEDMATRIX_LETTER_M[] = {254, 64, 48, 64, 255};
const uint8_t LEDMATRIX_LETTER_N[] = {254, 32, 16, 255};
const uint8_t LEDMATRIX_LETTER_O[] = {124, 130, 130, 125};
const uint8_t LEDMATRIX_LETTER_P[] = {254, 144, 144, 97};
const uint8_t LEDMATRIX_LETTER_Q[] = {124, 130, 138, 124, 3};
const uint8_t LEDMATRIX_LETTER_R[] = {254, 144, 152, 103};
const uint8_t LEDMATRIX_LETTER_S[] = {100, 146, 146, 77};
const uint8_t LEDMATRIX_LETTER_T[] = {128, 128, 254, 128, 129};
const uint8_t LEDMATRIX_LETTER_U[] = {252, 2, 2, 253};
const uint8_t LEDMATRIX_LETTER_V[] = {248, 4, 2, 4, 249};
const uint8_t LEDMATRIX_LETTER_W[] = {252, 2, 28, 2, 253};
const uint8_t LEDMATRIX_LETTER_X[] = {198, 40, 16, 40, 199};
const uint8_t LEDMATRIX_LETTER_Y[] = {224, 16, 14, 16, 225};
const uint8_t LEDMATRIX_LETTER_Z[] = {134, 138, 146, 162, 195};
const uint8_t LEDMATRIX_LETTER_0[] = {124, 146, 162, 125};
const uint8_t LEDMATRIX_LETTER_1[] = {66, 254, 3};
const uint8_t LEDMATRIX_LETTER_2[] = {70, 138, 146, 99};
const uint8_t LEDMATRIX_LETTER_3[] = {68, 146, 146, 109};
const uint8_t LEDMATRIX_LETTER_4[] = {24, 40, 72, 255};
const uint8_t LEDMATRIX_LETTER_5[] = {228, 162, 162, 157};
const uint8_t LEDMATRIX_LETTER_6[] = {124, 146, 146, 77};
const uint8_t LEDMATRIX_LETTER_7[] = {128, 158, 160, 193};
const uint8_t LEDMATRIX_LETTER_8[] = {108, 146, 146, 109};
const uint8_t LEDMATRIX_LETTER_9[] = {100, 146, 146, 125};

const uint8_t* LEDMATRIX_LETTERS[] = { LEDMATRIX_LETTER_A, LEDMATRIX_LETTER_B, LEDMATRIX_LETTER_C, LEDMATRIX_LETTER_D, LEDMATRIX_LETTER_E, LEDMATRIX_LETTER_F, LEDMATRIX_LETTER_G, LEDMATRIX_LETTER_H, LEDMATRIX_LETTER_I, LEDMATRIX_LETTER_J, LEDMATRIX_LETTER_K, LEDMATRIX_LETTER_L, LEDMATRIX_LETTER_M, LEDMATRIX_LETTER_N, LEDMATRIX_LETTER_O, LEDMATRIX_LETTER_P, LEDMATRIX_LETTER_Q, LEDMATRIX_LETTER_R, LEDMATRIX_LETTER_S, LEDMATRIX_LETTER_T, LEDMATRIX_LETTER_U, LEDMATRIX_LETTER_V, LEDMATRIX_LETTER_W, LEDMATRIX_LETTER_X, LEDMATRIX_LETTER_Y, LEDMATRIX_LETTER_Z };
const uint8_t* LEDMATRIX_NUMBERS[] = { LEDMATRIX_LETTER_0, LEDMATRIX_LETTER_1, LEDMATRIX_LETTER_2, LEDMATRIX_LETTER_3, LEDMATRIX_LETTER_4, LEDMATRIX_LETTER_5, LEDMATRIX_LETTER_6, LEDMATRIX_LETTER_7, LEDMATRIX_LETTER_8, LEDMATRIX_LETTER_9 };

void init_ledmatrix() {
	DDRB |= (1 << 4) | (1 << 5) | (1 << 7);
	PORTB |= (1 << 4);
	
	SPCR0 = (1 << SPE0) | (1 << MSTR0) | (1 << SPR00) | (1 << SPR10);
	SPSR0 = (1 << SPI2X0);
	
	PORTB &= ~(1 << 4);
	
	ledmatrix_clear();
}

uint8_t send_byte(uint8_t b) {
	SPDR0 = b;
	while ((SPSR0 & (1 << SPIF0)) == 0);
	return SPDR0;
}

void ledmatrix_set_pixel(uint8_t x, uint8_t y, uint8_t color) {
	send_byte(LEDMATRIX_CMD_UPDATE_PIXEL);
	send_byte(((y & 0x07) << 4) | (x & 0x0f));
	send_byte(color);
}

void ledmatrix_shift_left() {
	send_byte(LEDMATRIX_CMD_SHIFT_DISPLAY);
	send_byte(0x02);
}

void ledmatrix_set_column(uint8_t x, uint8_t col[LEDMATRIX_ROWS]) {
	send_byte(LEDMATRIX_CMD_UPDATE_COLUMN);
	send_byte(x & 0x0f);
	for (uint8_t i = 0; i < LEDMATRIX_ROWS; i++) {
		send_byte(col[i]);
	}
}

uint8_t charData[256];
uint8_t* pos = NULL;
void ledmatrix_scroll_text(char* text) {
	memset(charData, 0, sizeof(uint8_t) * 256);
	uint8_t length = strlen(text);
	uint8_t tempPos = 0;
	for (uint8_t i = 0; i < length; i++) {
		char c = text[i];
		uint8_t* vals;
		if (c >= 'a' && c <= 'z') {
			vals = LEDMATRIX_LETTERS[c - 'a'];
		} else if (c >= 'A' && c <= 'Z') {
			vals = LEDMATRIX_LETTERS[c - 'A'];
		} else if (c >= '0' && c <= '9') {
			vals = LEDMATRIX_NUMBERS[c - '0'];
		} else {
			vals = NULL;
		}
		
		if (vals != NULL) {
			uint8_t v = 0;			
			for (uint8_t i = 0; !(v & 0x01); i++) {
				v = vals[i];
				charData[tempPos++] = v & ~0x01;
			}
			charData[tempPos++] = 0;
		}
	}
	pos = charData;
}

void ledmatrix_clear() {
	send_byte(LEDMATRIX_CMD_CLEAR_SCREEN);
}

void task_ledmatrix() {
	if (pos != NULL) {
		ledmatrix_shift_left();
		uint8_t columnData[LEDMATRIX_ROWS];
		uint8_t val = *pos;
		for (uint8_t i = 0; i < LEDMATRIX_ROWS; i++) {
			if (val & (1 << i)) {
				columnData[i] = LEDMATRIX_COLOR_GREEN;
			} else {
				columnData[i] = LEDMATRIX_COLOR_BLACK;
			}
		}
		ledmatrix_set_column(15, columnData);
		pos += 1;
		if ((pos - charData) >= 255) {
			pos = NULL;
		}
	}
}