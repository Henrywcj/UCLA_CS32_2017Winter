#include <string>
#include <stack>
#include <iostream>
using namespace std;

class Coord
{
public:
	Coord(int rr, int cc) : m_r(rr), m_c(cc) {}
	int r() const { return m_r; }
	int c() const { return m_c; }
private:
	int m_r;
	int m_c;
};

bool pathExists(string maze[], int nRows, int nCols, int sr, int sc, int er, int ec)
{
	stack<Coord> coordStack;
	coordStack.push(Coord(sr, sc));
	maze[sr][sc] = 'X';
	while (!coordStack.empty()) {
		Coord current = coordStack.top();
		coordStack.pop();
		Coord destination(er, ec);
		if (current.r() == destination.r() && current.c() == destination.c()) return true;
		if (maze[current.r() - 1][current.c()] == '.')
		{
			coordStack.push(Coord(current.r() - 1, current.c()));
			maze[current.r() - 1][current.c()] = 'X';
		}
		if (maze[current.r()][current.c() + 1] == '.')
		{
			coordStack.push(Coord(current.r(), current.c() + 1));
			maze[current.r()][current.c() + 1] = 'X';
		}

		if (maze[current.r() + 1][current.c()] == '.')
		{
			coordStack.push(Coord(current.r() + 1, current.c()));
			maze[current.r() + 1][current.c()] = 'X';
		}
		if (maze[current.r()][current.c() - 1] == '.')
		{
			coordStack.push(Coord(current.r(), current.c() - 1));
			maze[current.r()][current.c() - 1] = 'X';
		}
	}
	return false; 
}

int main()
{
	string maze[10] = {
		"XXXXXXXXXX",
		"X........X",
		"XX.X.XXXXX",
		"X..X.X...X",
		"X..X...X.X",
		"XXXX.XXX.X",
		"X.X....XXX",
		"X..XX.XX.X",
		"X...X....X",
		"XXXXXXXXXX"
	};

	if (pathExists(maze, 10, 10, 6, 4, 1, 1))
		cout << "Solvable!" << endl;
	else
		cout << "Out of luck!" << endl;
}