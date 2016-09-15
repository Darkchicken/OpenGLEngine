#pragma once

#include "Agent.h"

//Human is a child of Agent
class Human : public Agent
{
public:
	Human();
	//destructor must be virtual to call destructors of children as well as own
	virtual ~Human();

	virtual void update(std::vector<std::string>& levelData,
		std::vector<Human> humans,
		std::vector<Zombie> zombies);
};

