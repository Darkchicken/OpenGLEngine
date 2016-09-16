#pragma once

#include "Agent.h"

//Human is a child of Agent
class Human : public Agent
{
public:
	Human();
	//destructor must be virtual to call destructors of children as well as own
	virtual ~Human();
	//initialize human object
	void init(float speed, glm::vec2 position);

	virtual void update(const std::vector<std::string>& levelData,
		std::vector<Human*>& humans,
		std::vector<Zombie*>& zombies);

	//updated draw for human
	void draw(GameEngine::SpriteBatch& _spriteBatch) override;

private:
	glm::vec2 _direction;
	int _frames;
};

