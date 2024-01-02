#include "non.h"
#include <string.h>

non_t* non_initialize(size_t size)
{
	non_t *non = (non_t *) malloc(sizeof(non_t));
	non -> size = size;
	non -> total = 100; //Default values to not have auto win 
	non -> filled = 0;
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
	int total = 0;
	for(size_t i = 0; i < non -> size; i++)
	{
		for(size_t j = 0; j < non -> size; j++)
		{
			non -> table[i][j].status = rand() % 2;
			if (non -> table[i][j].status)
			{
				total++;
			}
		}
	}
	non -> total = total;
	non -> title = "RANDOM\n";
}

void build_from_file(non_t *non, char *file)
{
	char filePath[100];
	filePath[0] = '\0';
	char dir[] = "puzzles/";
	strcat(filePath, dir);
	strcat(filePath, file);
	//fprintf(stderr,"File Path: %s\n", filePath);
	FILE *p_file = fopen(filePath, "r");
	if(p_file == NULL)
	{
		fprintf(stderr,"File not found: %s\n", filePath);
		exit(1);
	}
	else
	{
		//For counting filled spots
		int total = 0;
		char buffer[100];
		char * size = fgets(buffer, sizeof(buffer), p_file);
		//fprintf(stderr, "%s", string);
		//TODO: ERROR HANDLE
		size_t puzzle_size = (size_t) atoi(size);
		//fprintf(stderr, "%ld\n", puzzle_size);
		char * name = fgets(buffer, sizeof(buffer), p_file);
		//fprintf(stderr, "%s", name);

		//Initialize member variables
		non -> title = name;
		non -> size = puzzle_size;

		/*
		   Iterate through the puzzle, change cell status', and calculate
		   total filled spots, just like randomize();
		 */
		char *puzzle_line = NULL;
		for(size_t i = 0; i < non -> size; i++)
		{
			//Pull a string
			puzzle_line = fgets(buffer, sizeof(buffer), p_file);

			for(size_t j = 0; j < non -> size; j++)
			{
				//Iterate through characters
				if(puzzle_line[j] == '_')
				{
					non -> table[i][j].status = 0;
				}
				else
				{
					non -> table[i][j].status = 1;
					total++;
				}

			}
		}
		non -> total = total;
		//fprintf(stderr, "Total = %d\n", total);
	}
}
void non_solve(non_t *non)
{
	if(non == NULL) return;
	//Current value to get pushed into the queue
	int count = 0;
	for(size_t i = 0; i < non -> size; i++)
	{
		for(size_t j = 0; j < non -> size; j++)
		{
			//Index through rows, and calculate
			//If there is a filled spot
			if(non -> table[i][j].status)
			{
				count++;
			}
			//We encounter an empty
			else
			{
				//If we have a streak going
				if(count)
				{
					clue_insert(non -> rowClues[i], count);
					//Reset Streak
					count = 0;
				}
			}
		}
		//If we have a streak going
		if(count)
		{
			clue_insert(non -> rowClues[i], count);
			//Reset Streak
			count = 0;
		}
	}
	//TODO: DO COLS THIS COULD BE MERGED EVENUTALLY
	count = 0;

	for(size_t i = 0; i < non -> size; i++)
	{
		for(size_t j = 0; j < non -> size; j++)
		{
			//Index through cols, and calculate
			//If there is a filled spot
			if(non -> table[j][i].status)
			{
				count++;
			}
			//We encounter an empty
			else
			{
				//If we have a streak going
				if(count)
				{
					clue_insert(non -> colClues[i], count);
					//Reset Streak
					count = 0;
				}
			}
		}
		//If we have a streak going
		if(count)
		{
			clue_insert(non -> colClues[i], count);
			//Reset Streak
			count = 0;
		}
	}
}
//TODO: PRINT CLUES
void non_clue_print_debug(WINDOW *win, non_t *non)
{
	if(non == NULL) return;
	clue_t * currClue;
	if(win == NULL)
	{
		//Print rows
		for(size_t i = 0; i < non -> size; i++)
		{
			printw("Row %ld:\t", i);
			for (currClue = non -> rowClues[i] -> head; currClue != NULL; currClue = currClue -> next)
			{
				printw("%d ", currClue -> value);
			}
			printw("\n");
		}
		//Print cols
		for(size_t i = 0; i < non -> size; i++)
		{
			printw("Col %ld:\t", i);
			for (currClue = non -> colClues[i] -> head; currClue != NULL; currClue = currClue -> next)
			{
				printw("%d ", currClue -> value);
			}
			printw("\n");
		}
	}
}
//Prints in the row clue window
void non_clue_print_x(WINDOW *win, non_t *non)
{
	init_pair(4, COLOR_BLACK, COLOR_YELLOW);
	//starts at 0, puzzlesize -1 0,4
	//0,puzzlesize -2 0,3
	//0,... until puzzlesize = 0
	//1,puzzlSize -1 1,4
	clue_t * currClue;
	for(size_t i = 0; i < non -> size; i++)
	{
		//Start at the tail of the list and print backward
		currClue = non -> rowClues[i] -> tail;
		for(int j = (int) non -> size - 1; j >= 0 && currClue; j--)
		{
				if(currClue -> value >= 10 && currClue -> value <= 20)
				{
					wattron(win, COLOR_PAIR(4));
				}
				mvwprintw(win,i,j,"%d",currClue -> value);
				if(currClue -> value >= 10 && currClue -> value <= 20)
				{	
					wattroff(win, COLOR_PAIR(4));
				}
				currClue = currClue -> prev;
				//Print a space to the left, (j-1)
				mvwprintw(win,i,j-1," ");
				//Decrement j
				j--;
		}
/* TODO: WHy isnt this equivalent
		int j = (int) non -> size;
		while (currClue && j >= 0)
		{
				mvwprintw(win,i,j,"%d",currClue -> value);
				currClue = currClue -> prev;
				j--;
				//Print a space to the left, (j-1)
				mvwprintw(win,i,j-1," ");
				//Decrement j
				j--;

		}*/

	}
}
//Prints in the column clue window
void non_clue_print_y(WINDOW *win, non_t *non)
{
	init_pair(4, COLOR_BLACK, COLOR_YELLOW);
	//starts at  puzzlesize -1, 0 (4,0)
	//puzzlesize -2, 0 (3,0)
	//0,... until puzzlesize = 0
	//puzzlesize -1, 1
	clue_t * currClue;
	for(size_t i = 0; i < non -> size; i++)
	{
		//Start at the tail of the list and print backward
		currClue = non -> colClues[i] -> tail;
		for(int j = (int) non -> size - 1; j >= 0 && currClue; j--)
		{
				if(currClue -> value >= 10 && currClue -> value <= 20)
				{
					wattron(win, COLOR_PAIR(4));
				}
				mvwprintw(win,j,i,"%d",currClue -> value);
				if(currClue -> value >= 10 && currClue -> value <= 20)
				{	
					wattroff(win, COLOR_PAIR(4));
				}
				currClue = currClue -> prev;
				//Print a space to the left, (j-1)
				mvwprintw(win,j-1,i," ");
				//Decrement j
				j--;
		}
	}
}

