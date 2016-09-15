#include "Bullet.h"
#include <GameEngine/ResourceManager.h>
#include "Agent.h"
#include "Human.h"
#include "Zombie.h"
#include "Level.h"

Bullet::Bullet(glm::vec2 position, glm::vec2 direction, float damage, float speed):
	_position(position),
	_direction(direction),
	_damage(damage),
	_speed(speed)
{


}


Bullet::~Bullet()
{
}

bool Bullet::update(const std::vector<std::string>& levelData)
{
	_position += _direction*_speed;
	return collideWithWorld(levelData);
}

void Bullet::draw(GameEngine::SpriteBatch& spriteBatch)
{
	//set bullet variables
	glm::vec4 destRect(_position.x + BULLET_RADIUS, _position.y + BULLET_RADIUS, BULLET_RADIUS*2, BULLET_RADIUS*2 );
	const glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);
	GameEngine::Color color;
	color.r = 75;
	color.g = 75;
	color.b = 75;
	color.a = 255;

	//draw the bullet
	spriteBatch.draw(destRect, uvRect, GameEngine::ResourceManager::getTexture("Textures/circle.png").id, 0.0f, color);
}

bool Bullet::collideWithAgent(Agent* agent)
{
	//minimum distance between agent and tile without a collision
	const float MIN_DISTANCE = AGENT_RADIUS + BULLET_RADIUS;

	//center point of this bullet
	glm::vec2 centerPosA = _position;
	//center point of colliding agent
	glm::vec2 centerPosB = agent->getPosition() + glm::vec2(AGENT_RADIUS);
	//distance between two agents
	glm::vec2 distVec = centerPosA - centerPosB;
	//get length of distance vec
	float distance = glm::length(distVec);
	//get depth of collision
	float collisionDepth = MIN_DISTANCE - distance;

	//check collision
	if (collisionDepth > 0)
	{

		//return that collision is true
		return true;
	}
	//return that collision is false
	return false;
}

bool Bullet::collideWithWorld(const std::vector<std::string>& levelData)
{
	//find current position
	glm::ivec2 gridPosition;
	gridPosition.x = floor(_position.x / (float)TILE_WIDTH);
	gridPosition.y = floor(_position.y / (float)TILE_WIDTH);

	//if we went out of bounds, return true
	//prevent agents from being placed outside of world
	if (gridPosition.x <0 || gridPosition.x > levelData[0].length() - 1 ||
		gridPosition.y <0 || gridPosition.y > levelData.size() - 1)
	{
		return true; //delete bullet
	}

	//return if there is a collision
	return (levelData[gridPosition.y][gridPosition.x] != '.');

}
