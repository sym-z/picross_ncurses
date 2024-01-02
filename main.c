#include "non.h"
#include <unistd.h>
#include <ncurses.h>

/*---------------------------TODO LIST--------------------------*/

//TODO: Print controls
//TODO:	Build from file function in non.h/c 
//TODO: Keep track of filled spots and countdown, so you know when the game is over
//TODO: Game Win Screen
//TODO: Game Over Screen
//TODO: Reveal and color the whole map using a function in non.c/h
//TODO: Main Menu
//TODO: Converting text files to puzzle
//TODO: Level Select vs. Random
//TODO: Edit spacing (I dont think I am going to do this, going to use BK's +10 method.
//TODO: Marking a spot that is filled deducts a point 
//TODO: Window for strikes that has GAME WIN/LOSS 
//TODO: Print number in red to indicate +10 Needs to be red even when highlighted, so it needs to be handled in the highlight function and the clue print function.

/*--------------------------DONE LIST---------------------------*/ 

//Highlight Row and Column that you are in by calling a function in the switch statement.
//Paint the hashtags, question marks and the X's so they are different colors
//Window for strikes that gets edited
//Deduct a life on a missed input



//Defines Color Pairs
#define MARK 1
#define FILLED 2
#define EMPTY 3

void start_ncurses();
void move_pos(WINDOW *win, cell_t *curr, cell_t *next);
void uncover(WINDOW *win, cell_t *curr);
void mark(WINDOW *win, cell_t *curr);
void strike();

bool gameWon = 0, gameLoss = 0;

//To allow strike function to edit window.
WINDOW * strike_win = NULL;

