#include "Gun.h"
#include <random>
#include <ctime>
#include <glm/gtx/rotate_vector.hpp>


Gun::Gun(std::string name, int fireRate, int bulletsPerShot, float spread, float bulletDamage, float bulletSpeed) :
	_name(name),
	_fireRate(fireRate),
	_bulletsPerShot(bulletsPerShot),
	_spread(spread),
	_bulletDamage(bulletDamage),
	_bulletSpeed(bulletSpeed),
	_frameCounter(0)
{
}


Gun::~Gun()
{
}

void Gun::update(bool isMouseDown, const glm::vec2& position, const glm::vec2& direction, std::vector<Bullet>& bullets)
{
	//increase frame counter each update call
	_frameCounter++;
	//if fire rate number of frames has passed
	if (_frameCounter >= _fireRate && isMouseDown)
	{
		//fire a bullet
		fire(position, direction, bullets);
		//return frame counter to 0
		_frameCounter = 0;
	}
}

void Gun::fire(const glm::vec2& position, const glm::vec2& direction, std::vector<Bullet>& bullets)
{
	//set up random engine
	//create a static random engine and seed with time
	static std::mt19937 randomEngine(time(nullptr));
	//create a distribution between -spread and +spread
	static std::uniform_real_distribution<float> randRotate(-_spread, _spread);
	const float DEG_TO_RAD = 3.14159265359f / 180.0f;


	//loop through number of bullets to fire per shot
	for (int i = 0; i < _bulletsPerShot; i++)
	{
		//create a new bullet and add to bullets vector
		bullets.emplace_back(position,glm::rotate(direction,randRotate(randomEngine)*DEG_TO_RAD), _bulletDamage, _bulletSpeed);
	}
}
