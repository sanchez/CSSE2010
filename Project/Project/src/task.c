#include "task.h"

Task tasks[TASK_MAX_TASKS];
uint8_t tasksPos;

void task_create_ticks(void (*f)(void), ITER_TYPE iterations, const char* name) {
	if (tasksPos >= TASK_MAX_TASKS) {
		ERROR("Max Tasks Exceeded");
	}
	
	Task t = malloc(sizeof(struct T) * 1);
	if (t == NULL) {
		ERROR("Memory Error");
	}
	
	t->f = f;
	t->iteration = iterations;
	t->name = malloc(sizeof(char) * (strlen(name) + 1));
	strcpy(t->name, name);
	char s[100];
	sprintf(s, "Loaded Task: %s", t->name);
	LOG(s);
	
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
		config_set(CONFIG_TASK_WARNING, 0);
		
		uint32_t currentTime = ticks();
		for (uint8_t i = 0; i < tasksPos; i++) {
			Task t = tasks[i];
			if (t->iteration <= (currentTime - t->lastIter)) {
				uint32_t before = ticks();
				t->f();
				t->lastIter = currentTime;
				uint32_t after = ticks();
				if ((after - before) >= millis_to_ticks(1) && config_get(CONFIG_DEBUG_ENABLE) && t->name[0] != '_') {
					if (config_get(CONFIG_DEBUG_ENABLE)) {
						char s[100];
						sprintf(s, "Task overtime: %s (%d ticks)", t->name, after - before);
						WARN(s);
					}
					config_set(CONFIG_TASK_WARNING, 1);
				}
			}
		}
		if (config_get(CONFIG_DEBUG_ENABLE)) {
			uint32_t diff = ticks() - currentTime;
			if (diff >= millis_to_ticks(2)) {
				char s[100];
				sprintf(s, "Behind (%d)", diff);
				WARN(s);
			}
		}
		lastRun = ticks();
	}
}