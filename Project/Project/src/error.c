#include "error.h"

void _error(const char* file, unsigned int line, const char* message) {
	serial_set_color(RED_FOREGROUND);
	printf("[%s:%d] ", file, line);
	serial_bold();
	printf(message);
	printf("\n");
	serial_reset();
	
	while (1);
}

void _warn(const char* file, unsigned int line, const char* message) {
	serial_set_color(YELLOW_FOREGROUND);
	printf("[%s:%d] ", file, line);
	serial_bold();
	printf(message);
	printf("\n");
	serial_reset();
}

void _log(const char* file, unsigned int line, const char* message) {
	if (!config_get(CONFIG_DEBUG_ENABLE)) return;
	serial_set_color(BLUE_FOREGROUND);
	printf("[%s:%d] ", file, line);
	serial_bold();
	printf(message);
	printf("\n");
	serial_reset();
}