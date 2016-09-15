#pragma once

#include <GameEngine/GLSLProgram.h>
#include <GameEngine/Window.h>
#include <GameEngine/Camera2D.h>
#include <GameEngine/InputManager.h>
#include <GameEngine/SpriteBatch.h>

#include <vector>

#include "Player.h"
#include "Level.h"

class Zombie;

enum class GameState {PLAY, EXIT};


class MainGame
{
public:
	MainGame();
	~MainGame();

	//function to run game
	void run();


private:
	//initialize core systems of game
	void initSystems();

	//initializes the level and sets up everything
	void initLevel();

	//initialize game shaders
	void initShaders();

	//Main game loop to run game
	void gameLoop();

	//update all agents
	void updateAgents();

	//handles player input
	void processInput();

	//draws game to screen
	void drawGame();

	//Member variables
	GameEngine::Window _window; //creates window for game
	GameEngine::GLSLProgram _textureProgram; //handles shaders
	GameEngine::InputManager _inputManager; //handles input
	GameEngine::Camera2D _camera; //handles main camera
	GameEngine::SpriteBatch _agentSpriteBatch; ///<draws all agents

	//vector of all levels in game (pointers to level object)
	std::vector<Level*> _levels;
	//vector of all Humans in game (pointers to human object)
	std::vector < Human*> _humans;
	//vector of all Zombies in game (pointers to zombie object)
	std::vector < Zombie*> _zombies;

	Player* _player;

	int _screenWidth, _screenHeight;
	float _fps;
	int _currentLevel;

	GameState _gameState;
	


};
