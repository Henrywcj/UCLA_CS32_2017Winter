#include "Actor.h"
#include "StudentWorld.h"
#include "Compiler.h"
#include <cmath>
#include <string>
#include <vector>
#include <iostream> //for testing

//problem about creating adult grasshopper from baby grasshopper
StudentWorld* Actor::getMyWorld() { return m_world; }
bool ant::eatFoodICarry()
{
	if (m_holdFood == 0) return false;
	else if (m_holdFood >= 100)
	{
		changeEnergy(100);
		m_holdFood -= 100;
		return true;
	}
	else
	{
		changeEnergy(m_holdFood);
		m_holdFood = 0;
		return true;
	}
}
int ant::convertStringToInt(std::string ss)
{
	std::string s = ss;
	int n = 0;
	for (int i = 0; i < s.length(); i++)
		n = n * 10 + int(s[i]) - int('0');
	return n;
}
bool ant::dangerInFrontOfMe()  //need implementation, pretty similar with walkIndirection Function
{
	//Direction dir = getDirection();
	return false;
}
void ant::doSomthing() // need testing
{
	changeEnergy(-1);
	bool bj = actionBasedOnStatus();
	if (!bj) return;
	//since the babygrasshopper is going to do something this round
	//check whether it is previously stunned by the water
	//if it is, set it to false
	if (getPreviouslyWaterStunnedStatue()) changePreviousWaterStunned();
	//step 4: interpret between one to ten commands 
	//fetch the next command
	Compiler c = *m_comp;
	Compiler::Command cmd;
	for (;;) //maybe 10 moves maximum??
	{
		if (!c.getCommand(instructionCounter, cmd))
		{
			setToDeath();
			return;
		}
		switch (cmd.opcode)
		{
		case 7: // moveForward
		{
			bool bj = walkingInDirection(); // check whether can move
			if (bj)
			{
				whetherBeingbitten = false;
				whetherBeingBlocked = false;
			}
			else whetherBeingBlocked = true; //remember being blocked
			instructionCounter++;
			break;
		}
		case 11: //eatFood is it changing the simulation world??
		{
			bool bj = eatFoodICarry();
			instructionCounter++;
			break;
			// need to stop??
		}
			
		case 10: //drop food
			getMyWorld()->addFoodFromBody(getX(), getY(), m_holdFood); //drop all the food
			m_holdFood = 0; // set the amount I hold to 0;
			instructionCounter++;
			break;
		case 8:
		{
			//randomly choose one enemy on the spot
			//first step --> find all the insect on the spot
			std::vector<insect*> target = getMyWorld()->actorOnTheSpot(getX(), getY());
			//second step --> remove all friendly ants, generating a new vector of insect*, target2
			std::vector<insect*>::iterator it;
			std::vector<insect*> target2;
			it = target.begin();
			while (it != target.end())
			{
				ant* aa = nullptr;
				getMyWorld()->checkForItem2(*it, aa); //check whether it is an ant
				if (aa != nullptr)
				{
					//check whether it is the same anthill
					if (aa->getMyColonyNum() == getMyColonyNum())it++;
					else //ant object, but not the same anthill
					{
						target2.push_back(*it);
						it++;
					}
				}
				else //not an ant, but an insect
				{
					target2.push_back(*it);
					it++;
				}
			}
			//third step --> randomize
			int random = randInt(0, target2.size());
			//do 15 damage
			target2[random]->beingBite(15); //does bite change simulation world??
			instructionCounter++;
			break;
		}
			
		case 9: //pick up food
		{
			food* ff = nullptr;
			getMyWorld()->checkForItem(getX(), getY(), ff);
			if (ff == nullptr) //No food --> next instruction
			{
				instructionCounter++;
				break;
			}
			else
			{
				int amountOfFood = ff->getEnergy();
				int freeSpace = 1800 - m_holdFood;
				if (freeSpace >= 400 && amountOfFood >= 400) //there is enough space and enough food
				{
					m_holdFood += 400;
					ff->changeEnergy(-400);
				}
				else
				{
					if (freeSpace < amountOfFood) // not enough space 
					{
						m_holdFood += freeSpace;
						ff->changeEnergy(freeSpace);
					}
					else // not enough food 
					{
						m_holdFood += amountOfFood;
						ff->changeEnergy(amountOfFood);
					}
				}
				instructionCounter++;
				break;
			}
		}
		//case 3: //emitPheromone
		case 4: //faceRandomDirection
			chooseRandomDirection();
			instructionCounter++;
			break;
		case 12: //generateRandomNumber
		{
			//convert cmd.operand1, a string, to int n
			int n = convertStringToInt(cmd.operand1);
			//randomize
			int random = 0;
			if (n != 0) random = randInt(0, n - 1); //where should I store this??
			instructionCounter++;
			break;
		}
		case 1: //goto_command
		{
			int lineToGoto = convertStringToInt(cmd.operand1);
			instructionCounter = lineToGoto;
			break;
		}
		case 2: //if_command
		{
			int n = convertStringToInt(cmd.operand1);
			/*i_smell_danger_in_front_of_me,		// 0
				i_smell_pheromone_in_front_of_me,	// 1
				i_was_bit,							// 2
				i_am_carrying_food,					// 3
				i_am_hungry,						// 4
				i_am_standing_on_my_anthill,		// 5
				i_am_standing_on_food,				// 6
				i_am_standing_with_an_enemy,		// 7
				i_was_blocked_from_moving,			// 8
				last_random_number_was_zero			// 9*/
			switch (n)
			{
			case 0:
				if (dangerInFrontOfMe())
				{
					instructionCounter = convertStringToInt(cmd.operand2);
					break;
				}
				else break;
			case 1: //need implementation 
				if (true)
				{ }
				else break;
				//.... more 
			}
			instructionCounter++;
			break;
		}
		//case 
		}
	}
	
}
void poolOfWater::doSomething() 
{
	std::vector<insect*> target = getMyWorld()->actorOnTheSpot(getX(), getY());
	std::vector<insect*>::iterator it;
	it = target.begin();
	while (it != target.end())
	{
		(*it)->waterStunned(); 
		//std::cout << "I stunned Something" << std::endl;  // test
		it++;
	}
}
void poison::doSomething() // need testing 
{
	std::vector<insect*> target = getMyWorld()->actorOnTheSpot(getX(), getY());
	std::vector<insect*>::iterator it;
	it = target.begin();
	while (it != target.end())
	{
		(*it)->poisoned(); //being poisoned, causing damage 150
		//std::cout << "I poisoned something" << std::endl;
		it++;
	}
}
void insect::chooseRandomDirection()
{
	int i = randInt(1, 4);
	if (i == 1) setDirection(up);
	if (i == 2) setDirection(right);
	if (i == 3) setDirection(down);
	if (i == 4) setDirection(left);
}
void grasshopper::Reset()
{
	m_walkDistance = randInt(2, 10);
	chooseRandomDirection();
	//std::cout << getDirection() << ' ' << m_walkDistance << std::endl; // testing
}
bool insect::walkingInDirection() //return true if can move, false if cannot move 
{
	GraphObject::Direction d = getDirection();
	if (d == GraphObject::Direction::up)
	{
		Pebble* pp = nullptr;
		getMyWorld()->checkForItem(getX(), getY() + 1, pp);
		if (pp == nullptr)
		{
			moveTo(getX(), getY() + 1); //if there is no Pebbles;
			return true;
		}
		else return false;
	}
	if (d == GraphObject::Direction::right)
	{
		Pebble* pp = nullptr;
		getMyWorld()->checkForItem(getX() + 1, getY(), pp);
		if (pp == nullptr)
		{
			moveTo(getX() + 1, getY()); //if there is no Pebbles;
			return true;
		}
		else return false;
	}
	if (d == GraphObject::Direction::down)
	{
		Pebble* pp = nullptr;
		getMyWorld()->checkForItem(getX(), getY() - 1, pp);
		if (pp == nullptr)
		{
			moveTo(getX(), getY() - 1); //if there is no Pebbles;
			return true;
		}
		else return false;
	}
	if (d == GraphObject::Direction::left)
	{
		Pebble* pp = nullptr;
		getMyWorld()->checkForItem(getX() - 1, getY(), pp);
		if (pp == nullptr)
		{
			moveTo(getX() - 1, getY()); //if there is no Pebbles;
			return true;
		}
		else return false;
	}
}
bool insect::actionBasedOnStatus()
{
	//std::cout << getEnergy() << std::endl; // testing
	if (getEnergy() <= 0) // check whether it is alive
	{
		//add 100 food in Simulation World 
		getMyWorld()->addFoodFromBody(getX(), getY(), 100); // need implement addFoodFromBody Function
		setToDeath();													//set state to dead 
		return false;
	}
	//std::cout << getMyState() << std::endl; // testing
	if (getMyState() > 0) // check whether it is stunned   PROBLEM WITH ADULTGRASSHOPPER
	{
		becomeActive(); //decrement the count of ticks 
		//std::cout << "stunned! decreasing by one" << std::endl; //testing
		return false;
	}
	return true;
}

