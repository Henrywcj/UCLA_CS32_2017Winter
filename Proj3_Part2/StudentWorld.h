#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include "Field.h"
#include "Actor.h"
#include <string>
#include <vector>
#include <list>

class StudentWorld : public GameWorld
{
public:
	StudentWorld(std::string assetDir)
	 : GameWorld(assetDir)
	{
		int m_ticks = 0;
		for (int i = 0; i < 4; i++) numOfAnt[i] = 0;
		numOfPlayers = 0;
	}
	virtual ~StudentWorld();
	virtual int init();
	virtual int move();
	virtual void cleanUp();
	void addAnt(int posX, int posY, int colonyNum, Compiler* comp);
	bool compileFile(int fileNum, Compiler*& comp);
	int getCurrentTicks() const;
	void addFoodFromBody(int posX, int posY, int amount);
	void addAdultGrasshopper(int posX, int posY);
	void updateTickCount();
	void addPheromone(pheromone* pp, int colNum, int posX, int posY);
	void removeDead();
	void changeNumOfAnt(int Id, int Num);
	std::vector<insect*> actorOnTheSpot(int posX, int posY, insect* myself = nullptr); 
	template <class Item>
	void checkForItem2(Actor* a, Item*& f) 
	{
		f = dynamic_cast<Item*> (a);
	}
	template <class Item>
	void checkForItem(int posX, int posY, Item*& f, int n = 5, int m = 7)  //int n used for identify pheromone, m for anthill
	{
			std::list<Actor*>::iterator it;
			if (slot[posY][posX].size() == 0) return;
			it = slot[posY][posX].begin();
			while (it != slot[posY][posX].end())
			{
				// *it is a Actor pointer
				Actor* ss = *it;
				f = dynamic_cast<Item*> (ss);
				if (f != nullptr)
				{
					if (n == 5) break;
					else if (n < 5)
					{
						pheromone* pp = dynamic_cast<pheromone*> (ss);
						if (pp->getColony() == n && pp->getEnergy() > 0) break;
						else f = nullptr;
					}
					else // n>5 which is used to check the anthill object 
					{
						Anthill* aa = dynamic_cast<Anthill*> (ss);
						if (aa->getMyId() == m && aa->getEnergy() > 0) break;
						else f = nullptr;
					}
				}
				it++;
			}
			return;
	}
	void setDisplayText();
	void updateLocation(Actor* a, int oldX, int oldY);
	int getNumberOfAntsForAnt(int i) const;
	int getWinningAntNumber() const;
	std::vector<std::string> getAntNames();
private:
	std::list<Actor*> slot[VIEW_HEIGHT][VIEW_WIDTH]; // row first, column second
	int m_ticks;
	int numOfAnt[4];
	std::vector<std::string> antNames;
	int numOfPlayers; 
};
#endif // STUDENTWORLD_H_
