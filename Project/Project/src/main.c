#include "serial.h"
#include "task.h"
#include "timer.h"

void hello_world() {
	printf("Hello World\n");
	for(uint32_t i = 0; i < 500000; i++);
}

void secondary() {
	printf("Timer: %ld\n", millis());
}

int main (void)
{
	init_uart(19200);
	printf("Loading\n");
	init_timers();
	
	task_create(hello_world, 1000, "hello_world");
	task_create(secondary, 100, "secondary");
	
	printf("Loaded\n");
	task_run();
}
