#pragma once

#include <SDL/SDL.h>
#include <Gl/glew.h>

#include <GameEngine/Sprite.h>
#include <GameEngine/GLSLProgram.h>
#include <GameEngine/GLTexture.h>
#include <GameEngine/Window.h>
#include <GameEngine/Camera2D.h>

#include <GameEngine/SpriteBatch.h>

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

	void calculateFPS();
	
	GameEngine::Window _window;
	int _screenWidth;
	int _screenHeight;

	GameState _gameState;
	

	GameEngine::GLSLProgram _colorProgram;
	GameEngine::Camera2D _camera;

	GameEngine::SpriteBatch _spriteBatch;

	float _fps;
	float _frameTime;
	

	float _time;
	float _maxFPS;

};

