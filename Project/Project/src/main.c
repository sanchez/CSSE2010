#include "serial.h"
#include "task.h"
#include "timer.h"
#include "sseg.h"
#include "buzzer.h"
#include "ledmatrix.h"
#include "config.h"

uint8_t counter = 0;
void hello_world() {
	sseg_set(counter++);
}

void secondary() {
	buzzer_gameover();
	ledmatrix_set_text_color(LEDMATRIX_COLOR_RED);
	ledmatrix_scroll_text("Game over");
}

int main (void)
{
	init_uart(19200);
	printf("Loading\n");
	init_ledmatrix();
	init_timers();
	init_sseg();
	init_buzzer();
	init_config();
	
	ledmatrix_scroll_text("Daniel Fitzmaurice 43961229");
	
	buzzer_startup();
	
	task_create_ticks(task_buzzer, 1, "buzzer");
	task_create(task_sseg, 5, "sseg");
	task_create(task_ledmatrix, 50, "ledmatrix");
	task_create(hello_world, 1000, "hello_world");
	task_create(secondary, 5000, "secondary");
	
	LOG("Some Log");
	WARN("Some Warning");
	ERROR("MAssive Error");
	
	printf("Loaded\n");
	task_run();
}
