#pragma once

#define ITER_TYPE uint32_t

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "timer.h"

struct T {
	void (*f)(void);
	ITER_TYPE iteration;
	ITER_TYPE lastIter;
	char* name;
};

typedef struct T* Task;

void task_create(void (*f)(void), ITER_TYPE iterations, const char* name);
void task_create_ticks(void (*f)(void), ITER_TYPE iterations, const char* name);
void task_run();