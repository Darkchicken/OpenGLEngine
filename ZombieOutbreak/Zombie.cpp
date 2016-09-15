#include "Zombie.h"

#include "Human.h"

Zombie::Zombie()
{
}


Zombie::~Zombie()
{
}

void Zombie::init(float speed, glm::vec2 position)
{
	_speed = speed;
	_position = position;

	//set zombie health
	_health = 150.0f;

	_color.r = 0;
	_color.g = 160;
	_color.b = 0;
	_color.a = 255;
}


void Zombie::update(const std::vector<std::string>& levelData,
	std::vector<Human*>& humans,
	std::vector<Zombie*>& zombies)
{
	//chase nearest humans
	Human* closestHuman = getNearestHuman(humans);
	if (closestHuman != nullptr)
	{
		//get direction of closest human as unit vector
		glm::vec2 direction = glm::normalize(closestHuman->getPosition() - _position);

		_position += direction * _speed;
	}

	//handle collisions during update
	collideWithLevel(levelData);
	

}

Human* Zombie::getNearestHuman(std::vector<Human*>& humans)
{
	//a pointer that will hold the closest human object
	Human* closestHuman = nullptr;
	//distance to closest human, will be reduced as humans are found
	float smallestDistance = 99999999.0f;

	//loop through all human objects
	for (int i = 0; i < humans.size(); i++)
	{
		//distance vector between current human and zombie
		glm::vec2 distVec = humans[i]->getPosition() - _position;
		//get length of distance vecotr
		float distance = glm::length(distVec);

		//if the distance to this human is less than the smallest distance so far
		if (distance < smallestDistance)
		{
			//set the smallest distance to this distance
			smallestDistance = distance;
			//set closest human to current human
			closestHuman = humans[i];
		}
	}
	return closestHuman;
}
