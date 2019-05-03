#include "serial.h"
#include "task.h"
#include "timer.h"
#include "sseg.h"

uint8_t counter = 0;
void hello_world() {
	sseg_set(counter++);
}

void secondary() {
	printf("Timer: %ld\n", millis());
}

int main (void)
{
	init_uart(19200);
	printf("Loading\n");
	init_timers();
	init_sseg();
	
	task_create(hello_world, 1000, "hello_world");
	task_create(secondary, 100, "secondary");
	task_create(task_sseg, 5, "sseg");
	
	printf("Loaded\n");
	task_run();
}
