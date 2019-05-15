#include "error.h"

uint8_t logCount = 0;

void _error(const char* file, unsigned int line, const char* message) {
	serial_set_color(RED_FOREGROUND);
	serial_move(1, ERROR_INITIAL_LINE + logCount++);
	if (logCount >= ERROR_LOG_ROLLOVER) logCount = 0;
	printf("[%s:%d] ", file, line);
	serial_bold();
	printf(message);
	printf("\n");
	serial_reset();
	
	while (1);
}

void _warn(const char* file, unsigned int line, const char* message) {
	serial_set_color(YELLOW_FOREGROUND);
	serial_move(1, ERROR_INITIAL_LINE + logCount++);
	if (logCount >= ERROR_LOG_ROLLOVER) logCount = 0;
	printf("[%s:%d] ", file, line);
	serial_bold();
	printf(message);
	printf("\n");
	serial_reset();
}

void _log(const char* file, unsigned int line, const char* message) {
	if (!config_get(CONFIG_DEBUG_ENABLE)) return;
	serial_set_color(BLUE_FOREGROUND);
	serial_move(1, ERROR_INITIAL_LINE + logCount++);
	if (logCount >= ERROR_LOG_ROLLOVER) logCount = 0;
	printf("[%s:%d] ", file, line);
	serial_bold();
	printf(message);
	printf("\n");
	serial_reset();
}