#include "MainGame.h"
#include <SDL/SDL.h>
#include <GameEngine/GameEngine.h>
#include <GameEngine/Timing.h>
#include <iostream>
#include <random>
#include <ctime>
#include "Zombie.h"

//const float for player default speed
const float PLAYER_SPEED = 5.0f;
//const float for human default speed
const float HUMAN_SPEED = 1.0f;
//const float for zombie default speed
const float ZOMBIE_SPEED = 1.3f;

MainGame::MainGame() : 
_screenWidth(1024),
_screenHeight(768),
_gameState(GameState::PLAY),
_fps(0.0f),
_player(nullptr)
{
	

}


MainGame::~MainGame()
{
	//remove levels from memory to prevent memory leaks
	//delete each level in _levels vector
	for (int i = 0; i < _levels.size(); i++)
	{
		delete _levels[i];
	}
}

void MainGame::run()
{
	//sets up window, shaders, camera, etc.
	initSystems();
	//loads level
	initLevel();

	//begin game loop
	gameLoop();
	

}
void MainGame::initSystems()
{
	//initialize GameEngine
	GameEngine::init();

	//create window (window name, window width, window height, flags)
	_window.create("Zombie Outbreak",_screenWidth, _screenHeight, 0);

	//set background color
	glClearColor(0.7f, 0.7f, 0.7f, 1.0f);

	//load shaders and texture shading
	initShaders();

	//initialize the agent sprite batch
	_agentSpriteBatch.init();

	//initialize camera
	_camera.init(_screenWidth, _screenHeight);

	
}

void MainGame::initLevel()
{
	//initialize level (be sure to delete in destructor to free memory)
	//Level 1
	_levels.push_back(new Level("Levels/level1.txt"));
	_currentLevel = 0;

	//set up player
	_player = new Player();
	//set variables (speed, position, inputmanager)
	_player->init(PLAYER_SPEED, _levels[_currentLevel] ->getStartPlayerPos(), &_inputManager);
	//add player to humans vector
	_humans.push_back(_player);


	//create a static random engine
	static std::mt19937 randomEngine;
	//seed the random engine with time
	randomEngine.seed(time(nullptr));
	//create a distribution that random numbers can fall between
	static std::uniform_int_distribution<int> randX(2, _levels[_currentLevel]->getWidth()-2);
	static std::uniform_int_distribution<int> randY(2, _levels[_currentLevel]->getHeight()-2);

	
	
	//add all the random humans
	for (int i = 0; i < _levels[_currentLevel]->getNumHumans(); i++)
	{
		//push a new human object onto the _humans vector
		_humans.push_back(new Human);
		//create a new random position vector
		glm::vec2 pos(randX(randomEngine) *TILE_WIDTH, randY(randomEngine)*TILE_WIDTH);
		//go to the newly created human and set its speed and position
		_humans.back()->init(HUMAN_SPEED, pos);

	}

	//add all the zombies
	const std::vector<glm::vec2>& zombiePositions = _levels[_currentLevel]->getStartZombiePos();
	for (int i = 0; i < zombiePositions.size(); i++)
	{
		//push a new zombie object onto the _zombie vector
		_zombies.push_back(new Zombie);
		
		//go to the newly created human and set its speed and position
		_zombies.back()->init(ZOMBIE_SPEED, zombiePositions[i]);

	}
	
}

void MainGame::initShaders()
{
	//compile all contained shaders
	_textureProgram.compileShaders("Shaders/colorShading.vert", "Shaders/colorShading.frag");
	//add all attributes contained on the vertex shader
	_textureProgram.addAttribute("vertexPosition");
	_textureProgram.addAttribute("vertexColor");
	_textureProgram.addAttribute("vertexUV");
	_textureProgram.linkShaders();
}

void MainGame::gameLoop()
{
	//limit screen fps
	GameEngine::FPSLimiter fpsLimiter;
	fpsLimiter.setMaxFPS(60.0f);

	//main game loop to run game
	while (_gameState == GameState::PLAY)
	{
		//start fps limiter
		fpsLimiter.begin();


		//process player input
		processInput();

		//update all agents
		updateAgents();

		//camera follows player
		_camera.setPosition(_player->getPosition());


		//update camera
		_camera.update();

		//draw game 
		drawGame();

		//end fps limiter and store fps value
		_fps = fpsLimiter.end();

	}
}

