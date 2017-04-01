#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include "Field.h"
#include "Actor.h"
#include <string>
#include <list>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
	StudentWorld(std::string assetDir)
	 : GameWorld(assetDir)
	{
		int m_ticks = 0;
		for (int i = 0; i < 4; i++) numOfAnt[i] = 0;
	}
	~StudentWorld();
	//Implement a constructor for this class that initializes your data members. What is this
	virtual int init();
	virtual int move();
	virtual void cleanUp();
	void addAnt(int posX, int posY, int colonyNum, Compiler* comp);
	bool compileFile(int fileNum, Compiler*& comp);
	int getCurrentTicks();
	void addFoodFromBody(int posX, int posY, int amount);
	void addAdultGrasshopper(int posX, int posY);
	void updateTickCount();
	void removeDead();
	void changeNumOfAnt(int Id, int Num);
	std::vector<insect*> actorOnTheSpot(int posX, int posY);
	template <class Item>
	void checkForItem2(Actor* a, Item*& f)
	{
		f = dynamic_cast<Item*> (a);
	}
	template <class Item>
	void checkForItem(int posX, int posY, Item*& f)
	{
 		std::list<Actor*>::iterator it;
		if (slot[posY][posX].size() == 0) return;
		it = slot[posY][posX].begin();
		while (it != slot[posY][posX].end())
		{
			// *it is a Actor pointer
			Actor* ss = *it;
			f = dynamic_cast<Item*> (ss);
			if (f != nullptr) break;
			it++;
		}
		return;
	}
	void setDisplayText();
	void updateLocation(Actor* a, int oldX, int oldY);
	int getNumberOfAntsForAnt(int i);
	int getWinningAntNumber();
private:
	std::list<Actor*> slot[VIEW_HEIGHT][VIEW_WIDTH]; // row first, column second
	int m_ticks;
	int numOfAnt[4];
	//int numOfPlayers; 
};

#endif // STUDENTWORLD_H_


/* Available gameworld functions:
void setGameStatText(string text);
string assetDirectory() const;
string getFieldFilename() const;
void setWinner(string name);
vector<string> getFilenamesOfAntPrograms() const;*/