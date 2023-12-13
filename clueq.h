#ifndef CLUEQ_H
#define CLUEQ_H
#include <stdlib.h>
#include <stdbool.h>
typedef struct Clue
{
	int value;
	struct Clue *next;
}clue_t;

typedef struct ClueQueue
{
	clue_t *head;
	clue_t *tail;
}cluequeue_t;

bool clue_insert(cluequeue_t *cq, int val);
void clue_delete(cluequeue_t *cq);
cluequeue_t * clue_initialize();
#endif //CLUEQ_H
