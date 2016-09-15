#pragma once

#include <GameEngine/InputManager.h>

#include "Human.h"

//Player is a child of Human
class Player : public Human
{
public:
	Player();
	~Player();
	//initializes player	
	void init(float speed, glm::vec2 position, GameEngine::InputManager* inputManager);
	//updates player, handles movement
	void update(const std::vector<std::string>& levelData,
		std::vector<Human*>& humans,
		std::vector<Zombie*>& zombies);

private:
	GameEngine::InputManager* _inputManager;


	
};

