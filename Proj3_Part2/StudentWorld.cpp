#include "StudentWorld.h"
#include "GameConstants.h"
#include "Field.h"
#include <string>
#include <sstream>
#include <vector>
#include <set>
#include <iomanip>
#include <list>
using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}
StudentWorld::~StudentWorld()
{
	//cout << "Destructing StudentWorld" << endl; //testing
	cleanUp(); 
}

bool StudentWorld::compileFile(int fileNum, Compiler*& comp) // compileFile with File number FileNum
{
	std::vector<std::string> fileNames = getFilenamesOfAntPrograms();
	comp = new Compiler;
	std::string error;
	if (!comp->compile(fileNames[fileNum], error))
	{
		// entrant 0¡¯s source code had a syntax error!
		setError(fileNames[fileNum] + " " + error);
		// return an error to tell our simulation framework
		// that something went wrong, and it¡¯ll inform the user
		return false;
	}
	return true;
}

int StudentWorld::init()
{
	//load the field into Field class object;
	numOfPlayers = getFilenamesOfAntPrograms().size();
	m_ticks = 0;
	Field f;
	std::string fieldFile = getFieldFilename();
	cout << fieldFile << endl;
	if (f.loadField(fieldFile) != Field::LoadResult::load_success)
		return GWSTATUS_LEVEL_ERROR; 
	//for every element in the Field object --> update into two-dimentional linked list "slot"
	for (int i = 0; i < VIEW_HEIGHT; i++)
		for (int j = 0; j < VIEW_WIDTH; j++)
		{
			Field::FieldItem item = f.getContentsOf(j, i); //column first, row second
			// what is the item on (i, j)
			if (item == Field::FieldItem::empty) continue;
			if (item == Field::FieldItem::rock)
			{
				Pebble* p = new Pebble(j, i, this);
				//pheromone* p = new pheromone(13, j, i, this, 2); //testing
				slot[i][j].push_back(p);
			}
			if (item == Field::FieldItem::water)
			{
				poolOfWater* w = new poolOfWater(j, i, this);
				slot[i][j].push_back(w);
			}
			if (item == Field::FieldItem::food)
			{
				food* w = new food(j, i, 6000, this); //testing
				slot[i][j].push_back(w); 
			}
			if (item == Field::FieldItem::poison)
			{
				poison* p = new poison(j, i, this);
				slot[i][j].push_back(p);
			}
			if (item == Field::FieldItem::grasshopper)
			{
				babyGrasshopper* g = new babyGrasshopper(j, i, this);
				slot[i][j].push_back(g);
			}
			if (item == Field::FieldItem::anthill0) 
			{
				Compiler *compilerForEntrant0;
				Anthill *ah0;
				bool bb = compileFile(0, compilerForEntrant0);
				if (bb == false) return GWSTATUS_LEVEL_ERROR;
				ah0 = new Anthill(j, i, this, 0, compilerForEntrant0);
				antNames.push_back(compilerForEntrant0->getColonyName());
				slot[i][j].push_back(ah0);
			}
			if (item == Field::FieldItem::anthill1 && numOfPlayers >= 2) 
			{
				Compiler *compilerForEntrant1;
				Anthill *ah1;
				bool bb = compileFile(1, compilerForEntrant1);
				if (bb == false) return GWSTATUS_LEVEL_ERROR;
				ah1 = new Anthill(j, i, this, 1, compilerForEntrant1);
				antNames.push_back(compilerForEntrant1->getColonyName());
				slot[i][j].push_back(ah1);
			}
			if (item == Field::FieldItem::anthill2 && numOfPlayers >= 3)
			{
				Compiler *compilerForEntrant2;
				Anthill *ah2;
				bool bb = compileFile(2, compilerForEntrant2);
				if (bb == false) return GWSTATUS_LEVEL_ERROR;
				ah2 = new Anthill(j, i, this, 2, compilerForEntrant2);
				antNames.push_back(compilerForEntrant2->getColonyName());
				slot[i][j].push_back(ah2);
			}
			if (item == Field::FieldItem::anthill3 && numOfPlayers >= 4)
			{
				Compiler *compilerForEntrant3;
				Anthill *ah3;
				bool bb = compileFile(3, compilerForEntrant3);
				if (bb == false) return GWSTATUS_LEVEL_ERROR;
				ah3 = new Anthill(j, i, this, 3, compilerForEntrant3);
				antNames.push_back(compilerForEntrant3->getColonyName());
				slot[i][j].push_back(ah3);
			}
		}
	return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
	set<Actor*> alreadyUpdate;
	updateTickCount(); // update the current tick # in the simulation
					   // The term "actors" refers to all ants, anthills, poison, pebbles,
					   // baby and adult grasshoppers, food, pools of water, etc.
					   // Give each actor a chance to do something
	// copy every actor from slot to slot2; and make motifications on slot
	for (int i = 0; i < VIEW_WIDTH; i++)
		for (int j = 0; j < VIEW_HEIGHT; j++)
		{
			list<Actor*>::iterator it;
			bool bj;
			if (i == 1 && j == 62)
				bj = true;
			if (slot[j][i].size() == 0) continue;
			it = slot[j][i].begin();
			list<Actor*> slotActors;
			 
			
			while (it != slot[j][i].end())
			{
				slotActors.push_back(*it); //prevent the problem of creating new actors
				it++;
			}
			it = slotActors.begin();
			while (it != slotActors.end())
			{
				set<Actor*>::iterator it2;
				it2 = alreadyUpdate.find(*it);

				if (it2 != alreadyUpdate.end())
				{
					it++;
					continue;
				}
				else alreadyUpdate.insert(*it);
				int oldX = (*it)->getX(), oldY = (*it)->getY();
				if ((*it)->checkStatus())
				{
					// ask each actor to do something (e.g. move)
					(*it)->doSomething();
				}
				Actor* a = *it;
				it++;
				if ((a)->getX() != oldX || (a)->getY() != oldY)
				{
					updateLocation((a), oldX, oldY);
					//cout << a->getX() << ',' << a->getY() << ' ' << oldX << ',' << oldY << endl; //testing;
				}
				
			}
		}
	
	// Remove newly-dead actors after each tick
	removeDead(); // delete dead simulation objects
								   // Update the simulation Status Line
	setDisplayText(); // update the ticks/ant stats text at screen top
						 // If the simulation¡¯s over (ticks == 2000) then see if we have a winner
	if (m_ticks == 2000)
	{
		if (getWinningAntNumber() != 5)
		{
			vector<string> progName = getAntNames();
			setWinner(progName[getWinningAntNumber()]);
			return GWSTATUS_PLAYER_WON;
		}
		else
			return GWSTATUS_NO_WINNER;
	}
	return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
	
	for (int i = 0; i < VIEW_HEIGHT; i++)
		for (int j = 0; j < VIEW_WIDTH; j++)
		{
			while (!slot[j][i].empty())
			{
				Actor* ss = slot[j][i].front();
				delete ss;
				slot[j][i].pop_front();
			}
			
		}
}

