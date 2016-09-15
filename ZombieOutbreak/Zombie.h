#pragma once

#include "Agent.h"

//Zombie is a child of Agent
class Zombie : public Agent
{
public:
	Zombie();
	~Zombie();

	virtual void update(const std::vector<std::string>& levelData,
		std::vector<Human*>& humans,
		std::vector<Zombie*>& zombies);
};

