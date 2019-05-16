#include "serial.h"
#include "task.h"
#include "timer.h"
#include "sseg.h"
#include "buzzer.h"
#include "ledmatrix.h"
#include "config.h"
#include "joystick.h"
#include "buttons.h"
#include <stdlib.h>

#define MAX_PROJECTILES 10
#define MAX_ASTEROIDS 10

struct Position {
	uint8_t alive;
	uint8_t x;
	uint8_t y;
};

LedMatrix game;
uint8_t score;
int8_t lives;
uint8_t running = 0;
uint8_t baseX = 2;
uint16_t speed;
struct Position projectiles[MAX_PROJECTILES];
struct Position asteroids[MAX_ASTEROIDS];

uint8_t num_projectiles() {
	uint8_t count = 0;
	for (uint8_t i = 0; i < MAX_PROJECTILES; i++) {
		if (projectiles[i].alive) count++;
	}
	return count;
}

uint8_t num_asteroids() {
	uint8_t count = 0;
	for(uint8_t i = 0; i < MAX_ASTEROIDS; i++) {
		if (asteroids[i].alive) count++;
	}
	return count;
}

void create_asteroids();
void draw_asteroids();
void move();
void init_game() {
	score = 0;
	lives = 4;
	speed = 500;
	for (uint8_t i = 0; i < MAX_PROJECTILES; i++) {
		projectiles[i].alive = 0;
	}
	
	for (uint8_t i = 0; i < MAX_ASTEROIDS; i++) {
		asteroids[i].alive = 0;
	}
	
	for (uint8_t i = 0; i < 7; i++) {
		draw_asteroids();
		create_asteroids();
		if (random() % 5 == 0) create_asteroids();
	}
	
	for (uint8_t i = 0; i < LEDMATRIX_ROWS; i++) {
		for (uint8_t j = 0; j < LEDMATRIX_COLUMNS; j++) {
			ledmatrix_set(game, j, i, LEDMATRIX_COLOR_BLACK);
		}
	}
}

void fire_projectile() {
	buzzer_up();
	for (uint8_t i = 0; i < MAX_PROJECTILES; i++) {
		if (projectiles[i].alive) {
			if (projectiles[i].x == baseX && projectiles[i].y == (LEDMATRIX_COLUMNS - 2)) return;
			continue;
		}
		
		projectiles[i].alive = 1;
		projectiles[i].x = baseX;
		projectiles[i].y = LEDMATRIX_COLUMNS - 2;
		return;
	}
}

void draw_projectiles() {
	for (uint8_t i = 0; i < MAX_PROJECTILES; i++) {
		if (projectiles[i].alive) {
			if (projectiles[i].y < LEDMATRIX_COLUMNS - 2) ledmatrix_set(game, projectiles[i].y, projectiles[i].x, LEDMATRIX_COLOR_BLACK);
			projectiles[i].y--;
			if (projectiles[i].y > LEDMATRIX_COLUMNS) {
				projectiles[i].alive = 0;
				continue;
			}
			ledmatrix_set(game, projectiles[i].y, projectiles[i].x, LEDMATRIX_COLOR_RED);
		}
	}
}

void draw_asteroids() {
	for (uint8_t i = 0; i < MAX_ASTEROIDS; i++) {
		if (asteroids[i].alive) {
			ledmatrix_set(game, asteroids[i].y, asteroids[i].x, LEDMATRIX_COLOR_BLACK);
			asteroids[i].y++;
			if (asteroids[i].y >= LEDMATRIX_COLUMNS) {
				asteroids[i].alive = 0;
				continue;
			}
			ledmatrix_set(game, asteroids[i].y, asteroids[i].x, LEDMATRIX_COLOR_GREEN);
		}
	}
}

void create_asteroids() {
	uint8_t exists = 0;
	uint8_t x;
	uint8_t pos = MAX_ASTEROIDS;
	for (uint8_t i = 0; i < MAX_ASTEROIDS; i++) {
		if (!asteroids[i].alive) {
			pos = i;
			break;
		}
	}
	
	if (pos == MAX_ASTEROIDS) return;
	
	do {
		x = random() % LEDMATRIX_ROWS;
		exists = 0;
		for (uint8_t i = 0; i < MAX_ASTEROIDS; i++) {
			if (asteroids[i].alive) {
				if (asteroids[i].y == 0 && asteroids[i].x == x) {
					exists = 1;
				}
			}
		}
	} while (exists == 1);
	
	asteroids[pos].alive = 1;
	asteroids[pos].x = x;
	asteroids[pos].y = 0;
	ledmatrix_set(game, asteroids[pos].y, asteroids[pos].x, LEDMATRIX_COLOR_GREEN);
}

