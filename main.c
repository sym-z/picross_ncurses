#include "non.h"
#include <ncurses.h>
//TODO: Print controls
//TODO: Keep track of filled spots and countdown, so you know when the game is over
//TODO: Deduct a life on a missed input
//TODO: Edit spacing
//TODO: Marking a spot that is filled deducts a point 
//TODO: Window for points that gets edited
//TODO: Print number in red to indicate +10
//TODO: Paint the hashtags, question marks and the X's so they are different colors
/*Done: 

//Highlight Row and Column that you are in by calling a function in the switch statement.
*/
void start_ncurses();
void move_pos(WINDOW *win, cell_t *curr, cell_t *next);
void uncover(WINDOW *win, cell_t *curr);
void mark(WINDOW *win, cell_t *curr);

int main(int argc, char *argv[])
{
	//Optionally uses command line args to build the puzzle.
	size_t puzzleSize = 5;
	if(argc > 1)
	{
		if(atoi(argv[1]) >= 5)
		{
			puzzleSize = atoi(argv[1]);
		}
	}

	start_ncurses();
	int scrnRow, scrnCol;

	getmaxyx(stdscr,scrnRow, scrnCol);


	printw("Sym-Z Picross World!\nRows:%d\tCols:%d\t\n\n", scrnRow,scrnCol);
	printw("Controls:\nMove: Arrow Keys\nUncover: Z\nMark as empty: X\n");
	refresh();

	//Make the puzzle, randomize it, and fill clue queues
	non_t *puzzle = non_initialize(puzzleSize);
	non_randomize(puzzle);
	non_solve(puzzle);

	//Start the puzzle in the center of the screen
	int startx, starty;
	starty = (scrnRow - puzzle -> size)/2 + 8;
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


	keypad(puzzle_win, TRUE); //Get our keyboard
	while(1)
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
				break;
			case 'x':
				mark(puzzle_win, currSpot);
				break;
			default:
				goto end;
		}
	}
end:
	non_delete(puzzle);
	delwin(puzzle_win);
	delwin(puzzle_border);
	delwin(clue_win_x);
	endwin();
	return 0;
}


void start_ncurses()
{
	initscr(); //Start NCurses
	raw(); //Line Buffering
	keypad(stdscr, TRUE); //Get our keyboard
	start_color();
	noecho();	//Don't echo to the screen
	curs_set(0);

}
void move_pos(WINDOW *win, cell_t *curr, cell_t *next)
{
	mvwprintw(win, curr->y, curr->x, "%c",curr->symbol);

	wattron(win, A_REVERSE);
	mvwprintw(win,next->y,next->x,"%c",next->symbol);
	wattroff(win, A_REVERSE);
	wrefresh(win);
}
void uncover(WINDOW *win, cell_t *curr)
{
	if(curr -> status == 1) curr -> symbol = '#'; 
	else curr -> symbol = '_';
	wattron(win, A_REVERSE);
	mvwprintw(win,curr->y,curr->x,"%c",curr->symbol);
	wattroff(win, A_REVERSE);
	wrefresh(win);
}
void mark(WINDOW *win, cell_t *curr)
{
	if(curr -> symbol == '?')
	{
		curr -> symbol = 'X';
		//wattron(win, A_REVERSE);
		init_pair(1,COLOR_WHITE,COLOR_RED);
		wattron(win,COLOR_PAIR(1));
		mvwprintw(win,curr->y,curr->x,"%c",curr->symbol);
		wattroff(win,COLOR_PAIR(1));
		//wattroff(win, A_REVERSE);
		wrefresh(win);
	}
}
