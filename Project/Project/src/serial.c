#include "serial.h"

FILE uartOutput = FDEV_SETUP_STREAM(uart_putchar, NULL, _FDEV_SETUP_WRITE);
FILE uartInput = FDEV_SETUP_STREAM(NULL, uart_getchar, _FDEV_SETUP_READ);

void init_uart(unsigned long baudrate) {
	unsigned int baudPrescale = (F_CPU / (8UL * baudrate)) - 1;
	
	UCSR0B |= (1 << RXEN0) | (1 << TXEN0);
	UCSR0C |= (1 << UCSZ00) | (1 << UCSZ01);
	UBRR0H = (baudPrescale >> 8);
	UBRR0L = baudPrescale;
	
	stdout = &uartOutput;
	stdin = &uartInput;
	stderr = &uartOutput;
}

void uart_putchar(char c, FILE *stream) {
	if (c == '\n') {
		uart_putchar('\r', NULL);
	}
	while ((UCSR0A & (1 << UDRE0)) == 0);
	UDR0 = c;
}

char uart_getchar(FILE *stream) {
	while (!(UCSR0A & (1 << RXC0)));
	return UDR0;
}

uint8_t uart_available() {
	if (UCSR0A & (1 << RXC0)) {
		return 0;
	} else {
		return 1;
	}
}