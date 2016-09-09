#pragma once

#include <SDL/SDL.h>
#include <Gl/glew.h>

#include <GameEngine/Sprite.h>
#include <GameEngine/GLSLProgram.h>
#include <GameEngine/GLTexture.h>
#include <GameEngine/Window.h>

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
	//vector of sprite pointers
	std::vector<GameEngine::Sprite*> _sprites;
	

	GameEngine::GLSLProgram _colorProgram;

	float _fps;
	float _frameTime;
	

	float _time;
	float _maxFPS;

};

