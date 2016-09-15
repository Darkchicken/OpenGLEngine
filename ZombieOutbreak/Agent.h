#pragma once

#include <glm/glm.hpp>
#include <GameEngine/SpriteBatch.h>

const float AGENT_WIDTH = 60;//slightly smaller than tile

//forward declaration for zombie and human
class Zombie;
class Human;

class Agent
{
public:
	Agent();
	//destructor must be virtual to call destructors of children as well as own
	virtual ~Agent();
	//update for any agent
	virtual void update(std::vector<std::string>& levelData,
						std::vector<Human> humans,
						std::vector<Zombie> zombies) = 0; ///< pure virtual function, cannot create an agent, can only have derived classes, funciton must exist

	void collideWithLevel(const std::vector<std::string>& levelData);


	//same texture for every agent (may want to change this)
	void draw(GameEngine::SpriteBatch& _spriteBatch);

	const glm::vec2 getPosition() const { return _position; };

	
												

protected:
	void checkTilePosition(const std::vector<std::string>& levelData,std::vector<glm::vec2>& collideTilePositions,float cornerX, float cornerY );

	void collideWithTile();
	//position of agent
	glm::vec2 _position;
	//movement speed of agent
	float _speed;
	//color of agent
	GameEngine::Color _color;

	

	

};

