#include "Actor.h"
#include "StudentWorld.h"
#include "Compiler.h"
#include <cmath>
#include <string>
#include <vector>

void ant::doSomething()
{
	changeEnergy(-1);
	//std::cout << "my health: " << getEnergy() << std::endl; //testing 
	bool bj = actionBasedOnStatus();
	if (!bj) return;
	//step 4: interpret between one to ten commands 
	//fetch the next command
	Compiler c = *m_comp;
	Compiler::Command cmd;
	for (int i = 0;i < 10;i++)
	{
		if (!c.getCommand(instructionCounter, cmd))
		{
			setToDeath();
			return;
		}
		switch (cmd.opcode)
		{
		case 7: // moveForward Checked 
		{
			bool bj = walkingInDirection(); // check whether can move
			if (bj)
			{
				whetherBeingbitten = false;
				whetherBeingBlocked = false;
				if (getPreviouslyWaterStunnedStatue()) changePreviousWaterStunned();
			}
			else whetherBeingBlocked = true; //remember being blocked
			instructionCounter++;
			return;
		}
		case 11: //eatFood Checked 
		{
			bool bj = eatFoodICarry();
			instructionCounter++;
			return;
		}
		case 10: //drop food Checked
		{
			if (m_holdFood > 0)
			{
				getMyWorld()->addFoodFromBody(getX(), getY(), m_holdFood); //drop all the food
				m_holdFood = 0; // set the amount I hold to 0;
			}
			instructionCounter++;
			break;
		}
		case 5:// rotate clockwise checked
		{
			Direction dir = getDirection();
			switch (dir)
			{
			case up:
				setDirection(right);
				break;
			case right:
				setDirection(down);
				break;
			case down:
				setDirection(left);
				break;
			case left:
				setDirection(up);
			}
			instructionCounter++;
			return;
		}
		case 6:// rotate counterclockwise checked
		{
			Direction dir = getDirection();
			switch (dir)
			{
			case up:
				setDirection(left);
				break;
			case right:
				setDirection(up);
				break;
			case down:
				setDirection(right);
				break;
			case left:
				setDirection(down);
			}
			instructionCounter++;
			return;
		}
		case 8: //bite checked
		{
			//find enemy on the spot

			std::vector<insect*> target2 = enemyOnTheSpot(getX(), getY());
			//randomize
			if (target2.size() != 0)
			{
				int random = randInt(0, target2.size());
				//do 15 damage
				//std::cout << "initial health: " << target2[random]->getEnergy() << std::endl; //testing
				target2[random]->beingBite(15);
				//std::cout << "after-biting health: " << target2[random]->getEnergy() << std::endl; //testing
			}
			instructionCounter++;
			return;
		}
			
		case 9: //pick up food checked 
		{
			//std::cout << "I currently have: " << m_holdFood << std::endl; //testing
			food* ff = nullptr;
			getMyWorld()->checkForItem(getX(), getY(), ff);
			if (ff == nullptr) //No food --> next instruction
			{
				instructionCounter++;
				return;
			}
			else
			{
				int amountOfFood = ff->getEnergy();
				int freeSpace = 1800 - m_holdFood;
				if (freeSpace >= 400 && amountOfFood >= 400) //there is enough space and enough food
				{
					m_holdFood += 400;
					ff->changeEnergy(-400);
					if (ff->getEnergy() == 0) ff->setToDeath();
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
						ff->setToDeath();
					}
				}
				//std::cout << "now I have:" << m_holdFood << std::endl; //testing
				instructionCounter++;
				return;
			}
		}
		case 3: //emitPheromone checked
		{
			pheromone* pp = nullptr;
			getMyWorld()->checkForItem(getX(), getY(), pp, getMyColonyNum());
			getMyWorld()->addPheromone(pp, getMyColonyNum(), getX(), getY());
			instructionCounter++;
			return;
		}
		case 4: //faceRandomDirection Checked
		{
			chooseRandomDirection();
			instructionCounter++;
			return;
		}
		case 12: //generateRandomNumber 
		{
			//convert cmd.operand1, a string, to int n
			int n = convertStringToInt(cmd.operand1);
			//randomize
			int random = 0;
			if (n != 0) random = randInt(0, n - 1); 
			lastRandomNum = random;//
			instructionCounter++;
			break;
		}
		case 1: //goto_command checked
		{
			int lineToGoto = convertStringToInt(cmd.operand1);
			instructionCounter = lineToGoto;
			break;
		}
		case 2: //if_command Partially Checked 
		{
			int n = convertStringToInt(cmd.operand1);
			switch (n)
			{
			case 0: //i_smell_danger_in_front_of_me checked
			{
				if (dangerInFrontOfMe())
				{
					instructionCounter = convertStringToInt(cmd.operand2);
					break;
				}
				else
				{
					instructionCounter++;
					break;
				}
			}
				
			case 1: //i_smell_pheromone_in_front_of_me  checked
			{
				if (pheromoneInFront())
				{
					instructionCounter = convertStringToInt(cmd.operand2);
					break;
				}
				else
				{
					instructionCounter++;
					break;
				}
			}
				
			case 2: //i_was_bit
			{
				if (whetherBeingbitten)
				{
					instructionCounter = convertStringToInt(cmd.operand2);
					break;
				}
				else
				{
					instructionCounter++;
					break;
				}
			}
				
			case 3: //i_am_carrying_food Checked
			{
				if (m_holdFood > 0)
				{
					instructionCounter = convertStringToInt(cmd.operand2);
					break;
				}
				else
				{
					instructionCounter++;
					break;
				}
			}				
			case 4: //i_am_hungry Checked
			{
				if (getEnergy() <= 25)
				{
					instructionCounter = convertStringToInt(cmd.operand2);
					break;
				}
				else
				{
					instructionCounter++;
					break;
				}
			}
			case 5: // i_am_standing_on_my_anthill checked
			{
				Anthill* aa = nullptr;
				getMyWorld()->checkForItem(getX(), getY(), aa, 6, getMyColonyNum());
				if (aa != nullptr) // if find the anthill
				{
					instructionCounter = convertStringToInt(cmd.operand2);
					break;
				}
				else
				{
					instructionCounter++;
					break;
				}
			}
			case 6: //i_am_standing_on_food Checked
			{
				food* ff = nullptr;
				getMyWorld()->checkForItem(getX(), getY(), ff);
				if (ff != nullptr)
				{
					instructionCounter = convertStringToInt(cmd.operand2);
					break;
				}
				else
				{
					instructionCounter++;
					break;
				}
			}
			case 7: //i_am_standing_with_an_enemy checked
			{
				std::vector<insect*> target = enemyOnTheSpot(getX(), getY());
				if (target.size() != 0)
				{
					instructionCounter = convertStringToInt(cmd.operand2);
					break;
				}
				else
				{
					instructionCounter++;
					break;
				}
			}
			case 8: //i_was_blocked_from_moving checked
			{
				if (whetherBeingBlocked)
				{
					instructionCounter = convertStringToInt(cmd.operand2);
					break;
				}
				else
				{
					instructionCounter++;
					break;
				}
			}
				
			case 9: //last_random_number_was_zero checked
			{
				if (lastRandomNum == 0) {
					instructionCounter = convertStringToInt(cmd.operand2);
					break;
				}
				else
				{
					instructionCounter++;
					break;
				}
			}
				
			}
		}
		}
	}
}
void ant::beingBite(int amount)
{
	insect::beingBite(amount);
	whetherBeingbitten = true;
}


