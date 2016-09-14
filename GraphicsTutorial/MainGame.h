#pragma once

#include <SDL/SDL.h>
#include <Gl/glew.h>

#include <GameEngine/Sprite.h>
#include <GameEngine/GLSLProgram.h>
#include <GameEngine/GLTexture.h>
#include <GameEngine/Window.h>
#include <GameEngine/Camera2D.h>
#include <GameEngine/InputManager.h>
#include <GameEngine/Timing.h>

#include <GameEngine/SpriteBatch.h>
#include "Bullet.h"

#include <vector>

enum class GameState {PLAY, EXIT};

class MainGame
{
public:
	MainGame();
	~MainGame();

	void run();
	

private:
	void initSystems();
	void initShaders();
	void gameLoop();
	void processInput();
	void drawGame();

	GameEngine::Window _window;
	int _screenWidth;
	int _screenHeight;

	GameState _gameState;
	

	GameEngine::GLSLProgram _colorProgram;
	GameEngine::Camera2D _camera;

	GameEngine::SpriteBatch _spriteBatch;

	GameEngine::InputManager _inputManager;
	GameEngine::FPSLimiter _fpsLimiter;
	//keeps track of all bullets in game
	std::vector<Bullet> _bullets;

	float _fps;
	float _maxFPS;
	float _time;
	

};