//tested functions:
void StudentWorld::addAdultGrasshopper(int posX, int posY)
{
	adultGrasshopper* f = new adultGrasshopper(posX, posY, this);
	slot[posY][posX].push_back(f);
}
void StudentWorld::addAnt(int posX, int posY, int colonyNum, Compiler* comp)
{
	ant* n = new ant(colonyNum, posX, posY, this, comp, colonyNum);
	slot[posY][posX].push_back(n);
}
int StudentWorld::getCurrentTicks() const { return m_ticks; }
void StudentWorld::updateTickCount() { m_ticks++; }
void StudentWorld::addFoodFromBody(int posX, int posY, int amount)
{
	food* f = nullptr;
	checkForItem(posX, posY, f);
	if (f != nullptr)
		f->changeEnergy(amount); // if there is already a food object on (posX, posY)
	else // else create a new food object on StudentWorld
	{
		f = new food(posX, posY, amount, this);
		slot[posY][posX].push_back(f);
	}
}
void StudentWorld::updateLocation(Actor* a, int oldX, int oldY)
{
	//delete a in slot[oldY][oldX]
	slot[oldY][oldX].remove(a);
	//update to the new location
	int newY = a->getY(), newX = a->getX();
	slot[newY][newX].push_back(a);
}
void StudentWorld::addPheromone(pheromone* pp, int colNum, int posX, int posY)
{
	if (pp == nullptr)
	{
		pp = new pheromone(11 + colNum, posX, posY, this, colNum);
		slot[posY][posX].push_back(pp);
	}
	else
	{
		pp->changeEnergy(256);
		if (pp->getEnergy() >= 768) pp->changeEnergy(-(pp->getEnergy() - 768));
	}
}
void StudentWorld::removeDead()
{
	for (int i = 0; i< VIEW_WIDTH; i++)
		for (int j = 0; j < VIEW_HEIGHT; j++)
		{
			list<Actor*>::iterator it;
			if (slot[j][i].size() == 0) continue;
			it = slot[j][i].begin();
			while (it != slot[j][i].end()) // for every Actor in the StudentWorld
			{
				if ((*it)->checkStatus() == false) //Check its status, if it is "dead" 
				{
					Actor* a = *it;
					delete a;
					it++;
					slot[j][i].remove(a);
				}
				else it++; // if its not
			}
		}
}
vector<insect*> StudentWorld::actorOnTheSpot(int posX, int posY, insect* myself) 
{
	list<Actor*>::iterator it;
	vector<insect*> result;
	it = slot[posY][posX].begin();
	while (it != slot[posY][posX].end())
	{
		adultGrasshopper* gg = nullptr;
		checkForItem2(*it, gg);
		babyGrasshopper* bb = nullptr;
		checkForItem2(*it, bb);
		ant* aa = nullptr;
		checkForItem2(*it, aa);
		if (aa != nullptr && aa != myself) result.push_back(aa);
		if (bb != nullptr && bb != myself) result.push_back(bb);
		if (gg != nullptr && gg != myself) result.push_back(gg);
		it++;
	}
	return result;
}
vector<string> StudentWorld::getAntNames() { return antNames; }
void StudentWorld::changeNumOfAnt(int Id, int Num) { numOfAnt[Id] += Num; }
int StudentWorld::getNumberOfAntsForAnt(int i) const { return numOfAnt[i]; }
int StudentWorld::getWinningAntNumber()const 
{

	int max = numOfAnt[0], num = 0;

	for (int i = 1; i < 4; i++)
	{
		if (numOfAnt[i] > max)
		{
			max = numOfAnt[i];
			num = i;
		}
	}
	if (numOfAnt[0] == max && num != 0 ||
		numOfAnt[1] == max && num != 1 ||
		numOfAnt[2] == max && num != 2 ||
		numOfAnt[3] == max && num != 3) return 5;
	return num;
}
void StudentWorld::setDisplayText()
{
	int ticks = getCurrentTicks();
	int antsAnt[4];
	int winningAntNumber;
	for (int i = 0; i < 4; i++)
		antsAnt[i] = getNumberOfAntsForAnt(i);
	winningAntNumber = getWinningAntNumber();
	// Create a string from your statistics, of the form:
	// Ticks: 1134 - AmyAnt: 32 BillyAnt: 33 SuzieAnt*: 77 IgorAnt: 05
	vector<string> progName = getAntNames();
	ostringstream oss;
	oss << "Ticks:" << setw(5) << 2000 - ticks << " - ";
	oss.fill('0');
	int size = progName.size();
	for (int i = 0; i < size; i++)
	{
		if (i == winningAntNumber) oss << progName[i] << "*: " << setw(2) << antsAnt[i] << " ants" << "  ";
		else oss << progName[i] << ": " << setw(2) << antsAnt[i] << " ants" << "  ";
	}
	string s = oss.str();
	// Finally, update the display text at the top of the screen with your
	// newly created stats
	setGameStatText(s); // calls our provided GameWorld::setGameStatText
}