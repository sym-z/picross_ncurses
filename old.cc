#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <ncurses.h>

using namespace std;

enum CellStatus {EMPTY = 0, FILLED = 1, X = 3};

typedef struct Cell
{
	CellStatus status;
	unsigned int xloc;
	unsigned int yloc;
}cell_t;

typedef struct Nonogram
{
	vector<vector<cell_t>> table;
	size_t size;

	void initialize(size_t size)
	{
		this -> size = size;
		table.resize(size);	
		
		for (int i = 0; i < size; i++)
		{
			table[i].resize(size);
		}
	}
}non_t;

void randomize(non_t &non)
{
	for(vector<cell_t> &vec : non.table)
	{
		for(cell_t &cell : vec)
		{
			const int status = rand() % 2;
			cell.status = static_cast<CellStatus>(status); 
		}
	}

}

void print(non_t &non)
{
	//Print column headers
	vector<int> columns(5);
	for(int i = 0; i < non.size; i++)
	{
		for(int j = 0; j < non.size; j++)
		{
			if(non.table[j][i].status == 1)
			{
				//columns.at
			}
		}
	}
	for(int i = 0; i < non.size; i++)
	{
		for(int j = 0; j < non.size; j++)
		{
			cout << non.table[i][j].status << " ";
		}
		cout << endl;
	}
}
int main()
{
	srand(time(0));
	non_t answer, table;
		
	answer.initialize(5);	
	table.initialize(5);	
	randomize(answer);
	print(answer);

	return 0;
}
