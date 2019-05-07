#pragma once

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "error.h"

typedef struct QueueItem {
	void* data;
	struct Queue* next;
};

typedef struct Q {
	struct QueueItem* first;
	struct QueueItem* current;
	uint8_t size;
};

typedef struct Q* Queue;

Queue queue_create();
void queue_add(Queue q, void* d);
void* queue_remove(Queue q);
uint8_t queue_size(Queue q);
void queue_iterate(Queue q);
void* queue_next(Queue q);
