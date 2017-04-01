#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED
//#include "Arena.h"
class Arena;
class Game
{
public:
	// Constructor/destructor
	Game(int rows, int cols, int nRats);
	~Game();

	// Mutators
	void play();

private:
	Arena* m_arena;

	// Helper functions
	string takePlayerTurn();
};
#endif