void adultGrasshopper::jumpHigh()// need testing 
{
	//select randomly 1 open sqare within a circular radius of 10 squares
	std::vector<int> xPosition, yPosition; //positions that adultgrasshopper can choose
	for (int i = 0 ; i < VIEW_WIDTH; i++)
		for (int j = 0; j < VIEW_HEIGHT; j++)
		{
			double distance = sqrt(abs(getX() - i)*abs(getX() - i) + abs(getY() - j) * abs(getY() - j));
			Pebble* pp = nullptr;
			getMyWorld()->checkForItem(i, j, pp);
			if (distance <= 10.0 && pp == nullptr)
			{
				xPosition.push_back(i);
				yPosition.push_back(j);
			}
		}
	int random = randInt(0, xPosition.size() - 1);
	moveTo(xPosition[random], yPosition[random]);
	//std::cout << "jump to "<< xPosition[random] << ',' << yPosition[random] << std::endl; // testing
	//std::cout << "sleep/stunned after jumping" << std::endl; //testing
	stunned();
}

bool energyHolder::attemptToEat(int amount)
{
	food* ff = nullptr;
	getMyWorld()->checkForItem(getX(), getY(), ff);
	if (ff != nullptr) //Whether to eat
	{
		if (ff->getEnergy() >= amount)
		{
			ff->changeEnergy(-amount);
			changeEnergy(amount);
			if (ff->getEnergy() == 0) ff->setToDeath();
			//std::cout << "EAT 200" << std::endl; // test
			return true;
		}
		else
		{
			ff->changeEnergy(-(ff->getEnergy()));
			changeEnergy(ff->getEnergy());
			ff->setToDeath();
			//std::cout << "EAT " << ff->getEnergy() << std::endl; // test
			return true;
		}
	}
	return false;
}
void grasshopper::eatAndSleep()
{
	bool bb = attemptToEat(200);
	int random = randInt(1, 2); //contruct a 50% chance 
	if (random == 1 && bb) //if choose to sleep...
	{
		stunned();
		//std::cout << "I am stunned" << std::endl; //testing
		return;
	}
	if (getWalkDistance() == 0)
		Reset();//if finished walking in this direction
	bool bj = walkingInDirection(); //try to take one step
	if (!bj) //if blocked/failed
	{
		setDistZero();
		//std::cout << "I am Stunned" << std::endl; // testing
		stunned();
		return;
	}
	else //if succeed
	{
		takeOneStep();
		stunned();
	}
}
void babyGrasshopper::doSomething()
{
	changeEnergy(-1);
	bool bj = actionBasedOnStatus();
	if (!bj) return;
	//since the babygrasshopper is going to do something this round
	//check whether it is previously stunned by the water
	//if it is, set it to false
	if (getPreviouslyWaterStunnedStatue()) changePreviousWaterStunned(); 
	if (getEnergy() >= 1600) //Whether to transform itself into an adult grasshopper
	{
		getMyWorld()->addAdultGrasshopper(getX(), getY());
		setToDeath();
		getMyWorld()->addFoodFromBody(getX(), getY(), 100);
		return;
	}
	eatAndSleep();
}
void adultGrasshopper::doSomething() //need implementation 
{
	changeEnergy(-1);
	bool bj = actionBasedOnStatus();
	if (!bj) return;//after stunned, immediately return
	//whether to bite the enemy 1/3
	int random = randInt(1, 3);
	if (random == 1) bite();
	else
	{
		int random2 = randInt(1, 10);
		if (random2 == 1)
		{
			jumpHigh();
			return; //after stunned, immediately return
		}
	}
	//else, decide whether to jump 1/10
	//attempt to eat and possibly sleep
	eatAndSleep();
}