int main(int argc, char *argv[])
{
	//Optionally uses command line args to build the puzzle.
	size_t puzzleSize = 5;
	if(argc > 1)
	{
		if(atoi(argv[1]) >= 5)
		{
			//TODO: ERROR CHECK HERE
			puzzleSize = atoi(argv[1]);
		}
	}

	start_ncurses();
	int scrnRow, scrnCol;

	getmaxyx(stdscr,scrnRow, scrnCol);

	//Default Title and Controls
	printw("Sym-Z Picross World!\nRows:%d\tCols:%d\t\n\n", scrnRow,scrnCol);
	printw("Controls:\nMove: Arrow Keys\nUncover: Z\nMark as empty: X\nQ: Quit\n");
	refresh();

	//Make the puzzle, randomize it, and fill clue queues
	non_t *puzzle = non_initialize(puzzleSize);
	char file[] = "1";
	build_from_file(puzzle,file);
	//non_randomize(puzzle);
	non_solve(puzzle);

	//Start the puzzle in the center of the screen
	int startx, starty;
	starty = (scrnRow - puzzle -> size)/2 + 7;
	startx = (scrnCol - puzzle -> size)/2 + 5;


	//Cursor position in the puzzle
	size_t posx = 0, posy = 0;

	//Prints test clues
	//non_clue_print_debug(NULL, puzzle);
	//refresh();

	//Make puzzle frame
	int borderHeight = puzzleSize + 2;
	int borderWidth = puzzleSize + 4;
	int borderStartY = starty - 1;
	int borderStartX = startx - 2;
	WINDOW *puzzle_border = newwin(borderHeight,borderWidth, borderStartY, borderStartX);
	box(puzzle_border, 0,0);
	wrefresh(puzzle_border);

	//Print Puzzle
	WINDOW *puzzle_win= newwin(puzzleSize, puzzleSize, starty, startx);
	non_print(puzzle_win, puzzle);

	//Start with cursor at 0,0 on the window.
	wattron(puzzle_win, A_REVERSE);
	mvwprintw(puzzle_win,posy,posx,"%c", puzzle->table[posy][posx].symbol);
	wattroff(puzzle_win, A_REVERSE);
	wrefresh(puzzle_win);

	//Print clue window X and Y
	WINDOW *clue_win_x = newwin(puzzleSize,puzzleSize,starty,borderStartX - puzzleSize);
	non_clue_print_x(clue_win_x,puzzle);
	wrefresh(clue_win_x);
	clue_highlight_x(clue_win_x, puzzle,posy,1);
	wrefresh(clue_win_x);

	WINDOW *clue_win_y = newwin(puzzleSize,puzzleSize,borderStartY - puzzleSize,startx);
	non_clue_print_y(clue_win_y,puzzle);
	wrefresh(clue_win_y);
	clue_highlight_y(clue_win_y, puzzle,posx,1);
	wrefresh(clue_win_y);

	//Starts boxy, box_x + boxwidth + 2, width: 10, height 4.
	//To change the number of strikes, print at 1,10
	int strikeWidth = 12, strikeHeight = 4;
	//We don't need to initialize this because it is a global
	strike_win = newwin(strikeHeight,strikeWidth,borderStartY, borderStartX + borderWidth);
	box(strike_win, 0,0);
	mvwprintw(strike_win, 1,1,"Strikes: 0");
	//mvwprintw(strike_win, 1,10,"X");
	wrefresh(strike_win);


	keypad(puzzle_win, TRUE); //Get our keyboard
	//Once we win or lose, we cannot play the puzzle any longer
	while(!gameWon && !gameLoss)
	{
		//All uncovering does is change the symbol to be the status
		int c = wgetch(puzzle_win);
		cell_t *currSpot = &(puzzle -> table[posy][posx]);
		cell_t *nextSpot;
		switch(c)
		{
			case KEY_UP:
				if(posy != 0)
				{

					nextSpot = &(puzzle -> table[posy-1][posx]);
					clue_highlight_x(clue_win_x, puzzle,nextSpot -> y,1);

					move_pos(puzzle_win,currSpot, nextSpot);
					clue_highlight_x(clue_win_x,puzzle,currSpot -> y,0);
					wrefresh(clue_win_x);
					posy -= 1;
				}
				break;
			case KEY_DOWN:
				if(posy != puzzle -> size - 1)
				{
					nextSpot = &(puzzle -> table[posy+1][posx]);
					clue_highlight_x(clue_win_x, puzzle,nextSpot -> y,1);

					move_pos(puzzle_win,currSpot, nextSpot);
					clue_highlight_x(clue_win_x,puzzle,currSpot -> y,0);
					wrefresh(clue_win_x);
					posy += 1;
				}
				break;
			case KEY_LEFT:
				if(posx != 0)
				{	
					nextSpot = &(puzzle -> table[posy][posx-1]);
					clue_highlight_y(clue_win_y, puzzle,nextSpot -> x,1);

					move_pos(puzzle_win,currSpot, nextSpot);
					clue_highlight_y(clue_win_y,puzzle,currSpot -> x,0);
					wrefresh(clue_win_y);
					posx -= 1;
				}
				break;
			case KEY_RIGHT:
				if(posx != puzzle -> size - 1)
				{	
					nextSpot = &(puzzle -> table[posy][posx+1]);
					clue_highlight_y(clue_win_y, puzzle,nextSpot -> x,1);

					move_pos(puzzle_win,currSpot, nextSpot);
					clue_highlight_y(clue_win_y,puzzle,currSpot -> x,0);
					wrefresh(clue_win_y);
					posx += 1;
				}
				break;
			case 'z':
				uncover(puzzle_win, currSpot);
				//Check to see if the spot was filled or not, you can deduct a point here.
				if(puzzle -> table[posy][posx].symbol == '#')
				{
					puzzle -> filled += 1;
					if(puzzle -> total == puzzle -> filled)
					{
						//GAME WIN
						//printw("GAME WON\n");
						mvwprintw(strike_win, 2,1,"GAME WON");
						wrefresh(strike_win);
						gameWon = true;
						//Remove highlighting on win.
						wattron(puzzle_win,COLOR_PAIR(FILLED));
						mvwprintw(puzzle_win, posy, posx, "%c",currSpot->symbol);
						wattroff(puzzle_win,COLOR_PAIR(FILLED));
						wrefresh(puzzle_win);
						sleep(3);
					}
				}
				if(puzzle -> table[posy][posx].symbol == '_')
				{
					//STRIKE WHEN YOU UNCOVER AN EMPTY SPOT
					strike();
				}
				break;
			case 'x':
				mark(puzzle_win, currSpot);
				//If someone marks incorrectly, they could still win the game if it isn't their final strike
				if(puzzle -> table[posy][posx].symbol == '#')
				{
					puzzle -> filled += 1;
					if(puzzle -> total == puzzle -> filled)
					{
						//GAME WIN
						//printw("GAME WON\n");
						mvwprintw(strike_win, 2,1,"GAME WON");
						wrefresh(strike_win);
						gameWon = true;
						//Remove highlighting on win.
						wattron(puzzle_win,COLOR_PAIR(FILLED));
						mvwprintw(puzzle_win, posy, posx, "%c",currSpot->symbol);
						wattroff(puzzle_win,COLOR_PAIR(FILLED));
						wrefresh(puzzle_win);
						sleep(3);
					}
				}
				break;
			case 'q':
				goto end;
			default:
				break;
		}
	}
end:
	//Clean Up 
	non_delete(puzzle);
	delwin(puzzle_win);
	delwin(puzzle_border);
	delwin(clue_win_x);
	delwin(clue_win_y);
	delwin(strike_win);
	endwin();
	return 0;
}


