#ifndef NON_H
#define NON_H
#include <stddef.h>
#include <stdio.h>
#include <time.h>
#include <ncurses.h>
#include "clueq.h"

typedef struct Cell
{
	bool status;
	char symbol;
	unsigned int x;
	unsigned int y;
}cell_t;

typedef struct Nonogram
{
	cell_t** table;
	cluequeue_t ** colClues;
	cluequeue_t ** rowClues;
	size_t size;
	unsigned int total;
	unsigned int filled;
	char * title;
}non_t;

non_t *non_initialize(size_t size);

void non_randomize(non_t *non);

void build_from_file(non_t *non, char *file);

void non_print(WINDOW *win, non_t *non);

void non_delete(non_t *non);

void non_solve(non_t *non);

void non_clue_print_debug(WINDOW *win, non_t *non);

void non_clue_print_x(WINDOW *win, non_t *non);

void non_clue_print_y(WINDOW *win, non_t *non);

void clue_highlight_x(WINDOW *win, non_t* non, int xpos, bool hl);
void clue_highlight_y(WINDOW *win, non_t* non, int ypos, bool hl);

#endif //NON_H