void clue_highlight_x(WINDOW *win, non_t *non, int ypos, bool hl)
{
	clue_t *currClue;
	
	init_pair(4, COLOR_BLACK, COLOR_YELLOW);

	//Iterate through the row and highlight just that row.
	//Similar to previous loop, but we break when currClue is null;
	//To either highlight or unhighlight rows
	//If ypos = 0, then we iterate through colClues[0] from the
	//tail until colClues is null, printing spaces.
	/*0 0, 1,0 ...non -> size - 1*/
	if(hl)
	{

		wattron(win, A_REVERSE);
		currClue = non -> rowClues[ypos] -> tail;
		for(int j = (int) non -> size - 1; j >= 0 && currClue; j--)
		{
				if(currClue -> value >= 10 && currClue -> value <= 19)
				{
					wattron(win, COLOR_PAIR(4));
					wattroff(win, A_REVERSE);
				}
				mvwprintw(win,ypos,j,"%d",currClue -> value);
				if(currClue -> value >= 10 && currClue -> value <= 19)
				{
					wattroff(win, COLOR_PAIR(4));
					wattron(win, A_REVERSE);
				}
				currClue = currClue -> prev;
				if(currClue)
				{
					//Print a space to the left, (j-1)
					mvwprintw(win,ypos,j-1," ");
					//Decrement j
					j--;
				}
		}
		wattroff(win, A_REVERSE);
	}
	else
	{
		currClue = non -> rowClues[ypos] -> tail;
		for(int j = (int) non -> size - 1; j >= 0 && currClue; j--)
		{
				
				if(currClue -> value >= 10 && currClue -> value <= 19)
				{
					wattron(win, COLOR_PAIR(4));
				}
				mvwprintw(win,ypos,j,"%d",currClue -> value);
				if(currClue -> value >= 10 && currClue -> value <= 19)
				{
					wattroff(win, COLOR_PAIR(4));
				}
				currClue = currClue -> prev;
				//Print a space to the left, (j-1)
				if(currClue)
				{
					mvwprintw(win,ypos,j-1," ");
					//Decrement j
					j--;
				}
		}
	}
}
void clue_highlight_y(WINDOW *win, non_t *non, int xpos, bool hl)
{
	clue_t *currClue;

	init_pair(4, COLOR_BLACK, COLOR_YELLOW);
	//Iterate through the row and highlight just that row.
	//Similar to previous loop, but we break when currClue is null;
	//To either highlight or unhighlight rows
	//If ypos = 0, then we iterate through colClues[0] from the
	//tail until colClues is null, printing spaces.
	/*0 0, 1,0 ...non -> size - 1*/
	if(hl)
	{

		wattron(win, A_REVERSE);
		currClue = non -> colClues[xpos] -> tail;
		for(int j = (int) non -> size - 1; j >= 0 && currClue; j--)
		{

			if(currClue -> value >= 10 && currClue -> value <= 19)
			{
				wattron(win, COLOR_PAIR(4));
				wattroff(win, A_REVERSE);
			}
			mvwprintw(win,j,xpos,"%d",currClue -> value);
			if(currClue -> value >= 10 && currClue -> value <= 19)
			{
				wattroff(win, COLOR_PAIR(4));
				wattron(win, A_REVERSE);
			}
			currClue = currClue -> prev;
			if(currClue)
			{
				//Print a space to the left, (j-1)
				mvwprintw(win,j-1,xpos," ");
				//Decrement j
				j--;
			}
		}
		wattroff(win, A_REVERSE);
	}
	else
	{
		currClue = non -> colClues[xpos] -> tail;
		for(int j = (int) non -> size - 1; j >= 0 && currClue; j--)
		{

			if(currClue -> value >= 10 && currClue -> value <= 19)
			{
				wattron(win, COLOR_PAIR(4));
			}
			mvwprintw(win,j,xpos,"%d",currClue -> value);
			if(currClue -> value >= 10 && currClue -> value <= 19)
			{
				wattroff(win, COLOR_PAIR(4));
			}
			currClue = currClue -> prev;
			//Print a space to the left, (j-1)
			if(currClue)
			{
				mvwprintw(win,j-1,xpos," ");
				//Decrement j
				j--;
			}
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
