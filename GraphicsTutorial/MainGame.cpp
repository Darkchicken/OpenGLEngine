#include "MainGame.h"


#include <GameEngine/Sprite.h>
#include <GameEngine/Errors.h>
#include <GameEngine/GameEngine.h>

#include <iostream>
#include <string>




MainGame::MainGame() :
	_time(0.0f),
	_screenWidth(1024),
	_screenHeight(768),
	_gameState(GameState::PLAY),
	_maxFPS(60)
{
	//initialize camera
	_camera.init(_screenWidth, _screenHeight);
}


MainGame::~MainGame()
{
}

void MainGame::run()
{
	initSystems();

	_sprites.push_back(new GameEngine::Sprite());
	_sprites.back()->init(0.0f, 0.0f, _screenWidth/2, _screenWidth / 2, "Textures/jimmyJump_pack/PNG/CharacterRight_Standing.png");
	
	_sprites.push_back(new GameEngine::Sprite());
	_sprites.back()->init(_screenWidth / 2, 0.0f, _screenWidth / 2, _screenWidth / 2, "Textures/jimmyJump_pack/PNG/CharacterRight_Standing.png");

	
	//_playerTexture = ImageLoader::LoadPNG("Textures/jimmyJump_pack/PNG/CharacterRight_Standing.png");
	gameLoop();
}
void MainGame::initSystems()
{
	GameEngine::init();

	//create game window
	_window.create("Game Engine", _screenWidth, _screenHeight, GameEngine::FULLSCREEN);

	initShaders();
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
		float startTicks = SDL_GetTicks();

		processInput();

		//increment time variable
		_time += 0.01f;
		//update camera
		_camera.update();

		drawGame();
		//calculate frames per second
		calculateFPS();
		//print only once every 10 frames
		static int frameCounter = 0;
		frameCounter++;
		if (frameCounter == 10)
		{
			std::cout << _fps << std::endl;
			frameCounter = 0;
		}

		//time it took to run code in game loop
		float frameTicks = SDL_GetTicks() - startTicks;

		//limit FPS to max fps
		// ms/s divided by f/s = ms/f
		if (1000.0f / _maxFPS > frameTicks)
		{
			SDL_Delay(1000.0f / _maxFPS - frameTicks);
		}
		

	}
}
void MainGame::processInput()
{
	SDL_Event evnt;

	const float CAMERA_SPEED = 20.0f;
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
			case SDL_MOUSEMOTION:
				//std::cout << evnt.motion.x << " " << evnt.motion.y << std::endl;
				break;
			case SDL_KEYDOWN:
				switch (evnt.key.keysym.sym)
				{
					case SDLK_w:
						_camera.setPosition(_camera.getPosition() + glm::vec2(0.0, CAMERA_SPEED));
						break;
					case SDLK_s:
						_camera.setPosition(_camera.getPosition() + glm::vec2(0.0, -CAMERA_SPEED));
						break;
					case SDLK_a:
						_camera.setPosition(_camera.getPosition() + glm::vec2(CAMERA_SPEED, 0.0));
						break;
					case SDLK_d:
						_camera.setPosition(_camera.getPosition() + glm::vec2(-CAMERA_SPEED, 0.0));
						break;
					case SDLK_q://zoom out
						_camera.setScale(_camera.getScale() + SCALE_SPEED);
						break;
					case SDLK_e://zoom in
						_camera.setScale(_camera.getScale() - SCALE_SPEED);
						break;


				}
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


	_colorProgram.use();

	glActiveTexture(GL_TEXTURE0);
	
	GLint textureLocation = _colorProgram.getUniformLocation("mySampler");
	//tells it we are using gl_texture0
	glUniform1i(textureLocation, 0);
	


	GLint timeLocation = _colorProgram.getUniformLocation("time");
	glUniform1i(timeLocation, _time);


	//set the camera matrix
	GLint pLocation = _colorProgram.getUniformLocation("P");
	glm::mat4 cameraMatrix = _camera.getCameraMatrix();

	//upload matrix to gpu
	glUniformMatrix4fv(pLocation, 1, GL_FALSE, &(cameraMatrix[0][0]));

	for (int i = 0; i < _sprites.size(); i++)
	{
		_sprites[i]->draw();
	}
	
	//unbind texture
	glBindTexture(GL_TEXTURE_2D,0);

	_colorProgram.unuse();

	_window.swapBuffer();
}

//calculates the frames per second
void MainGame::calculateFPS()
{
	//number of frames to average
	static const int NUM_SAMPLES = 10;
	//constant num samples because you get an error without it
	static float frameTimes[NUM_SAMPLES];
	//where are you in frametimes buffer
	static int currentFrame = 0;

	//get current ticks, previous ticks
	static float previousTicks = SDL_GetTicks();
	float currentTicks;
	currentTicks = SDL_GetTicks();

	_frameTime = currentTicks - previousTicks;
	frameTimes[currentFrame % NUM_SAMPLES] = _frameTime;

	//set prev ticks to current ticks for next iteration
	previousTicks = currentTicks;

	int count;

	//increment current frame
	currentFrame++;


	if (currentFrame < NUM_SAMPLES)
	{
		count = currentFrame;
	}
	else
	{
		count = NUM_SAMPLES;
	}
	//on each call to CalculateFPS, set frameTimeAverage to 0...
	float frameTimeAverage = 0;
	//then add all frame times together...
	for (int i = 0; i < count; i++)
	{
		frameTimeAverage += frameTimes[i];
	}
	//and average them
	frameTimeAverage /= count;

	//then calculate frames per second
	if (frameTimeAverage > 0)
	{
		//convert frame time to f/s
		_fps = 1000.0f / frameTimeAverage;
	}
	else
	{
		_fps = 60.0f;
	}

	

}


