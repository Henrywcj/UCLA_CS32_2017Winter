#ifndef HISTORY_H_INCLUDED
#define HISTORY_H_INCLUDED
#include "globals.h"

class History
{
public:
	History(int nRows, int nCols);
	bool record(int r, int c);
	void display() const;
private:
	int m_row;
	int m_col;
	char m_grid[MAXROWS][MAXCOLS];
};

#endif