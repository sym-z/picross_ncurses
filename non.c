#include "non.h"

non_t* non_initialize(size_t size)
{
	non_t *non = (non_t *) malloc(sizeof(non_t));
	non -> size = size;
	if(non == NULL) return NULL;

	//Initialize Rows
	non -> table = (cell_t **) malloc(sizeof(cell_t *) * size);
	if(non -> table == NULL)
	{
		free(non);
		return NULL;
	}

	for(size_t i = 0; i < size; i++)
	{
		non -> table[i] = (cell_t *) malloc(sizeof(cell_t) * size);
		if(non -> table[i] == NULL)
		{
			for (size_t j = 0; j < i; j++)
			{
				free(non -> table[j]);
			}
			free(non -> table);
			free(non);
			return NULL;
		}
	}
	//Allocate space for the clue arrays
	non -> colClues = (cluequeue_t **) malloc(sizeof(cluequeue_t *) * size);
	non -> rowClues = (cluequeue_t **) malloc(sizeof(cluequeue_t *) * size);

	if(non -> colClues == NULL || non -> rowClues == NULL)
	{
		//One of these might not be null
		free(non -> colClues);
		free(non -> rowClues);
		for(size_t i = 0; i < size; i++)
		{
			free(non -> table[i]);
		}
		free(non -> table);
		free(non);
		return NULL;
	}

	for(size_t i = 0; i < size; i++)
	{
		non -> colClues[i] = clue_initialize();
		non -> rowClues[i] = clue_initialize();
		if(non -> colClues[i] == NULL || non -> rowClues[i] == NULL)
		{
			for (size_t j = 0; j < i; j++)
			{
				free(non -> colClues[j]);
				free(non -> rowClues[j]);
			}
			free(non -> rowClues);
			free(non -> colClues);
			for(size_t i = 0; i < size; i++)
			{
				free(non -> table[i]);
			}
			free(non -> table);
			free(non);
			return NULL;
		}
	}

	//Identify symbols that are going into the table
	for(size_t i = 0; i < non -> size; i++)
	{
		for(size_t j = 0; j < non -> size; j++)
		{
			non -> table[i][j].symbol = '?';
			non -> table[i][j].x = j;
			non -> table[i][j].y = i;
		}
	}
	return non;
}

void non_randomize(non_t *non)
{
	srand(time(NULL));

	for(size_t i = 0; i < non -> size; i++)
	{
		for(size_t j = 0; j < non -> size; j++)
		{
			non -> table[i][j].status = rand() % 2;
		}
	}
}

void non_print(WINDOW *win, non_t * non)
{
	if(win == NULL)
	{
		for(size_t i = 0; i < non -> size; i++)
		{
			for(size_t j = 0; j < non -> size; j++)
			{
					printw("%c",non->table[i][j].symbol);
			}
			printw("\n");
		}
	}
	else	
	{
		for(size_t i = 0; i < non -> size; i++)
		{
			for(size_t j = 0; j < non -> size; j++)
			{
					wprintw(win, "%c",non->table[i][j].symbol);
			}
		}
	}
}

void non_delete(non_t *non)
{
	if(non == NULL) return;
	if(non -> table == NULL)
	{
		free(non);
		return;
	}
	for(size_t i = 0; i < non -> size; i++)
	{
		if(non -> table[i] != NULL) free(non -> table[i]);
	}
	for(size_t i = 0; i < non -> size; i++)
	{
		clue_delete(non->colClues[i]);
		clue_delete(non->rowClues[i]);
	}
	free(non->colClues);
	free(non->rowClues);
	free(non -> table);
	free(non);
}
