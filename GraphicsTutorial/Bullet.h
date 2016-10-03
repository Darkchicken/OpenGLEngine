#pragma once

#include<glm/glm.hpp>
#include <GameEngine/SpriteBatch.h>

class Bullet
{
public:
	Bullet(glm::vec2 position, glm::vec2 direction, float speed, int lifetime);
	~Bullet();

	
	void draw(GameEngine::SpriteBatch& spriteBatch);

	//returns true when out of life (should be destroyed)
	bool update();

private:
	//number of frames bullet persists for
	int _lifetime;

	float _speed;
	glm::vec2 _direction;
	glm::vec2 _position;

};

