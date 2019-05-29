#include "defs.h"
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
#include <util/delay.h>

#define MAX_PROJECTILES 10
#define MAX_ASTEROIDS 10
#define MAX_ANIMATIONS 3

#define ANIMATION_TYPE_ASTEROID 1
#define ANIMATION_TYPE_BASE 2

struct Position {
	uint8_t alive;
	uint8_t x;
	uint8_t y;
	uint8_t counterIter;
	uint8_t stepCount;
};

struct Animation {
	uint8_t alive;
	uint8_t x;
	uint8_t y;
	uint8_t type;
	uint8_t step;
};

LedMatrix game;
uint8_t score;
int8_t lives;
uint8_t running = 0;
uint8_t baseX = 2;
uint16_t speed;
struct Position projectiles[MAX_PROJECTILES];
struct Position asteroids[MAX_ASTEROIDS];
struct Animation animations[MAX_ANIMATIONS];

void create_asteroids(void);
void draw_asteroids(void);
void move(void);
uint8_t num_projectiles(void);
uint8_t num_asteroids(void);
void init_game(void);
void fire_projectile(void);
void draw_projectiles(void);
void check_collisions(void);
void update_game_asteroids(void);
void update_game_projectiles(void);
inline void bounds_ship(void);
void serial_in(void);
void buttons(void);
void joystick(void);
void draw_ship(uint8_t x, uint8_t color);
void move(void);
void draw_score(void);
void increase_speed(void);
void first_delay(void);

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
	
	for (uint8_t i = 0; i < MAX_ANIMATIONS; i++) {
		animations[i].alive = 0;
	}
	
	for (uint8_t i = 0; i < LEDMATRIX_ROWS; i++) {
		for (uint8_t j = 0; j < LEDMATRIX_COLUMNS; j++) {
			ledmatrix_set(game, j, i, LEDMATRIX_COLOR_BLACK);
		}
	}
	
	for (uint8_t i = 0; i < 7; i++) {
		draw_asteroids();
		create_asteroids();
		if (random() % 5 == 0) create_asteroids();
	}
	
	serial_move(1, 1);
	printf("Daniel Fitzmaurice (43961229)\n");
}

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

void create_animation(uint8_t x, uint8_t y, uint8_t type) {
	for (uint8_t i = 0; i < MAX_ANIMATIONS; i++) {
		if (animations[i].alive) continue;
		
		animations[i].alive = 1;
		animations[i].x = x;
		animations[i].y = y;
		animations[i].step = 0;
		animations[i].type = type;
		return;
	}
}

