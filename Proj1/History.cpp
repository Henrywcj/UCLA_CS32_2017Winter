#include "History.h"
#include <iostream>
using namespace std;

History::History(int nRows, int nCols)
{
    m_row = nRows;
    m_col = nCols;
    for (int i = 0; i < nRows; i++){
        for (int j = 0; j < nCols; j++){
			m_grid[i][j] = '.';
        }
    }
}

bool History::record(int r, int c)
{
	if (r > m_row || c > m_col)
		return false;
    else
    {
        if (m_grid[r - 1][c - 1] != '.' && m_grid[r - 1][c - 1] != 'Z')
            m_grid[r - 1][c - 1] = m_grid[r - 1][c - 1] + 1;
        else if (m_grid[r - 1][c - 1] == '.') m_grid[r - 1][c - 1] = 'A';
        return true;
    }
}

void History::display() const
{
    clearScreen();
    for (int i = 0; i < m_row; i++)
    {
        for (int j = 0; j < m_col; j++)
            cout << m_grid[i][j];
        cout << endl;
    }
    cout << endl;
}