void adultGrasshopper::bite() // need further consideration, testing
{
	//select a random one
	std::vector<insect*> target = getMyWorld()->actorOnTheSpot(getX(), getY());
	int random = randInt(0, target.size() - 1);
	//cause damage
	target[random]->beingBite(50);
	stunned();
}
void adultGrasshopper::beingBite(int amount)//need testing
{
	insect::beingBite(amount);
	if (getEnergy() > 0)
	{
		int random = randInt(1, 2);
		if (random == 1) bite();
	}
}
void Anthill::doSomething()
{
	changeEnergy(-1);
	if (getEnergy() <= 0)//check whether no energy
	{
		setToDeath();
		return;
	}
	bool bb = attemptToEat(10000);//Eat if there is food
	if (bb) return; //if ate, immediately return 
	if (getEnergy() >= 2000)//Whether to create a new ant
	{
		changeEnergy(-1500);
		getMyWorld()->addAnt(getX(), getY(), getMyId(), getMyCompiler());
		getMyWorld()->changeNumOfAnt(getMyId(), 1);
	}
}
void pheromone::doSomething()
{
	changeEnergy(-1);
	if (getEnergy() == 0) setToDeath();
}
void insect::waterStunned() 
{ 
	if (!previouslyWaterStunned) //in order to prevent permanent stun
	{
		m_state += 2;
		previouslyWaterStunned = true;
	}
}
//one line functions: 
bool Actor::checkStatus() { return true; }
bool energyHolder::checkStatus() { return m_status; }
void energyHolder::changeEnergy(int i) { m_energy += i; }
int energyHolder::getEnergy() { return m_energy; }
void energyHolder::setToDeath() { m_status = false; }
void insect::poisoned() { changeEnergy(-150); }
void insect::beingBite(int amount) { changeEnergy(-amount); }
void insect::stunned() { m_state = 2; }
bool insect::getPreviouslyWaterStunnedStatue() { return previouslyWaterStunned; }
void insect::becomeActive() { m_state--; }
int insect::getMyState() { return m_state; }
void insect::changePreviousWaterStunned() { previouslyWaterStunned = false; }
int ant::getMyColonyNum() { return m_colonyNum; }
int Anthill::getMyId() { return m_id; }
Compiler* Anthill::getMyCompiler() { return m_comp; }
int grasshopper::getWalkDistance() { return m_walkDistance; }
void grasshopper::takeOneStep() { m_walkDistance--; }
void grasshopper::setDistZero() { m_walkDistance = 0; }
void adultGrasshopper::poisoned() {}
void adultGrasshopper::waterStunned() {}
// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp

/*
GraphObject(int imageID, int startX, int startY, Direction startDirection = none);
void setVisible(bool shouldIDisplay);
void getX() const;
void getY() const;
void moveTo(int x, int y);
Direction getDirection() const; // Directions: none, up, down, left, right
void setDirection(Direction d); // Directions: none, up, down, left, right*/