#include "Player.h"
#include <SDL/sdl.h>

#include "Gun.h"
#include "Bullet.h"

Player::Player():
	_currentGunIndex(-1)
{
	//empty
}


Player::~Player()
{
	//empty
}

void Player::init(float speed, glm::vec2 position, GameEngine::InputManager* inputManager, GameEngine::Camera2D* camera, std::vector<Bullet>* bullets)
{
	_speed = speed;
	_position = position;
	_inputManager = inputManager;
	_camera = camera;
	_bullets = bullets;
	//set player color to dark blue
	_color.r = 0;
	_color.g = 0;
	_color.b = 185;
	_color.a = 255;

}

void Player::addGun(Gun* gun)
{
	//add gun to inventory
	_guns.push_back(gun);

	//if no gun equipped, equip this gun
	if (_currentGunIndex == -1)
	{
		_currentGunIndex = 0;
	}

}


void Player::update(const std::vector<std::string>& levelData,
	std::vector<Human*>& humans,
	std::vector<Zombie*>& zombies)
{
	if (_inputManager->isKeyPressed(SDLK_w))
	{
		_position.y += _speed;
	}
	else if (_inputManager->isKeyPressed(SDLK_s))
	{
		_position.y -= _speed;
	}

	if (_inputManager->isKeyPressed(SDLK_a))
	{
		_position.x -= _speed;
	}
	else if (_inputManager->isKeyPressed(SDLK_d))
	{
		_position.x += _speed;
	}
	//switch guns
	if (_inputManager->isKeyPressed(SDLK_1) && _guns.size() >= 0)
	{
		_currentGunIndex = 0;
	}
	else if (_inputManager->isKeyPressed(SDLK_2) && _guns.size() >= 0)
	{
		_currentGunIndex = 1;
	}
	else if (_inputManager->isKeyPressed(SDLK_3) && _guns.size() >= 0)
	{
		_currentGunIndex = 2;
	}

	//update player's gun
	if (_currentGunIndex != -1)
	{
		//mouse coordinates
		glm::vec2 mouseCoords = _inputManager->getMouseCoords();
		//convert mouse coordinates to world coordinates
		mouseCoords = _camera->convertScreenToWorldCoords(mouseCoords);
		//get center of player agent
		glm::vec2 centerPosition = _position + glm::vec2 (AGENT_RADIUS);
		//direction to fire
		glm::vec2 direction = glm::normalize(mouseCoords - centerPosition);


		_guns[_currentGunIndex]->update(_inputManager->isKeyPressed(SDL_BUTTON_LEFT),
			centerPosition,
			direction,
			*_bullets);
	}

	//test collisions
	collideWithLevel(levelData);
}