void check_collisions() {
	for (uint8_t i = 0; i < MAX_ASTEROIDS; i++) {
		struct Position ast = asteroids[i];
		if (!ast.alive) continue;
		
		for (uint8_t j = 0; j < MAX_PROJECTILES; j++) {
			struct Position proj = projectiles[j];
			if (!proj.alive) continue;
			
			if (proj.x == ast.x && proj.y == ast.y) {
				asteroids[i].alive = 0;
				projectiles[j].alive = 0;
				buzzer_collide();
				score++;
				ledmatrix_set(game, proj.y, proj.x, LEDMATRIX_COLOR_BLACK);
				break;
			}
		}
		
		if (ast.y == (LEDMATRIX_COLUMNS - 1)) {
			if (ast.x >= (baseX - 1) && ast.x <= (baseX + 1)) {
				lives--;
				asteroids[i].alive = 0;
				buzzer_collide();
				move();
				continue;
			}
		}
		
		if (ast.y == (LEDMATRIX_COLUMNS - 2) && ast.x == baseX) {
			lives--;
			asteroids[i].alive = 0;
			buzzer_collide();
			move();
		}
	}
}

void update_game() {
	if (!running) return;
	draw_projectiles();
	check_collisions();
	draw_asteroids();
	for (uint8_t i = 0; i < 1; i++) {
		create_asteroids();
	}
	check_collisions();
}

inline void bounds_ship() {
	if (baseX > 200) baseX = 0;
	if (baseX > 7) baseX = 7;
}

void serial_in() {
	int c = fgetc(stdin);
	if (c != EOF) {
		if (c == 'p') running = !running;
		
		if (running) {
			if (lives <= 0) {
				init_game();
				buzzer_startup();
				running = !running;
			} else if (c == 'a') baseX -= 1;
			else if (c == 'd') baseX += 1;
			else if (c == 'w') fire_projectile();
			bounds_ship();
		}
	}
}

void buttons() {
	uint8_t left = button_pressed(BUTTON_LEFT);
	uint8_t right = button_pressed(BUTTON_RIGHT);
	uint8_t middle = button_pressed(BUTTON_UP) || button_pressed(BUTTON_DOWN);
	
	if (!running && (left || right || middle)) {
		if (lives <= 0) {
			init_game();
			buzzer_startup();
			running = 1;
		}
	} else if (running) {
		if (left) baseX -= 1;
		if (right) baseX += 1;
		if (middle) fire_projectile();
		bounds_ship();
	}	
}

void draw_ship(uint8_t x, uint8_t color) {
	if (x < 7) ledmatrix_set(game, LEDMATRIX_COLUMNS - 1, x + 1, color);
	ledmatrix_set(game, LEDMATRIX_COLUMNS - 1, x, color);
	if (x > 0) ledmatrix_set(game, LEDMATRIX_COLUMNS - 1, x - 1, color);
	ledmatrix_set(game, LEDMATRIX_COLUMNS - 2, x, color);
}

uint8_t lastShipX = 1;
void move() {
	if (lastShipX != baseX) {
		buzzer_move();
	}
	if (1) {
		draw_ship(lastShipX, LEDMATRIX_COLOR_BLACK);
		if (num_projectiles() == MAX_PROJECTILES)
			draw_ship(baseX, LEDMATRIX_COLOR_LIGHT_YELLOW);
		else
			draw_ship(baseX, LEDMATRIX_COLOR_YELLOW);
		lastShipX = baseX;
	}
}

uint16_t lastScore = 100;
int8_t lastLives = 10;
void draw_score() {
	if (lastScore != score) {
		sseg_set(score);
		serial_move(1, 1);
		printf("Score: %4d\n", score);
		lastScore = score;
	}
	if (lastLives != lives) {
		if (lives >= 0) config_set(CONFIG_GAME_LIVES, lives);
		else config_set(CONFIG_GAME_LIVES, 0);
		serial_move(1, 2);
		printf("Lives: %4d\n", lives);
		lastLives = lives;
		if (lives <= 0) {
			running = 0;
			buzzer_gameover();
			ledmatrix_set_text_color(LEDMATRIX_COLOR_RED);
			ledmatrix_scroll_text("Game Over");
		}
	}
}

void increase_speed() {
	if (!running) return;
	
	speed = 500 - (score * 5);
	task_change_time_ms(0, speed);
	task_change_time_ms(1, speed);
}

uint8_t firstDelayRun = 0;
void first_delay() {
	if (firstDelayRun) return;
	
	firstDelayRun = 1;
	running = 1;
}

int main (void)
{
	init_uart(38400);
	serial_clear();
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
	
	init_game();
	
	buzzer_startup();
	
	task_create(update_game, 500, "_game");
	task_create(draw_score, 500, "_score");
	task_create(task_buzzer, 1, "buzzer");
	task_create(task_sseg, 5, "sseg");
	task_create(task_ledmatrix, 1, "ledmatrix");
	task_create(task_joystick, 25, "joystick");
	task_create(task_button, 5, "button");
	//task_create(secondary, 5000, "secondary");
	task_create(move, 150, "move");
	task_create(buttons, 100, "buttons");
	task_create(serial_in, 50, "serial_in");
	task_create(increase_speed, 100, "speed");
	task_create(first_delay, 9000, "run");
	
	LOG("Loaded");
	
	task_run();
}
