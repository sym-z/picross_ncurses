#include "non.h"


int main(int argc, char *argv[])
{
	//TODO: Use command line args to build the puzzle.
	int puzzleSize = 5;
	if(argc > 1)
	{
		puzzleSize = atoi(argv[1]);
	}

	initscr(); //Start NCurses
	raw(); //Line Buffering
	keypad(stdscr, TRUE); //Get our keyboard
	noecho();	//Don't echo to the screen
	curs_set(0);

	int scrnRow, scrnCol;

	getmaxyx(stdscr,scrnRow, scrnCol);

	printw("Sym-Z Picross World!\nRows:%d\tCols:%d\t\n", scrnRow,scrnCol);
	refresh();

	non_t *puzzle = non_initialize(puzzleSize);
	
	int startx, starty;
	starty = (scrnRow - puzzle -> size)/2;
	startx = (scrnCol - puzzle -> size)/2;


	WINDOW *puzzle_border = newwin(puzzle -> size + 2, puzzle -> size + 4, starty - 1, startx - 2);
	box(puzzle_border, 0,0);
	wrefresh(puzzle_border);
	
	size_t posx = 0, posy = 0;

	WINDOW *puzzle_win= newwin(puzzle -> size, puzzle -> size, starty, startx);
	keypad(puzzle_win, TRUE); //Get our keyboard
	non_randomize(puzzle);
	non_print(puzzle_win, puzzle);
	wattron(puzzle_win, A_REVERSE);
	mvwprintw(puzzle_win,posy,posx,"%d",puzzle->table[posy][posx].status);
	wattroff(puzzle_win, A_REVERSE);
	wrefresh(puzzle_win);
	
	while(1)
	{
		int c = wgetch(puzzle_win);
		switch(c)
		{
			case KEY_UP:
				if(posy != 0)
				{
					mvwprintw(puzzle_win,posy,posx,"%d",puzzle->table[posy][posx].status);
					wattron(puzzle_win, A_REVERSE);
					mvwprintw(puzzle_win,posy-1,posx,"%d",puzzle->table[posy-1][posx].status);
					wattroff(puzzle_win, A_REVERSE);
					wrefresh(puzzle_win);
					posy -= 1;
				}
				break;
			case KEY_DOWN:
				if(posy != puzzle -> size - 1)
				{
					mvwprintw(puzzle_win,posy,posx,"%d",puzzle->table[posy][posx].status);
					wattron(puzzle_win, A_REVERSE);
					mvwprintw(puzzle_win,posy+1,posx,"%d",puzzle->table[posy+1][posx].status);
					wattroff(puzzle_win, A_REVERSE);
					wrefresh(puzzle_win);
					posy += 1;
				}
				break;
			case KEY_LEFT:
				if(posx != 0)
				{
					mvwprintw(puzzle_win,posy,posx,"%d",puzzle->table[posy][posx].status);
					wattron(puzzle_win, A_REVERSE);
					mvwprintw(puzzle_win,posy,posx-1,"%d",puzzle->table[posy][posx-1].status);
					wattroff(puzzle_win, A_REVERSE);
					wrefresh(puzzle_win);
					posx -= 1;
				}
				break;
			case KEY_RIGHT:
				if(posx != puzzle -> size - 1)
				{
					mvwprintw(puzzle_win,posy,posx,"%d",puzzle->table[posy][posx].status);
					wattron(puzzle_win, A_REVERSE);
					mvwprintw(puzzle_win,posy,posx+1,"%d",puzzle->table[posy][posx+1].status);
					wattroff(puzzle_win, A_REVERSE);
					wrefresh(puzzle_win);
					posx += 1;
				}
				break;
			default:
				goto end;
		}
	}


end:
	non_delete(puzzle);
	endwin();
	return 0;
}