void draw_animations() {
	for (uint8_t i = 0; i < MAX_ANIMATIONS; i++) {
		if (animations[i].alive) {
			uint8_t minX = animations[i].x - 1;
			if (animations[i].x == 0) minX = 0;
			uint8_t maxX = animations[i].x + 1;
			if (animations[i].x == LEDMATRIX_ROWS - 1) maxX = LEDMATRIX_ROWS - 1;
			
			uint8_t minY = animations[i].y - 1;
			if (animations[i].y == 0) minY = 0;
			uint8_t maxY = animations[i].y + 1;
			if (animations[i].y == LEDMATRIX_COLUMNS - 1) maxY = LEDMATRIX_COLUMNS - 1;
			
			for (uint8_t j = minX; j <= maxX; j++) {
				for (uint8_t k = minY; k <= maxY; k++) {
					if (ledmatrix_get(game, k, j) == LEDMATRIX_COLOR_ORANGE) {
						ledmatrix_set(game, k, j, LEDMATRIX_COLOR_BLACK);
					}
				}
			}
		}
	}
	for (uint8_t i = 0; i < MAX_ANIMATIONS; i++) {
		if (!animations[i].alive) continue;
		
		if (animations[i].step >= 8) {
			animations[i].alive = 0;
			continue;
		}
		
		if (animations[i].type == ANIMATION_TYPE_ASTEROID) {
			uint8_t x = animations[i].x;
			uint8_t y = animations[i].y;
			switch (animations[i].step) {
				case 0:
					ledmatrix_set(game, y, x, LEDMATRIX_COLOR_ORANGE);
					break;
				case 1:
					ledmatrix_set(game, y+1, x+1, LEDMATRIX_COLOR_ORANGE);
					ledmatrix_set(game, y+1, x-1, LEDMATRIX_COLOR_ORANGE);
					ledmatrix_set(game, y-1, x+1, LEDMATRIX_COLOR_ORANGE);
					ledmatrix_set(game, y-1, x-1, LEDMATRIX_COLOR_ORANGE);
					break;
				case 2:
					ledmatrix_set(game, y+1, x, LEDMATRIX_COLOR_ORANGE);
					ledmatrix_set(game, y-1, x, LEDMATRIX_COLOR_ORANGE);
					ledmatrix_set(game, y, x+1, LEDMATRIX_COLOR_ORANGE);
					ledmatrix_set(game, y, x-1, LEDMATRIX_COLOR_ORANGE);
					break;
				case 3:
					ledmatrix_set(game, y, x, LEDMATRIX_COLOR_ORANGE);
					break;
				case 4:
					ledmatrix_set(game, y+1, x+1, LEDMATRIX_COLOR_ORANGE);
					ledmatrix_set(game, y+1, x-1, LEDMATRIX_COLOR_ORANGE);
					ledmatrix_set(game, y-1, x+1, LEDMATRIX_COLOR_ORANGE);
					ledmatrix_set(game, y-1, x-1, LEDMATRIX_COLOR_ORANGE);
					ledmatrix_set(game, y+1, x, LEDMATRIX_COLOR_ORANGE);
					ledmatrix_set(game, y-1, x, LEDMATRIX_COLOR_ORANGE);
					ledmatrix_set(game, y, x+1, LEDMATRIX_COLOR_ORANGE);
					ledmatrix_set(game, y, x-1, LEDMATRIX_COLOR_ORANGE);
					break;
				case 5:
					ledmatrix_set(game, y+1, x+1, LEDMATRIX_COLOR_ORANGE);
					ledmatrix_set(game, y+1, x-1, LEDMATRIX_COLOR_ORANGE);
					ledmatrix_set(game, y-1, x+1, LEDMATRIX_COLOR_ORANGE);
					ledmatrix_set(game, y-1, x-1, LEDMATRIX_COLOR_ORANGE);
					ledmatrix_set(game, y+1, x, LEDMATRIX_COLOR_ORANGE);
					ledmatrix_set(game, y-1, x, LEDMATRIX_COLOR_ORANGE);
					ledmatrix_set(game, y, x+1, LEDMATRIX_COLOR_ORANGE);
					ledmatrix_set(game, y, x-1, LEDMATRIX_COLOR_ORANGE);
					ledmatrix_set(game, y, x, LEDMATRIX_COLOR_ORANGE);
			}
		}
		animations[i].step++;
	}
}

