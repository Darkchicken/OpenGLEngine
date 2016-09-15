#pragma once
#include<glm/glm.hpp>
#include <string>
#include <vector>

#include "Bullet.h"

class Gun
{
public:
	Gun(std::string name, int fireRate, int bulletsPerShot, float spread, float bulletDamage, float bulletSpeed);
	~Gun();


	void update(bool isMouseDown,const glm::vec2& position, const glm::vec2& direction, std::vector<Bullet>& bullets);
	




private:

	void fire(const glm::vec2& position, const glm::vec2& direction, std::vector<Bullet>& bullets);

	std::string _name; ///< gun name

	int _frameCounter;///<counts frames in update

	int _fireRate;///< fire rate in terms of frames

	int _bulletsPerShot;///<how many bullets are fired at a time

	float _spread; ///< how accurate the weapon is

	float _bulletSpeed;

	float _bulletDamage;




};

