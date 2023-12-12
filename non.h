#ifndef NON_H
#define NON_H
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ncurses.h>

typedef struct Cell
{
	bool status;
	bool covered;
	unsigned int xloc;
	unsigned int yloc;
}cell_t;

typedef struct Nonogram
{
	cell_t** table;

	size_t size;
}non_t;

non_t *non_initialize(size_t size);

void non_randomize(non_t *non);

void non_print(WINDOW *win, non_t *non);

void non_delete(non_t *non);

#endif //NON_H