void draw_projectiles() {
	for (uint8_t i = 0; i < MAX_PROJECTILES; i++) {
		if (projectiles[i].alive) {
			if (projectiles[i].y < LEDMATRIX_COLUMNS - 2 && ledmatrix_get(game, projectiles[i].y, projectiles[i].x) == LEDMATRIX_COLOR_RED) {
				ledmatrix_set(game, projectiles[i].y, projectiles[i].x, LEDMATRIX_COLOR_BLACK);
			}
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
			if (ledmatrix_get(game, asteroids[i].y, asteroids[i].x) == LEDMATRIX_COLOR_GREEN)
				ledmatrix_set(game, asteroids[i].y, asteroids[i].x, LEDMATRIX_COLOR_BLACK);
		}
	}
	for (uint8_t i = 0; i < MAX_ASTEROIDS; i++) {
		if (asteroids[i].alive) {
			asteroids[i].stepCount++;
			if (asteroids[i].stepCount >= asteroids[i].counterIter) {
				asteroids[i].stepCount = 0;
				// ledmatrix_set(game, asteroids[i].y, asteroids[i].x, LEDMATRIX_COLOR_BLACK);
				
				uint8_t speedInfront = asteroids[i].counterIter;
				for (uint8_t j = 0; j < MAX_ASTEROIDS; j++) {
					if (!asteroids[j].alive) continue;
					if (asteroids[j].x == asteroids[i].x && asteroids[j].y == (asteroids[i].y + 2)) {
						speedInfront = asteroids[j].counterIter;
					}
				}
				asteroids[i].counterIter = speedInfront;
				
				asteroids[i].y++;
				if (asteroids[i].y >= LEDMATRIX_COLUMNS) {
					asteroids[i].alive = 0;
					continue;
				}
				//ledmatrix_set(game, asteroids[i].y, asteroids[i].x, LEDMATRIX_COLOR_GREEN);
			}
		}
	}
	for (uint8_t i = 0; i < MAX_ASTEROIDS; i++) {
		if (asteroids[i].alive) {
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
	
	uint8_t cycles = 0;
	do {
		cycles++;
		x = random() % LEDMATRIX_ROWS;
		exists = 0;
		for (uint8_t i = 0; i < MAX_ASTEROIDS; i++) {
			if (asteroids[i].alive) {
				if (asteroids[i].y == 0 && asteroids[i].x == x) {
					exists = 1;
				}
			}
		}
	} while (exists == 1 && cycles < 10);
	
	if (cycles >= 10) return;
	
	asteroids[pos].alive = 1;
	asteroids[pos].x = x;
	asteroids[pos].y = 0;
	asteroids[pos].counterIter = (random() % 10) + 1;
	asteroids[pos].stepCount = 0;
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
				create_animation(proj.x, proj.y, ANIMATION_TYPE_ASTEROID);
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

void update_game_projectiles() {
	if (!running) return;
	draw_projectiles();
	check_collisions();
}

void update_game_asteroids() {
	if (!running) return;
	draw_asteroids();
	for (uint8_t i = 0; i < 2; i++) {
		create_asteroids();
	}
	check_collisions();
}

void update_game_animations() {
	if (!running) return;
	draw_animations();
}

inline void bounds_ship() {
	if (baseX > 200) baseX = 0;
	if (baseX > 7) baseX = 7;
}

uint8_t lastCharESC = 0;
uint8_t row = 5;
void serial_in() {
	int c = fgetc(stdin);
	if (c != EOF) {
		if (c == 91) {
			lastCharESC = 1;
			return;
		}
		
		if (c == 'p' || c == 'P') {
			int pauseC;
			cli();
			buzzer_pause();
			do {
				pauseC = fgetc(stdin);
				task_sseg();
				_delay_ms(5);
			} while (pauseC != 'p' && pauseC != 'P');
			sei();
			//running = !running;
		}
		
		if (running) {
			if (lives <= 0) {
				init_game();
				buzzer_startup();
				running = !running;
			} else if (c == 'a') baseX -= 1;
			else if (c == 'l' || c == 'L') baseX -= 1;
			else if (lastCharESC == 1 && c == 68) baseX -= 1;
			else if (c == 'd') baseX += 1;
			else if (c == 'r' || c == 'R') baseX += 1;
			else if (lastCharESC == 1 && c == 67) baseX += 1;
			else if (c == 'w') fire_projectile();
			else if (c == ' ') fire_projectile();
			else if (lastCharESC == 1 && c == 65) fire_projectile();
			bounds_ship();
		}
		
		lastCharESC = 0;
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
		}
		running = 1;
	} else if (running) {
		if (left) baseX -= 1;
		if (right) baseX += 1;
		if (middle) fire_projectile();
		bounds_ship();
	}	
}

void joystick() {
	uint8_t x = joystick_read(JOYSTICK_X);
	uint8_t y = joystick_read(JOYSTICK_Y);
	if (running) {
		if (x <= 85) baseX -= 1;
		if (x >= 170) baseX += 1;
		if (y <= 85) fire_projectile();
		if (y >= 170) fire_projectile();
	}
	bounds_ship();
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
		serial_move(1, 2);
		printf("Score: %4d\n", score);
		lastScore = score;
	}
	if (lastLives != lives) {
		if (lives >= 0) config_set(CONFIG_GAME_LIVES, lives);
		else config_set(CONFIG_GAME_LIVES, 0);
		serial_move(1, 3);
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
	
	uint8_t s = score;
	if (s >= 20) s = 19;
	
	speed = 100 - s;
	task_change_time_ms(0, speed);
	task_change_time_ms(1, speed);
	task_change_time_ms(2, speed);
}

uint8_t firstDelayRun = 0;
void first_delay() {
	if (firstDelayRun) return;
	
	firstDelayRun = 1;
	running = 0;
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
	
	task_create(update_game_asteroids, 100, "_ast");
	task_create(update_game_projectiles, 500, "proj");
	task_create(update_game_animations, 250, "anim");
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
	task_create(joystick, 200, "jmove");
	task_create(first_delay, 9000, "run");
	
	LOG("Loaded");
	
	task_run();
}
