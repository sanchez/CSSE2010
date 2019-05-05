#include "task.h"

Task tasks[20];
uint8_t tasksPos;

void task_create_ticks(void (*f)(void), ITER_TYPE iterations, const char* name) {
	Task t = malloc(sizeof(struct T) * 1);
	if (t == NULL) {
		// TODO: Error handling here #1
	}
	
	t->f = f;
	t->iteration = iterations;
	t->name = malloc(sizeof(char) * (strlen(name) + 1));
	strcpy(t->name, name);
	printf("Loaded task: %s\n", t->name);
	
	tasks[tasksPos++] = t;
}

void task_create(void (*f)(void), ITER_TYPE iterations, const char* name) {
	task_create_ticks(f, millis_to_ticks(iterations), name);
}

void task_run() {
	uint32_t lastRun = ticks();
	
	for (uint8_t i = 0; i < tasksPos; i++) {
		tasks[i]->lastIter = lastRun;
	}
	
	while(1) {
		
		uint32_t currentTime = ticks();
		for (uint8_t i = 0; i < tasksPos; i++) {
			Task t = tasks[i];
			if (t->iteration <= (currentTime - t->lastIter)) {
				uint32_t before = ticks();
				t->f();
				t->lastIter = currentTime;
				uint32_t after = ticks();
				if ((after - before) >= millis_to_ticks(1)) {
					// TODO: Warning about time here #1
					printf("Task overtime: %s\n", t->name);
				}
			}
		}
		lastRun = currentTime;
	}
}