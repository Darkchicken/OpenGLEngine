#include "MainGame.h"


#include <GameEngine/Sprite.h>
#include <GameEngine/Errors.h>
#include <GameEngine/GameEngine.h>
#include <GameEngine/ResourceManager.h>

#include <iostream>
#include <string>




MainGame::MainGame() :
	_time(0.0f),
	_screenWidth(1024),
	_screenHeight(768),
	_gameState(GameState::PLAY),
	_maxFPS(60.0f)
{
	_camera.init(_screenWidth, _screenHeight);
	

}


MainGame::~MainGame()
{
}

void MainGame::run()
{
	initSystems();
	
	//_playerTexture = ImageLoader::LoadPNG("Textures/jimmyJump_pack/PNG/CharacterRight_Standing.png");
	gameLoop();
}
void MainGame::initSystems()
{
	GameEngine::init();

	//create game window
	_window.create("Game Engine", _screenWidth, _screenHeight, GameEngine::FULLSCREEN);

	initShaders();

	_spriteBatch.init();

	_fpsLimiter.init(_maxFPS);
}

void MainGame::initShaders()
{
	//compile all contained shaders
	_colorProgram.compileShaders("Shaders/colorShading.vert", "Shaders/colorShading.frag");
	//add all attributes contained on the vertex shader
	_colorProgram.addAttribute("vertexPosition");
	_colorProgram.addAttribute("vertexColor");
	_colorProgram.addAttribute("vertexUV");
	_colorProgram.linkShaders();
}

void MainGame::gameLoop()
{
	while (_gameState != GameState::EXIT)
	{
		//use for frame time measuring
		_fpsLimiter.begin();

		processInput();

		//increment time variable
		_time += 0.01f;

		//update camera
		_camera.update();

		for (int i = 0; i < _bullets.size();)
		{
			if (_bullets[i].update() == true)
			{
				_bullets[i] = _bullets.back();
				_bullets.pop_back();
			}
			else
			{
				//only increment i if a bullet wasnt deleted
				i++;
			}
		}

		//draw game on screen
		drawGame();

		_fps = _fpsLimiter.end();

		//print only once every 10 frames
		static int frameCounter = 0;
		frameCounter++;
		if (frameCounter == 10000)
		{
			std::cout << _fps << std::endl;
			frameCounter = 0;
		}
		

	}
}
void MainGame::processInput()
{
	SDL_Event evnt;

	const float CAMERA_SPEED = 2.0f;
	const float SCALE_SPEED = 0.1f;

	//asks operating system what events need to be processed
	//returns 1 if pending event, 0 if none available
	while (SDL_PollEvent(&evnt))
	{
		//we have an event
		switch (evnt.type)
		{
			case SDL_QUIT:
				_gameState = GameState::EXIT;
				break;
			
			case SDL_KEYDOWN:
				_inputManager.pressKey(evnt.key.keysym.sym);
				break;
			case SDL_KEYUP:
				_inputManager.releaseKey(evnt.key.keysym.sym);
				break;
			case SDL_MOUSEMOTION:
				_inputManager.setMouseCoords(evnt.motion.x, evnt.motion.y);
				break;
			case SDL_MOUSEBUTTONDOWN:
				_inputManager.pressKey(evnt.button.button);
				break;
			case SDL_MOUSEBUTTONUP:
				_inputManager.releaseKey(evnt.button.button);
				break;
			

		}



		if (_inputManager.isKeyPressed(SDLK_w))
		{
			_camera.setPosition(_camera.getPosition() + glm::vec2(0.0f, CAMERA_SPEED));
		}
		if (_inputManager.isKeyPressed(SDLK_s))
		{
			_camera.setPosition(_camera.getPosition() + glm::vec2(0.0f, -CAMERA_SPEED));
		}
		if (_inputManager.isKeyPressed(SDLK_a))
		{
			_camera.setPosition(_camera.getPosition() + glm::vec2(-CAMERA_SPEED, 0.0f));
		}
		if (_inputManager.isKeyPressed(SDLK_d))
		{
			_camera.setPosition(_camera.getPosition() + glm::vec2(CAMERA_SPEED, 0.0f));
		}
		if (_inputManager.isKeyPressed(SDLK_q))
		{
			_camera.setScale(_camera.getScale() + SCALE_SPEED);
		}
		if (_inputManager.isKeyPressed(SDLK_e))
		{
			_camera.setScale(_camera.getScale() - SCALE_SPEED);
		}
		//if left mouse button is pressed
		if (_inputManager.isKeyPressed(SDL_BUTTON_LEFT))
		{
			glm::vec2 mouseCoords = _inputManager.getMouseCoords();
			mouseCoords = _camera.convertScreenToWorldCoords(mouseCoords);
			//position of player
			glm::vec2 playerPos(0.0f);
			//direction from player to where we clicked
			glm::vec2 direction = mouseCoords-playerPos;
			//normalize direction
			direction = glm::normalize(direction);
			//create new bullet and add to _bullets vector
			_bullets.emplace_back(playerPos, direction, 5.0f, 1000);
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


	_colorProgram.use();

	glActiveTexture(GL_TEXTURE0);
	
	GLint textureLocation = _colorProgram.getUniformLocation("mySampler");
	//tells it we are using gl_texture0
	glUniform1i(textureLocation, 0);
	

	//set the camera matrix
	GLint pLocation = _colorProgram.getUniformLocation("P");
	glm::mat4 cameraMatrix = _camera.getCameraMatrix();

	glUniformMatrix4fv(pLocation, 1, GL_FALSE, &(cameraMatrix[0][0]));

	//call out sprite batch 
	_spriteBatch.begin();
	//x and y at 0, width and height at 50
	glm::vec4 pos(0,0,50,50);
	glm::vec4 uv(0.0f,0.0f,1.0f,1.0f);
	static GameEngine::GLTexture texture = GameEngine::ResourceManager::getTexture("Textures/jimmyJump_pack/PNG/CharacterRight_Standing.png");
	GameEngine::Color color;
	color.r = 255;
	color.g = 255;
	color.b = 255;
	color.a = 255;

	//draw sprite
	_spriteBatch.draw(pos,uv,texture.id, 0.0f,color);

	for (int i = 0; i < _bullets.size(); i++)
	{
		_bullets[i].draw(_spriteBatch);
	}

	_spriteBatch.end();

	_spriteBatch.renderBatch();
	
	//unbind texture
	glBindTexture(GL_TEXTURE_2D,0);

	_colorProgram.unuse();

	_window.swapBuffer();
}


