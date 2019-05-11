#include "serial.h"
#include "task.h"
#include "timer.h"
#include "sseg.h"
#include "buzzer.h"
#include "ledmatrix.h"
#include "config.h"
#include "joystick.h"

uint8_t counter = 0;
void hello_world() {
	sseg_set(counter++);
}

void secondary() {
	buzzer_gameover();
	ledmatrix_set_text_color(LEDMATRIX_COLOR_RED);
	ledmatrix_scroll_text("Game over");
}

void serial_in() {
	int c = fgetc(stdin);
	if (c != EOF) {
		printf("%c", c);
	}
}

uint8_t posX = 5;
uint8_t posY = 5;
void joysticks() {
	uint8_t jX = joystick_read(JOYSTICK_X);
	uint8_t jY = joystick_read(JOYSTICK_Y);
	
	if (jX >= 130) {
		posX += 1;
		if (posX >= LEDMATRIX_ROWS) posX = LEDMATRIX_ROWS;
	}
	if (jX <= 120) {
		posX -= 1;
		if (posX < 1) posX = 1;
	}
	
	if (jY >= 130) {
		posY += 1;
		if (posY >= LEDMATRIX_COLUMNS) posY = LEDMATRIX_COLUMNS;
	}
	if (jY <= 120) {
		posY -= 1;
		if (posY < 1) posY = 1;
	}
}

uint8_t lastPosX = 5;
uint8_t lastPosY = 5;
LedMatrix game;
void move() {	
	ledmatrix_set(game, lastPosY, lastPosX, LEDMATRIX_COLOR_BLACK);
	lastPosX = posX - 1;
	lastPosY = posY - 1;
	ledmatrix_set(game, lastPosY, lastPosX, LEDMATRIX_COLOR_ORANGE);
}

int main (void)
{
	init_uart(38400);
	LOG("Loading");
	init_ledmatrix();
	init_timers();
	init_sseg();
	init_buzzer();
	init_config();
	init_joystick();
	game = ledmatrix_create();
	ledmatrix_set_active(game);
	
	ledmatrix_scroll_text("Daniel Fitzmaurice 43961229");
	
	buzzer_startup();
	
	task_create(task_buzzer, 1, "buzzer");
	task_create(task_sseg, 5, "sseg");
	task_create(task_ledmatrix, 1, "ledmatrix");
	task_create(task_joystick, 25, "joystick");
	task_create(hello_world, 1000, "hello_world");
	//task_create(secondary, 5000, "secondary");
	task_create(move, 150, "move");
	task_create(joysticks, 100, "joysticks");
	task_create(serial_in, 50, "serial_in");
	
	LOG("Loaded");
	
	task_run();
}
