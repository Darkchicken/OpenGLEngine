#pragma once

#include <glm/glm.hpp>
#include <GameEngine/SpriteBatch.h>

const float AGENT_WIDTH = 60;//slightly smaller than tile

class Agent
{
public:
	Agent();
	//destructor must be virtual to call destructors of children as well as own
	virtual ~Agent();
	//update for any agent
	virtual void update() = 0; ///< pure virtual function, cannot create an agent, can only have derived classes, funciton must exist

	//same texture for every agent (may want to change this)
	void draw(GameEngine::SpriteBatch& _spriteBatch);

	const glm::vec2 getPosition() const { return _position; };

	
												

protected:
	//position of agent
	glm::vec2 _position;
	//movement speed of agent
	float _speed;
	//color of agent
	GameEngine::Color _color;

	

	

};

