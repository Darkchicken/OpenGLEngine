#include "Bullet.h"
#include <GameEngine/ResourceManager.h>


Bullet::Bullet(glm::vec2 position, glm::vec2 direction, float speed, int lifetime)
{
	_lifetime = lifetime;
	_position = position;
	_direction = direction;
	_speed = speed;
}


Bullet::~Bullet()
{
}


void Bullet::draw(GameEngine::SpriteBatch& spriteBatch)
{
	glm::vec4 uv(0.0f, 0.0f, 1.0f, 1.0f);
	static GameEngine::GLTexture texture = GameEngine::ResourceManager::getTexture("Textures/jimmyJump_pack/PNG/CharacterRight_Standing.png");
	GameEngine::Color color;
	color.r = 255;
	color.g = 255;
	color.b = 255;
	color.a = 255;

	glm::vec4 posAndSize= glm::vec4 (_position.x, _position.y, 30,30);

	spriteBatch.draw(posAndSize, uv, texture.id, 0.0f, color);
}

bool Bullet::update()
{
	//direction is unit length, speed changes position
	_position += _direction * _speed;
	//decrement lifetime
	_lifetime--;
	if (_lifetime == 0)
	{
		return true;
	}
	return false;
}
