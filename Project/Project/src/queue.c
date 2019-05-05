#include "queue.h"

Queue queue_create() {	
	Queue q = malloc(sizeof(struct Q) * 1);
	if (q == NULL) {
		// TODO: Some error handling here #1
	}
}

void queue_add(Queue q, void* d) {
	struct QueueItem* qi = malloc(sizeof(struct QueueItem) * 1);
	if (qi == NULL) {
		// TODO: Some error reporting here #1
	}
	
	qi->data = d;
	qi->next = NULL;
	
	if (q->first == NULL) {
		q->first = qi;
	} else {
		struct QueueItem* lastPos = NULL;
		struct QueueItem* pos = q->first;
		while (pos != NULL) {
			lastPos = pos;
			pos = pos->next;
		}
		qi->next = qi;
	}
	
	q->size++;
}

void* queue_remove(Queue q) {
	struct QueueItem* first = q->first;
	
	if (first == NULL) return NULL;
	
	q->first = first->next;
	void* d = first->data;
	free(first);
	return d;
}

uint8_t queue_size(Queue q) {
	return q->size;
}

void queue_iterate(Queue q) {
	q->current = q->first;
}

void* queue_next(Queue q) {
	if (q->current->next == NULL) {
		return NULL;
	}
	q->current = q->current->next;
	return q->current->data;
}