void MainGame::updateAgents()
{
	//update all humans
	for (int i = 0; i < _humans.size(); i++)
	{
		
		_humans[i]-> update(_levels[_currentLevel]->getLevelData(),_humans,_zombies);
	}

	//update all zombies
	for (int i = 0; i < _zombies.size(); i++)
	{

		_zombies[i]->update(_levels[_currentLevel]->getLevelData(), _humans, _zombies);
	}

	//update zombie collisions
	for (int i = 0; i < _zombies.size(); i++)
	{
		//collide with other zombies
		for (int j = i + 1; j < _zombies.size(); j++)
		{

			_zombies[i]->collideWithAgent(_zombies[j]);
		}
		//collide with humans
		for (int j = i + 1; j < _humans.size(); j++)
		{
			//if a zombie collides with a human
			if (_zombies[i]->collideWithAgent(_humans[j]))
			{
				//turn human into a zombie

				//push back a new zombie in _zombies vector
				_zombies.push_back(new Zombie);
				//initialize the new zombie and add it
				_zombies.back()->init(ZOMBIE_SPEED, _humans[j]->getPosition());

				//delete the human
				delete _humans[j];
				//back up the humans vector by one
				_humans[j] = _humans.back();
				_humans.pop_back();
			}
			
		}
		
	}

	//update human collisions
	for (int i = 0; i < _humans.size(); i++)
	{
		//collide with other humans
		for (int j = i+1; j < _humans.size(); j++)
		{

			_humans[i]->collideWithAgent(_humans[j]);
		}
		//_humans[i]->update(_levels[_currentLevel]->getLevelData(), _humans, _zombies);
	}

	

	

}

void MainGame::processInput()
{
	SDL_Event evnt;

	//asks operating system what events need to be processed
	//returns 1 if pending event, 0 if none available
	while (SDL_PollEvent(&evnt))
	{
		//we have an event
		switch (evnt.type)
		{
		case SDL_QUIT:
			//exit game
			break;
		case SDL_MOUSEMOTION:
			_inputManager.setMouseCoords(evnt.motion.x, evnt.motion.y);
			break;
		case SDL_KEYDOWN:
			_inputManager.pressKey(evnt.key.keysym.sym);
			break;
		case SDL_KEYUP:
			_inputManager.releaseKey(evnt.key.keysym.sym);
			break;
		case SDL_MOUSEBUTTONDOWN:
			_inputManager.pressKey(evnt.button.button);
			break;
		case SDL_MOUSEBUTTONUP:
			_inputManager.releaseKey(evnt.button.button);
			break;
		}
	}


}
void MainGame::drawGame()
{


	//tells open gl what depth to clear to
	glClearDepth(1.0);

	//clear screen 
	// | is bitwise or, combines bit
	//same as calling glclear twice, once on each bit
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	_textureProgram.use();


	//draw game here
	glActiveTexture(GL_TEXTURE0);
	//make sure shader uses texture 0
	GLint textureUniform = _textureProgram.getUniformLocation("mySampler");
	glUniform1i(textureUniform,0);

	//grab the camera matrix
	glm::mat4 projectionMatrix = _camera.getCameraMatrix();
	GLint pUniform = _textureProgram.getUniformLocation("P");
	glUniformMatrix4fv(pUniform, 1, GL_FALSE, &projectionMatrix[0][0]);

	//draw level
	_levels[_currentLevel]-> draw();

	//begin drawing agents
	_agentSpriteBatch.begin();

	//draw all humans including player
	for (int i = 0; i < _humans.size(); i++)
	{
		_humans[i]->draw(_agentSpriteBatch);
	}

	//draw all zombies 
	for (int i = 0; i < _zombies.size(); i++)
	{
		_zombies[i]->draw(_agentSpriteBatch);
	}

	//end drawing agents
	_agentSpriteBatch.end();

	//render all agents
	_agentSpriteBatch.renderBatch();

	_textureProgram.unuse();

	//swap buffer and draw everything to screen
	_window.swapBuffer();
}

