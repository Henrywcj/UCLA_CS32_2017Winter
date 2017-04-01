#include <iostream>
#include <string>
#include <queue>
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
	queue<Coord> coordQueue;
	coordQueue.push(Coord(sr, sc));
	maze[sr][sc] = 'X';
	while (!coordQueue.empty()) {
		Coord current = coordQueue.front();
		coordQueue.pop();
		Coord destination(er, ec);
		if (current.r() == destination.r() && current.c() == destination.c()) return true;
		if (maze[current.r() - 1][current.c()] == '.')
		{
			coordQueue.push(Coord(current.r() - 1, current.c()));
			maze[current.r() - 1][current.c()] = 'X';
		}
		if (maze[current.r()][current.c() + 1] == '.')
		{
			coordQueue.push(Coord(current.r(), current.c() + 1));
			maze[current.r()][current.c() + 1] = 'X';
		}

		if (maze[current.r() + 1][current.c()] == '.')
		{
			coordQueue.push(Coord(current.r() + 1, current.c()));
			maze[current.r() + 1][current.c()] = 'X';
		}
		if (maze[current.r()][current.c() - 1] == '.')
		{
			coordQueue.push(Coord(current.r(), current.c() - 1));
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

	if (pathExists(maze, 10, 10, 5, 4, 1, 1))
		cout << "Solvable!" << endl;
	else
		cout << "Out of luck!" << endl;
}