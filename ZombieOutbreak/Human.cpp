#include "Human.h"
#include <ctime>
#include <random>

Human::Human()
{
}


Human::~Human()
{
}

void Human::init(float speed, glm::vec2 position)
{
	//create a static random engine
	static std::mt19937 randomEngine;
	randomEngine.seed(time(nullptr));
	static std::uniform_real_distribution<float> randDir(-1.0f, 1.0f);

	_speed = speed;
	_position = position;
	//randomly assign direction
	_direction = glm::vec2(randDir(randomEngine), randDir(randomEngine));
	//check for a 0 direction
	if (_direction.length() == 0)
	{
		_direction = glm::vec2(1.0f, 0.0f);
	}
	//set direction vector to unit length
	_direction = glm::normalize(_direction);
}

void Human::update(const std::vector<std::string>& levelData,
	std::vector<Human*>& humans,
	std::vector<Zombie*>& zombies)
{
	//move human
	_position += _direction * _speed;

	//handle collisions during update
	collideWithLevel(levelData);

}
