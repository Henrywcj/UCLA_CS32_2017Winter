#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "Compiler.h"
#include <string>

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
class StudentWorld;
class Actor :public GraphObject
{
public: 
	Actor(int imageID, int startX, int startY, StudentWorld* myWorld, 
		Direction startDirection = right, unsigned int depth = 2) :
		GraphObject(imageID, startX, startY, startDirection, depth),m_world(myWorld) {}
	virtual void doSomething() {}
	virtual bool checkStatus();
	StudentWorld* getMyWorld();
private:
	StudentWorld* m_world;
};
class poolOfWater : public Actor
{
public:
	poolOfWater(int startX, int startY, StudentWorld* myWorld) :
		Actor(IID_WATER_POOL, startX, startY, myWorld, right, 2) {}
	virtual void doSomething();
};
class poison :public Actor
{
public:
	poison(int startX, int startY, StudentWorld* myWorld) :
		Actor(IID_POISON, startX, startY, myWorld, right, 2) {}
	virtual void doSomething();
};
class energyHolder : public Actor
{
public:
	energyHolder(int imageID, int startX, int startY, StudentWorld* myWorld, int energy,
		Direction startDirection = right, unsigned int depth = 2) :
		Actor(imageID, startX, startY, myWorld, startDirection, depth), m_energy(energy), m_status(true) {}
	virtual bool checkStatus();
	void changeEnergy(int i);
	int getEnergy();
	void setToDeath();
	bool attemptToEat(int amount); //return true if eat something, return false if not eat 
private: 
	int m_energy;
	bool m_status;
};
class insect : public energyHolder
{
public:
	insect(int imageID, int startX, int startY, int hitPoint, StudentWorld* myWorld,
		Direction startDirection = right, unsigned int depth = 2) :
		energyHolder(imageID, startX, startY, myWorld, hitPoint, startDirection, depth), m_state(0),previouslyWaterStunned(false)
	{
		int dir = randInt(1, 4);
		if (dir == 1) setDirection(up);
		if (dir == 2) setDirection(right); 
		if (dir == 3) setDirection(down);
		if (dir == 4) setDirection(left);
	}
	void changePreviousWaterStunned();
	bool getPreviouslyWaterStunnedStatue();
	bool actionBasedOnStatus();
	void chooseRandomDirection();
	virtual void poisoned();
	virtual void beingBite(int amount);
	void stunned();
	virtual void waterStunned();
	void becomeActive();
	int getMyState();
	//bite function in here or what
	bool walkingInDirection();
private:
	bool previouslyWaterStunned;
	int m_state; //0 is active, 1/2 is sleep/stun 
};
class food :public energyHolder
{
public:
	food(int startX, int startY, int food_amount, StudentWorld* myWorld) :
		energyHolder(IID_FOOD, startX, startY, myWorld, food_amount) {}
};
class pheromone : public energyHolder
{
public:
	pheromone(int imageID, int startX, int startY, StudentWorld* myWorld) :
		energyHolder(imageID, startX, startY, myWorld, 256) {}
	virtual void doSomething();
};

class ant :public insect
{
public:
	ant(int imageID, int startX, int startY, StudentWorld* myWorld, Compiler* comp, int colonyNum) :
		insect(imageID, startX, startY, 1500, myWorld, right, 1), m_comp(comp), 
		m_colonyNum(colonyNum), m_holdFood(0), instructionCounter(0),whetherBeingbitten(false), whetherBeingBlocked(false){}
	virtual void doSomthing();
	int getMyColonyNum();
	int convertStringToInt(std::string s);
	bool dangerInFrontOfMe();
	bool eatFoodICarry(); //return true of eat something, false if eat nothing
private:
	int instructionCounter;
	bool whetherBeingBlocked; // false means not blocked
	bool whetherBeingbitten; // false means not bitten
	int m_colonyNum;
	Compiler* m_comp;
	int m_holdFood;
};

class Anthill :public energyHolder //need test
{
public:
	Anthill(int startX, int startY, StudentWorld* myWorld, int ID, Compiler* comp):
		energyHolder(IID_ANT_HILL, startX, startY, myWorld, 8999), m_id(ID), m_comp(comp){}
	virtual void doSomething();
	int getMyId();
	Compiler* getMyCompiler();
private:
	int m_id; //0, 1, 2, 3
	Compiler* m_comp;
};


class Pebble :public Actor
{
public:
	Pebble(int startX, int startY, StudentWorld* myWorld) :
		Actor(IID_ROCK, startX, startY, myWorld, right, 1) {};
};



class grasshopper : public insect
{
public:
	grasshopper(int imageID, int startX, int startY, int hitPoint, StudentWorld* myWorld) :
		insect(imageID, startX, startY, hitPoint, myWorld, right, 2), m_walkDistance(randInt(2,10)) {}
	int getWalkDistance();
	void takeOneStep();
	void setDistZero();
	void eatAndSleep();
	void Reset();
private:
	int m_walkDistance;
};
class babyGrasshopper :public grasshopper
{
public:
	babyGrasshopper(int startX, int startY, StudentWorld* myWorld) :
		grasshopper(IID_BABY_GRASSHOPPER, startX, startY, 500, myWorld) {} //testing
	virtual void doSomething(); 
};
class adultGrasshopper : public grasshopper
{
public:
	adultGrasshopper(int startX, int startY, StudentWorld* myWorld):
		grasshopper(IID_ADULT_GRASSHOPPER,startX, startY, 1600, myWorld) {}
	virtual void doSomething();
	void bite();
	void jumpHigh();
	virtual void beingBite(int amount);
	virtual void poisoned();
	virtual void waterStunned();
};
#endif // ACTOR_H_

