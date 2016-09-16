#pragma once

#include "Agent.h"

//Zombie is a child of Agent
class Zombie : public Agent
{
public:
	Zombie();
	~Zombie();
	//initialize zombie object
	void init(float speed, glm::vec2 position);

	virtual void update(const std::vector<std::string>& levelData,
		std::vector<Human*>& humans,
		std::vector<Zombie*>& zombies);

	//updated draw for zombie
	void draw(GameEngine::SpriteBatch& _spriteBatch) override;

private:

	Human* getNearestHuman(std::vector<Human*>& humans);
};