void start_ncurses()
{
	initscr(); //Start NCurses
	raw(); //Line Buffering
	keypad(stdscr, TRUE); //Get our keyboard
	start_color();
	init_pair(MARK,COLOR_WHITE,COLOR_BLUE);
	init_pair(FILLED,COLOR_WHITE,COLOR_GREEN);
	init_pair(EMPTY,COLOR_WHITE,COLOR_RED);
	noecho();	//Don't echo to the screen
	curs_set(0);

}
void move_pos(WINDOW *win, cell_t *curr, cell_t *next)
{
	int currSymbol = curr -> symbol, nextSymbol = next -> symbol;
	
	if(currSymbol == '?')
	{
		//Remove highlighting for current space
		mvwprintw(win, curr->y, curr->x, "%c",curr->symbol);
	}
	if(currSymbol == 'X')
	{
		wattron(win,COLOR_PAIR(MARK));
		mvwprintw(win,curr->y,curr->x,"%c",curr->symbol);
		wattroff(win,COLOR_PAIR(MARK));
		wrefresh(win);
	}
	if(currSymbol == '#')
	{
		wattron(win,COLOR_PAIR(FILLED));
		mvwprintw(win,curr->y,curr->x,"%c",curr->symbol);
		wattroff(win,COLOR_PAIR(FILLED));
		wrefresh(win);
	}
	if(currSymbol == '_')
	{
		wattron(win,COLOR_PAIR(EMPTY));
		mvwprintw(win,curr->y,curr->x,"%c",curr->symbol);
		wattroff(win,COLOR_PAIR(EMPTY));
		wrefresh(win);
	}
	
	//Highlight next space
	wattron(win, A_REVERSE);
	mvwprintw(win,next->y,next->x,"%c",nextSymbol);
	wattroff(win, A_REVERSE);
	wrefresh(win);
}
void uncover(WINDOW *win, cell_t *curr)
{
	//Change symbol to match status, and highlight it
	if(curr -> status == 1)
	{
		curr -> symbol = '#'; 
	}
	else
	{
		curr -> symbol = '_';
	}
	wattron(win, A_REVERSE);
	mvwprintw(win,curr->y,curr->x,"%c",curr->symbol);
	wattroff(win, A_REVERSE);
	wrefresh(win);
}
void mark(WINDOW *win, cell_t *curr)
{
	if(curr -> symbol == '?')
	{
		if(curr -> status == 0)
		{
			//The red for the mark doesn't show until you move
			//your cursor.
			curr -> symbol = 'X';
			wattron(win, A_REVERSE);
			mvwprintw(win,curr->y,curr->x,"%c",curr->symbol);
			wattroff(win, A_REVERSE);
			wrefresh(win);
		}
		else
		{
			//STRIKE FOR MARKING A FILLED SPACE
			uncover(win, curr);
			strike();
		}
	}
	else
	{
		//You cannot mark spaces that have been revealed already
		return;
	}
}
void strike()
{
	static int strikes = 0;
	int loss = 3;
	strikes++;
	//printw("Strikes: %d\n", strikes);
	mvwprintw(strike_win, 1, 10, "%d", strikes);
	wrefresh(strike_win);
	refresh();
	if(strikes == loss)
	{
		//TODO: GAME LOSS
		//printw("GAME LOSS\n");
		mvwprintw(strike_win, 2,1,"GAME LOSS");
		wrefresh(strike_win);
		//TODO: END SCREEN
		gameLoss = true;
		sleep(3);
	}
}