//one line functions: 
bool Actor::checkStatus() const { return true; }
bool energyHolder::checkStatus() const { return m_status; }
void energyHolder::changeEnergy(int i) { m_energy += i; }
int energyHolder::getEnergy() const { return m_energy; }
void energyHolder::setToDeath() { m_status = false; }
void insect::stunned() { m_state = 2; }
bool insect::getPreviouslyWaterStunnedStatue() const { return previouslyWaterStunned; }
void insect::becomeActive() { m_state--; }
int insect::getMyState() const { return m_state; }
void insect::changePreviousWaterStunned() { previouslyWaterStunned = false; }
int ant::getMyColonyNum() const { return m_colonyNum; }
int Anthill::getMyId() const { return m_id; }
Compiler* Anthill::getMyCompiler() const { return m_comp; }
int grasshopper::getWalkDistance() const { return m_walkDistance; }
void grasshopper::takeOneStep() { m_walkDistance--; }
void grasshopper::setDistZero() { m_walkDistance = 0; }
StudentWorld* Actor::getMyWorld() const { return m_world; }
void adultGrasshopper::poisoned() {}
void adultGrasshopper::waterStunned() {}

//tested functions:
bool ant::dangerInFrontOfMe()
{
	Direction dir = getDirection();
	switch (dir)
	{
	case up:
		if (enemyOnTheSpot(getX(), getY() + 1).size() > 0) return true;
		break;
	case down:
		if (enemyOnTheSpot(getX(), getY() - 1).size() > 0) return true;
		break;
	case left:
		if (enemyOnTheSpot(getX() - 1, getY()).size() > 0) return true;
		break;
	case right:
		if (enemyOnTheSpot(getX() + 1, getY()).size() > 0) return true;
		break;
	}
	return false;
}
void insect::poisoned()
{
	changeEnergy(-150);
	//std::cout << "being poisoned" << std::endl; //test
	if (getEnergy() <= 0)
	{
		setToDeath();
		getMyWorld()->addFoodFromBody(getX(), getY(), 100);
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
void poison::doSomething()
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
		getMyWorld()->addFoodFromBody(getX(), getY(), 100); 
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
bool adultGrasshopper::jumpHigh()
{
	//select randomly 1 open sqare within a circular radius of 10 squares
	std::vector<int> xPosition, yPosition; //positions that adultgrasshopper can choose
	for (int i = 0; i < VIEW_WIDTH; i++)
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
	if (xPosition.size() == 0) return false;
	int random = randInt(0, xPosition.size() - 1);
	moveTo(xPosition[random], yPosition[random]);
	//std::cout << "jump to "<< xPosition[random] << ',' << yPosition[random] << std::endl; // testing
	//std::cout << "sleep/stunned after jumping" << std::endl; //testing
	return true;
}
int ant::convertStringToInt(std::string ss) //Checked
{
	std::string s = ss;
	int n = 0;
	for (int i = 0; i < s.length(); i++)
		n = n * 10 + int(s[i]) - int('0');
	return n;
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
			//std::cout << "Now I have" << getEnergy() << std::endl; //test
			return true;
		}
		else
		{
			//std::cout << "before:" << getEnergy() << std::endl; //test
			int energy = ff->getEnergy();
			//std::cout << "EAT " << energy << std::endl; // test
			changeEnergy(energy);

			//std::cout << "Now I have" << getEnergy() << std::endl; //test
			ff->changeEnergy(-(energy));
			ff->setToDeath();
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
	//std::cout << getEnergy() << std::endl; // testing
	changeEnergy(-1);
	bool bj = actionBasedOnStatus();
	if (!bj) return;
	//since the babygrasshopper is going to do something this round
	//check whether it is previously stunned by the water
	//if it is, set it to false
	if (getPreviouslyWaterStunnedStatue()) changePreviousWaterStunned();
	if (getEnergy() >= 1600) //Whether to transform itself into an adult grasshopper
	{
		//std::cout << "I died" << std::endl; //testing
		getMyWorld()->addAdultGrasshopper(getX(), getY());
		setToDeath();
		getMyWorld()->addFoodFromBody(getX(), getY(), 100);
		return;
	}
	eatAndSleep();
}

void adultGrasshopper::doSomething()
{
	changeEnergy(-1);
	bool bj = actionBasedOnStatus();
	if (!bj) return;//after stunned, immediately return
					//whether to bite the enemy 1/3
	int random = randInt(1, 3);
	bool bj2 = true;
	if (random == 1)
	{
		bj2 = bite();
		if (bj2)
		{
			stunned();
			return; // proceed to step 13;
		}
	}
	//else, decide whether to jump 1/10
	else
	{
		int random2 = randInt(1, 10);
		if (random2 == 1)
		{
			bool bj3 = true;
			bj3 = jumpHigh();
			if (bj3)
			{
				stunned();
				return;
			} //after stunned, immediately return
		}
	}
	//attempt to eat, walk and possibly sleep
	eatAndSleep();
}
//enermyOnTheSpot returns ant's enemy on spot (posX, posY)
std::vector<insect*> ant::enemyOnTheSpot(int posX, int posY)  
{
	//first step --> find all the insect on the spot
	std::vector<insect*> target = getMyWorld()->actorOnTheSpot(posX, posY, this);
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
	return target2;
}
void pheromone::doSomething()
{
	changeEnergy(-1);
	//std::cout << "I am Pheromone: with " << getEnergy() << std::endl; // testing
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
void Anthill::doSomething()
{
	changeEnergy(-1); 
	//std::cout << "Anthill energy: " << getEnergy() << std::endl; // testing 
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
		Compiler* antComp; 
		bool bb = getMyWorld()->compileFile(getMyId(), antComp); 
		getMyWorld()->addAnt(getX(), getY(), getMyId(), antComp);
		getMyWorld()->changeNumOfAnt(getMyId(), 1);
	}
}
bool ant::pheromoneInFront()
{
	Direction dir = getDirection();
	switch (dir)
	{
	case up:
	{
		pheromone* pp = nullptr;
		getMyWorld()->checkForItem(getX(), getY() + 1, pp, getMyColonyNum());
		if (pp != nullptr) return true;
		else return false;
	}
	case down:
	{
		pheromone* pp = nullptr;
		getMyWorld()->checkForItem(getX(), getY() - 1, pp, getMyColonyNum());
		if (pp != nullptr) return true;
		else return false;
	}
	case left:
	{
		pheromone* pp = nullptr;
		getMyWorld()->checkForItem(getX() - 1, getY(), pp, getMyColonyNum());
		if (pp != nullptr) return true;
		else return false;
	}
	case right:
	{
		pheromone* pp = nullptr;
		getMyWorld()->checkForItem(getX() + 1, getY(), pp, getMyColonyNum());
		if (pp != nullptr) return true;
		else return false;
	}
	}
	return false;
}
bool adultGrasshopper::bite()
{
	//select a random one, avoid biting itself
	std::vector<insect*> target = getMyWorld()->actorOnTheSpot(getX(), getY(), this);
	if (target.size() == 0) return false;
	int random = randInt(0, target.size() - 1);
	//cause damage
	//std::cout << "initial health: " << target[random]->getEnergy() << std::endl; //testing
	target[random]->beingBite(50);
	//std::cout << "after-biting health: " << target[random]->getEnergy() << std::endl; //testing
	return true;
}
void adultGrasshopper::beingBite(int amount)
{
	insect::beingBite(amount);
	if (getEnergy() > 0)
	{
		int random = randInt(1, 2);
		if (random == 1) bite();
	}
}
void insect::beingBite(int amount)
{
	changeEnergy(-amount);
	if (getEnergy() <= 0) setToDeath();
}
