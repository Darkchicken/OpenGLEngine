#pragma once
#include<glm/glm.hpp>
#include <vector>

#include <GameEngine/SpriteBatch.h>

//forward declarations
class Human;
class Zombie;
class Agent;

const int BULLET_RADIUS = 5;

class Bullet
{
public:
	Bullet(glm::vec2 position,glm::vec2 direction,float damage,float speed);
	~Bullet();

	//when update returns true, delete bullet
	bool update(const std::vector<std::string>& levelData, float deltaTime);

	void draw(GameEngine::SpriteBatch& spriteBatch);

	//check collision with agent, return true if so
	bool collideWithAgent(Agent* agent);

	float getDamage()const { return _damage; };

private:
	
	bool collideWithWorld(const std::vector<std::string>& levelData);

	glm::vec2 _position;

	glm::vec2 _direction;

	float _damage;

	float _speed;

};

