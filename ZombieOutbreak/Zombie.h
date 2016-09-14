#pragma once

#include "Agent.h"

//Zombie is a child of Agent
class Zombie : public Agent
{
public:
	Zombie();
	~Zombie();

	virtual void update();
};

