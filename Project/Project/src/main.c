#include "serial.h"
#include "task.h"
#include "timer.h"
#include "sseg.h"
#include "buzzer.h"
#include "ledmatrix.h"

uint8_t counter = 0;
void hello_world() {
	printf("Hello\n");
	sseg_set(counter++);
}

void secondary() {
	buzzer_gameover();
}

int main (void)
{
	init_uart(19200);
	printf("Loading\n");
	init_ledmatrix();
	init_timers();
	init_sseg();
	init_buzzer();
	
	ledmatrix_scroll_text("1234567890qwertyuiopasdfghjklzxcvbnm");
	ledmatrix_set_pixel(5, 5, LEDMATRIX_COLOR_GREEN);
	
	buzzer_startup();
	
	task_create_ticks(task_buzzer, 1, "buzzer");
	task_create(task_sseg, 5, "sseg");
	task_create(task_ledmatrix, 50, "ledmatrix");
	task_create(hello_world, 1000, "hello_world");
	task_create(secondary, 5000, "secondary");
	
	printf("Loaded\n");
	task_run();
}
