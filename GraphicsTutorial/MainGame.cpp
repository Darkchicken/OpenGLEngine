#include "MainGame.h"
#include "Sprite.h"
#include "Errors.h"
#include <iostream>
#include <string>




MainGame::MainGame():
	_time(0.0f),
	_window(nullptr),
	_screenWidth(1024),
	_screenHeight(768),
	_gameState(GameState::PLAY)
{
	
	
}


MainGame::~MainGame()
{
}

void MainGame::run()
{
	initSystems();

	_sprites.push_back(new Sprite());
	_sprites.back()->init(-1.0f, -1.0f, 1.0f, 1.0f, "Textures/jimmyJump_pack/PNG/CharacterRight_Standing.png");
	
	_sprites.push_back(new Sprite());
	_sprites.back()->init(0.0f, -1.0f, 1.0f, 1.0f, "Textures/jimmyJump_pack/PNG/CharacterRight_Standing.png");

	_sprites.push_back(new Sprite());
	_sprites.back()->init(-1.0f, 0.0f, 1.0f, 1.0f, "Textures/jimmyJump_pack/PNG/CharacterRight_Standing.png");
	//_playerTexture = ImageLoader::LoadPNG("Textures/jimmyJump_pack/PNG/CharacterRight_Standing.png");
	gameLoop();
}
void MainGame::initSystems()
{
	//initialize SDL
	SDL_Init(SDL_INIT_EVERYTHING);
	//Creates Window
	_window = SDL_CreateWindow("Game Engine",SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,_screenWidth, _screenHeight,SDL_WINDOW_OPENGL);
	if (_window == nullptr)
	{
		fatalError("SDL window could not be opened!");
	}

	SDL_GLContext glContext = SDL_GL_CreateContext(_window);
	if (glContext == nullptr)
	{
		fatalError("SDL GL Context could not be created!");
	}
	//set up glew
	GLenum error = glewInit();

	if (error != GLEW_OK)
	{
		fatalError("Could not initialize GLEW!");
	}
	//makes it so you have 2 windows, one you draw to while you clear the other
	//prevents flickering
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER,1);

	//set background color
	glClearColor(0.0f,0.0f,1.0f,1.0f);

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

		processInput();

		//increment time variable
		_time += 0.01f;

		drawGame();

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
				_gameState = GameState::EXIT;
				break;
			case SDL_MOUSEMOTION:
				//std::cout << evnt.motion.x << " " << evnt.motion.y << std::endl;
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

	GLuint timeLocation = _colorProgram.getUniformLocation("time");

	glUniform1f(timeLocation, _time);
	for (int i = 0; i < _sprites.size(); i++)
	{
		_sprites[i]->draw();
	}
	
	//unbind texture
	glBindTexture(GL_TEXTURE_2D,0);

	_colorProgram.unuse();

	SDL_GL_SwapWindow(_window);
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
	frameTimes[currentFrame % NUM_SAMPLES];

	int count;

	if (currentFrame < NUM_SAMPLES)
	{
		count = currentFrame;
	}
	else
	{
		count = NUM_SAMPLES;
	}

	float frameTimeAverage = 0;


}


