#pragma once

#include <GameEngine/InputManager.h>
#include <GameEngine/Camera2D.h>

#include "Human.h"
#include "Bullet.h"

//forward declaration
class Gun;


//Player is a child of Human
class Player : public Human
{
public:
	Player();
	~Player();
	//initializes player	
	void init(float speed, glm::vec2 position, GameEngine::InputManager* inputManager, GameEngine::Camera2D* camera, std::vector<Bullet>* bullets);

	void addGun(Gun* gun);

	//updates player, handles movement
	void update(const std::vector<std::string>& levelData,
		std::vector<Human*>& humans,
		std::vector<Zombie*>& zombies
		) override;

private:
	GameEngine::InputManager* _inputManager;
	GameEngine::Camera2D* _camera;

	std::vector<Gun*> _guns;
	std::vector<Bullet>* _bullets;

	int _currentGunIndex;


	
};

