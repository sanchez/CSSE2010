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

uint8_t x = 0;
uint8_t y = 0;
LedMatrix game;
void move() {	
	y++;
	if (y >= LEDMATRIX_ROWS) y = 0;
	
	if (y == 0) {
		buzzer_up();
		for (uint8_t i = 0; i < LEDMATRIX_COLUMNS; i++) {
			for (uint8_t j = 0; j < LEDMATRIX_ROWS; j++) {
				ledmatrix_set(game, i, j, LEDMATRIX_COLOR_BLACK);
			}
		}
	}
	
	for (uint8_t i = 0; i < LEDMATRIX_COLUMNS; i += 2) {
		ledmatrix_set(game, i, y, LEDMATRIX_COLOR_ORANGE);
	}
}

int main (void)
{
	init_uart(19200);
	LOG("Loading");
	init_ledmatrix();
	init_timers();
	init_sseg();
	init_buzzer();
	init_config();
	game = ledmatrix_create();
	ledmatrix_set_active(game);
	
	ledmatrix_scroll_text("Daniel Fitzmaurice 43961229");
	
	buzzer_startup();
	
	task_create_ticks(task_buzzer, 1, "buzzer");
	task_create(task_sseg, 5, "sseg");
	task_create(task_ledmatrix, 5, "ledmatrix");
	task_create(hello_world, 1000, "hello_world");
	//task_create(secondary, 5000, "secondary");
	task_create(move, 150, "move");
	
	LOG("Loaded");
	
	task_run();
}
