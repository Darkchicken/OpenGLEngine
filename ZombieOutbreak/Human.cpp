#include "Human.h"
#include <ctime>
#include <random>
#include <glm/gtx/rotate_vector.hpp>

Human::Human() :
	_frames(0)
{
}


Human::~Human()
{
}

void Human::init(float speed, glm::vec2 position)
{
	//create a static random engine and seed with time
	static std::mt19937 randomEngine(time(nullptr));
	
	//create a distribution that random numbers can fall between
	static std::uniform_real_distribution<float> randDir(-1.0f, 1.0f);

	//set variables
	_color.r = 200;
	_color.g = 0;
	_color.b = 200;
	_color.a = 255;

	_speed = speed;
	_position = position;
	//randomly assign direction
	_direction = glm::vec2(randDir(randomEngine), randDir(randomEngine));

	//check for a 0 direction
	if (_direction.length() == 0)
	{_direction = glm::vec2(1.0f, 0.0f);}

	//set direction vector to unit length
	_direction = glm::normalize(_direction);
}

void Human::update(const std::vector<std::string>& levelData,
	std::vector<Human*>& humans,
	std::vector<Zombie*>& zombies)
{
	//create a static random engine and seed with time
	static std::mt19937 randomEngine(time(nullptr));
	//create a distribution that random numbers can fall between
	static std::uniform_real_distribution<float> randRotate(-20.0f, 20.0f);

	//move human
	_position += _direction * _speed;
	//randomly change direction every 20 frames
	if (_frames == 20)
	{
		_direction = glm::rotate(_direction, randRotate(randomEngine));
		_frames = 0;
	}
	else
	{
		_frames++;
	}
	

	//handle collisions during update
	collideWithLevel(levelData);

}
