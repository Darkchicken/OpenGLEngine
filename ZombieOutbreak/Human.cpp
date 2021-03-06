#include "Human.h"
#include <ctime>
#include <random>
#include <glm/gtx/rotate_vector.hpp>
#include <GameEngine/ResourceManager.h>

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

	//set human health
	_health = 20.0f;

	//set variables
	_color.r = 255;
	_color.g = 255;
	_color.b = 255;
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
	std::vector<Zombie*>& zombies,
	float deltaTime)
{
	//create a static random engine and seed with time
	static std::mt19937 randomEngine(time(nullptr));
	//create a distribution that random numbers can fall between
	static std::uniform_real_distribution<float> randRotate(-40.0f, 40.0f);
	const float DEG_TO_RAD = 3.14159265359f / 180.0f;
	//move human, multiply by deltatime for timestep
	_position += _direction * _speed * deltaTime;
	//randomly change direction every 20 frames
	if (_frames == 20)
	{
		_direction = glm::rotate(_direction, randRotate(randomEngine) * DEG_TO_RAD); //convert movement to degrees
		_frames = 0;
	}
	else
	{
		_frames++;
	}
	

	//handle collisions during update
	if (collideWithLevel(levelData))
	{
		_direction = glm::rotate(_direction, randRotate(randomEngine) * DEG_TO_RAD); //convert movement to degrees
	}

}

void Human::draw(GameEngine::SpriteBatch& _spriteBatch)
{
	static int textureID = GameEngine::ResourceManager::getTexture("Textures/civilian.png").id;
	const glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);


	glm::vec4 destRect;
	destRect.x = _position.x;
	destRect.y = _position.y;
	destRect.z = AGENT_WIDTH;
	destRect.w = AGENT_WIDTH;
	_spriteBatch.draw(destRect, uvRect, textureID, 0, _color);
}
