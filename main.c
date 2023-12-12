#include "non.h"

void start_ncurses();
void end_ncurses();

int main(int argc, char *argv[])
{
	//TODO: Use command line args to build the puzzle.
	int puzzleSize = 5;
	if(argc > 1)
	{
		puzzleSize = atoi(argv[1]);
	}

	start_ncurses();
	int scrnRow, scrnCol;

	getmaxyx(stdscr,scrnRow, scrnCol);

	printw("Sym-Z Picross World!\nRows:%d\tCols:%d\t\n", scrnRow,scrnCol);
	refresh();

	non_t *puzzle = non_initialize(puzzleSize);
	
	//Start the puzzle in the center of the screen
	int startx, starty;
	starty = (scrnRow - puzzle -> size)/2;
	startx = (scrnCol - puzzle -> size)/2;

	
	WINDOW *puzzle_border = newwin(puzzle -> size + 2, puzzle -> size + 4, starty - 1, startx - 2);
	box(puzzle_border, 0,0);
	wrefresh(puzzle_border);
	
	//Cursor position in the puzzle
	size_t posx = 0, posy = 0;

	//Testing by printing answer sheet
	WINDOW *puzzle_win= newwin(puzzle -> size, puzzle -> size, starty, startx);
	keypad(puzzle_win, TRUE); //Get our keyboard
	non_randomize(puzzle);
	non_print(puzzle_win, puzzle);
	
	//Start with cursor at 0,0 on the window.
	wattron(puzzle_win, A_REVERSE);
	//mvwprintw(puzzle_win,posy,posx,"%d",puzzle->table[posy][posx].status);
	mvwprintw(puzzle_win,posy,posx,"?");
	wattroff(puzzle_win, A_REVERSE);
	wrefresh(puzzle_win);
	
	while(1)
	{
		int c = wgetch(puzzle_win);
		cell_t currSpot = puzzle -> table[posy][posx];
		cell_t nextSpot;
		switch(c)
		{
			case KEY_UP:
				if(posy != 0)
				{
						
					nextSpot = puzzle -> table[posy-1][posx];
					
					if(!currSpot.covered)
					{
						mvwprintw(puzzle_win,posy,posx,"%d",puzzle->table[posy][posx].status);
						if(!nextSpot.covered)
						{
							wattron(puzzle_win, A_REVERSE);
							mvwprintw(puzzle_win,posy-1,posx,"%d",puzzle->table[posy-1][posx].status);
							wattroff(puzzle_win, A_REVERSE);
							wrefresh(puzzle_win);
						}
						else
						{
							wattron(puzzle_win, A_REVERSE);
							mvwprintw(puzzle_win,posy-1,posx,"?");
							wattroff(puzzle_win, A_REVERSE);
							wrefresh(puzzle_win);
							
						}
					}
					else
					{
						mvwprintw(puzzle_win,posy,posx,"?");
						if(!nextSpot.covered)
						{
							wattron(puzzle_win, A_REVERSE);
							mvwprintw(puzzle_win,posy-1,posx,"%d",puzzle->table[posy-1][posx].status);
							wattroff(puzzle_win, A_REVERSE);
							wrefresh(puzzle_win);
						}
						else
						{
							wattron(puzzle_win, A_REVERSE);
							mvwprintw(puzzle_win,posy-1,posx,"?");
							wattroff(puzzle_win, A_REVERSE);
							wrefresh(puzzle_win);
						}	
					}
					posy -= 1;
				}
				break;
			case KEY_DOWN:
				if(posy != puzzle -> size - 1)
				{
					nextSpot = puzzle -> table[posy+1][posx];

					if(!currSpot.covered)
					{
						mvwprintw(puzzle_win,posy,posx,"%d",puzzle->table[posy][posx].status);
						if(!nextSpot.covered)
						{
							wattron(puzzle_win, A_REVERSE);
							mvwprintw(puzzle_win,posy+1,posx,"%d",puzzle->table[posy+1][posx].status);
							wattroff(puzzle_win, A_REVERSE);
							wrefresh(puzzle_win);
						}
						else
						{
							wattron(puzzle_win, A_REVERSE);
							mvwprintw(puzzle_win,posy+1,posx,"?");
							wattroff(puzzle_win, A_REVERSE);
							wrefresh(puzzle_win);

						}
					}
					else
					{
						mvwprintw(puzzle_win,posy,posx,"?");
						if(!nextSpot.covered)
						{
							wattron(puzzle_win, A_REVERSE);
							mvwprintw(puzzle_win,posy+1,posx,"%d",puzzle->table[posy+1][posx].status);
							wattroff(puzzle_win, A_REVERSE);
							wrefresh(puzzle_win);
						}
						else
						{
							wattron(puzzle_win, A_REVERSE);
							mvwprintw(puzzle_win,posy+1,posx,"?");
							wattroff(puzzle_win, A_REVERSE);
							wrefresh(puzzle_win);
						}	
					}
					posy += 1;
				}
				break;
			case KEY_LEFT:
				if(posx != 0)
				{
					nextSpot = puzzle -> table[posy][posx-1];

					if(!currSpot.covered)
					{
						mvwprintw(puzzle_win,posy,posx,"%d",puzzle->table[posy][posx].status);
						if(!nextSpot.covered)
						{
							wattron(puzzle_win, A_REVERSE);
							mvwprintw(puzzle_win,posy,posx-1,"%d",puzzle->table[posy][posx-1].status);
							wattroff(puzzle_win, A_REVERSE);
							wrefresh(puzzle_win);
						}
						else
						{
							wattron(puzzle_win, A_REVERSE);
							mvwprintw(puzzle_win,posy,posx-1,"?");
							wattroff(puzzle_win, A_REVERSE);
							wrefresh(puzzle_win);

						}
					}
					else
					{
						mvwprintw(puzzle_win,posy,posx,"?");
						if(!nextSpot.covered)
						{
							wattron(puzzle_win, A_REVERSE);
							mvwprintw(puzzle_win,posy,posx-1,"%d",puzzle->table[posy][posx-1].status);
							wattroff(puzzle_win, A_REVERSE);
							wrefresh(puzzle_win);
						}
						else
						{
							wattron(puzzle_win, A_REVERSE);
							mvwprintw(puzzle_win,posy,posx-1,"?");
							wattroff(puzzle_win, A_REVERSE);
							wrefresh(puzzle_win);
						}	
					}
					posx -= 1;
				}
				break;
			case KEY_RIGHT:
				if(posx != puzzle -> size - 1)
				{
					nextSpot = puzzle -> table[posy][posx+1];

					if(!currSpot.covered)
					{
						mvwprintw(puzzle_win,posy,posx,"%d",puzzle->table[posy][posx].status);
						if(!nextSpot.covered)
						{
							wattron(puzzle_win, A_REVERSE);
							mvwprintw(puzzle_win,posy,posx+1,"%d",puzzle->table[posy][posx+1].status);
							wattroff(puzzle_win, A_REVERSE);
							wrefresh(puzzle_win);
						}
						else
						{
							wattron(puzzle_win, A_REVERSE);
							mvwprintw(puzzle_win,posy,posx+1,"?");
							wattroff(puzzle_win, A_REVERSE);
							wrefresh(puzzle_win);

						}
					}
					else
					{
						mvwprintw(puzzle_win,posy,posx,"?");
						if(!nextSpot.covered)
						{
							wattron(puzzle_win, A_REVERSE);
							mvwprintw(puzzle_win,posy,posx+1,"%d",puzzle->table[posy][posx+1].status);
							wattroff(puzzle_win, A_REVERSE);
							wrefresh(puzzle_win);
						}
						else
						{
							wattron(puzzle_win, A_REVERSE);
							mvwprintw(puzzle_win,posy,posx+1,"?");
							wattroff(puzzle_win, A_REVERSE);
							wrefresh(puzzle_win);
						}	
					}
					posx += 1;
				}
				break;
			default:
				end_ncurses();
				goto end;
		}
	}
end:
	non_delete(puzzle);
	return 0;
}


void start_ncurses()
{
	initscr(); //Start NCurses
	raw(); //Line Buffering
	keypad(stdscr, TRUE); //Get our keyboard
	noecho();	//Don't echo to the screen
	curs_set(0);
	
}
void end_ncurses()
{
	endwin();
}
