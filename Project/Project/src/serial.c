#include "serial.h"

FILE uartOutput = FDEV_SETUP_STREAM(uart_putchar, NULL, _FDEV_SETUP_WRITE);
FILE uartInput = FDEV_SETUP_STREAM(NULL, uart_getchar, _FDEV_SETUP_READ);

void init_uart(unsigned long baudrate) {
	unsigned int baudPrescale = ((F_CPU / (8UL * baudrate)) + 1) / 2 - 1;
	
	UBRR0 = baudPrescale;
	UCSR0B |= (1 << RXEN0) | (1 << TXEN0);
	UCSR0C |= (1 << UCSZ00) | (1 << UCSZ01);
	
	stdout = &uartOutput;
	stdin = &uartInput;
	stderr = &uartOutput;
}

int uart_putchar(char c, FILE *stream) {
	while ((UCSR0A & (1 << UDRE0)) == 0);
	UDR0 = c;
	if (c == '\n') {
		uart_putchar('\r', NULL);
	}
	return 0;
}

int uart_getchar(FILE *stream) {
	if (UCSR0A & (1 << RXC0)) {
		return UDR0;
	}
	return EOF;
}

uint8_t uart_available() {
	if (UCSR0A & (1 << RXC0)) {
		return 0;
	} else {
		return 1;
	}
}

void serial_set_color(uint8_t color) {
	printf("\033[%dm", color);
}

void serial_bold() {
	printf("\033[1m");
}

void serial_reset() {
	printf("\033[0m");
}

void serial_clear() {
	printf("\x1b[2J");
}

void serial_move(uint16_t x, uint16_t y) {
	printf("\x1b[%d;%dH", y, x);
}