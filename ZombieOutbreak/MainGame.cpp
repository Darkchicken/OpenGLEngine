#include "MainGame.h"
#include <SDL/SDL.h>
#include <GameEngine/GameEngine.h>
#include <GameEngine/Timing.h>
#include <GameEngine/EngineErrors.h>
#include <GameEngine/ResourceManager.h>
#include <iostream>
#include <random>
#include <glm/gtx/rotate_vector.hpp>
#include <ctime>
#include <algorithm>
#include "Zombie.h"
#include "Gun.h"

//const float for player default speed
const float PLAYER_SPEED = 5.0f;
//const float for human default speed
const float HUMAN_SPEED = 1.0f;
//const float for zombie default speed
const float ZOMBIE_SPEED = 1.3f;

MainGame::MainGame() : 
m_screenWidth(1024),
m_screenHeight(768),
m_gameState(GameState::PLAY),
m_fps(0.0f),
m_player(nullptr),
m_numHumansKilled(0),
m_numZombiesKilled(0)
{
	//empty

}


MainGame::~MainGame()
{
	//remove levels from memory to prevent memory leaks
	//delete each level in _levels vector
	for (int i = 0; i < m_levels.size(); i++)
	{
		delete m_levels[i];
	}

	//delete all humans and zombies
	for (int i = 0; i < m_humans.size(); i++)
	{
		delete m_humans[i];
	}
	for (int i = 0; i < m_zombies.size(); i++)
	{
		delete m_zombies[i];
	}
}

void MainGame::run()
{
	//sets up window, shaders, camera, etc.
	initSystems();
	//loads level
	initLevel();
	//load level music
	GameEngine::Music music = m_audioEngine.loadMusic("Sounds/XYZ.ogg");
	//pass looping for music (-1 loop forever)
	music.play(-1);
	//begin game loop
	gameLoop();
	

}
void MainGame::initSystems()
{
	//initialize GameEngine
	GameEngine::init();

	//initialize sound, must happen after GameEngine.init
	m_audioEngine.init();

	//create window (window name, window width, window height, flags)
	m_window.create("Zombie Outbreak",m_screenWidth, m_screenHeight, 0);

	//set background color
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

	//load shaders and texture shading
	initShaders();

	//initialize the agent sprite batch
	m_agentSpriteBatch.init();
	//initialize the hud sprite batch
	m_hudSpriteBatch.init();

	//initialize sprite font
	m_spriteFont = new GameEngine::SpriteFont("Fonts/chintzy.ttf", 64); //< font path and 64 point render size

	//initialize camera
	m_camera.init(m_screenWidth, m_screenHeight);

	//initialize HUD camera
	m_hudCamera.init(m_screenWidth, m_screenHeight);
	m_hudCamera.setPosition(glm::vec2(m_screenWidth/2, m_screenHeight/2));

	//initialize particles

	m_bloodParticleBatch = new GameEngine::ParticleBatch2D();
	m_bloodParticleBatch->init(1000,0.05f,GameEngine::ResourceManager::getTexture("Textures/Particle.png"));
	m_particleEngine.addParticleBatch(m_bloodParticleBatch);


	
}

void MainGame::initLevel()
{
	//initialize level (be sure to delete in destructor to free memory)
	//Level 1
	m_levels.push_back(new Level("Levels/level1.txt"));
	m_currentLevel = 0;

	//set up player
	m_player = new Player();
	//set variables (speed, position, inputmanager)
	m_player->init(PLAYER_SPEED, m_levels[m_currentLevel] ->getStartPlayerPos(), &m_inputManager,&m_camera, &m_bullets);
	//add player to humans vector
	m_humans.push_back(m_player);


	//create a static random engine
	static std::mt19937 randomEngine;
	//seed the random engine with time
	randomEngine.seed(time(nullptr));
	//create a distribution that random numbers can fall between
	static std::uniform_int_distribution<int> randX(2, m_levels[m_currentLevel]->getWidth()-2);
	static std::uniform_int_distribution<int> randY(2, m_levels[m_currentLevel]->getHeight()-2);

	
	
	//add all the random humans
	for (int i = 0; i < m_levels[m_currentLevel]->getNumHumans(); i++)
	{
		//push a new human object onto the _humans vector
		m_humans.push_back(new Human);
		//create a new random position vector
		glm::vec2 pos(randX(randomEngine) *TILE_WIDTH, randY(randomEngine)*TILE_WIDTH);
		//go to the newly created human and set its speed and position
		m_humans.back()->init(HUMAN_SPEED, pos);

	}

	//add all the zombies
	const std::vector<glm::vec2>& zombiePositions = m_levels[m_currentLevel]->getStartZombiePos();
	for (int i = 0; i < zombiePositions.size(); i++)
	{
		//push a new zombie object onto the _zombie vector
		m_zombies.push_back(new Zombie);
		
		//go to the newly created human and set its speed and position
		m_zombies.back()->init(ZOMBIE_SPEED, zombiePositions[i]);

	}

	//set up player's guns
	const float BULLET_SPEED = 20.0f;
	m_player->addGun(new Gun("Magnum",10.0f, 1, 5.0f, 30.0f, BULLET_SPEED,m_audioEngine.loadSoundEffect("Sounds/pistol.wav")));
	m_player->addGun(new Gun("Shotgun", 30.0f, 12, 20.0f, 4.0f, BULLET_SPEED, m_audioEngine.loadSoundEffect("Sounds/shotgun.wav")));
	m_player->addGun(new Gun("MP5", 2.0f, 1, 10.0f, 20.0f, BULLET_SPEED, m_audioEngine.loadSoundEffect("Sounds/cg1.wav")));
	
}

void MainGame::initShaders()
{
	//compile all contained shaders
	m_textureProgram.compileShaders("Shaders/colorShading.vert", "Shaders/colorShading.frag");
	//add all attributes contained on the vertex shader
	m_textureProgram.addAttribute("vertexPosition");
	m_textureProgram.addAttribute("vertexColor");
	m_textureProgram.addAttribute("vertexUV");
	m_textureProgram.linkShaders();
}

void MainGame::gameLoop()
{

	const float DESIRED_FPS = 60.0f;
	//dont allow the game to go longer than 6 steps
	const int MAX_PHYSICS_STEPS = 6;

	//limit screen fps
	GameEngine::FPSLimiter fpsLimiter;
	fpsLimiter.setMaxFPS(DESIRED_FPS);

	//zoom out by 4 times
	const float CAMERA_SCALE = 1.0f / 4.0f;
	m_camera.setScale(CAMERA_SCALE);

	//calculate exact frametime
	const float MS_PER_SECOND = 1000.0f;
	const float DESIRED_FRAMETIME = MS_PER_SECOND / DESIRED_FPS;
	const float MAX_DELTA_TIME = 1.0f;
	//ticks of previous frame
	float previousTicks = SDL_GetTicks();

	//main game loop to run game
	while (m_gameState == GameState::PLAY)
	{
		
		//start fps limiter
		fpsLimiter.begin();

		//get frametime
		float newTicks = SDL_GetTicks();
		float frameTime = newTicks - previousTicks;
		previousTicks = newTicks;
		float totalDeltaTime = frameTime / DESIRED_FRAMETIME;

		//check if the player has won
		checkVictory();

		//update input manager
		m_inputManager.update();

		//process player input
		processInput();

		int i = 0;
		while (totalDeltaTime > 0.0f && i < MAX_PHYSICS_STEPS)
		{
			float deltaTime = std::min(totalDeltaTime, MAX_DELTA_TIME);

			//update all agents
			updateAgents(deltaTime);

			//update bullets
			updateBullets(deltaTime);

			//update particles
			m_particleEngine.update(deltaTime);

			totalDeltaTime -= deltaTime;
			i++;
		}
		
	

		//camera follows player
		m_camera.setPosition(m_player->getPosition());
		//update camera
		m_camera.update();

		//update hud camera
		m_hudCamera.update();

		//draw game 
		drawGame();

		//end fps limiter and store fps value
		m_fps = fpsLimiter.end();

		std::cout << m_fps << std::endl;

	}
}

void MainGame::updateAgents(float deltaTime)
{
	//update all humans
	for (int i = 0; i < m_humans.size(); i++)
	{
		
		m_humans[i]-> update(m_levels[m_currentLevel]->getLevelData(),
							m_humans,
							m_zombies, 
							deltaTime);
	}

	//update all zombies
	for (int i = 0; i < m_zombies.size(); i++)
	{

		m_zombies[i]->update(m_levels[m_currentLevel]->getLevelData(), 
							m_humans, 
							m_zombies, 
							deltaTime);
	}

	//update zombie collisions
	for (int i = 0; i < m_zombies.size(); i++)
	{
		//collide with other zombies
		for (int j = i + 1; j < m_zombies.size(); j++)
		{

			m_zombies[i]->collideWithAgent(m_zombies[j]);
		}
		//collide with humans (start at 1 to skip player human)
		for (int j = 1; j < m_humans.size(); j++)
		{
			//if a zombie collides with a human
			if (m_zombies[i]->collideWithAgent(m_humans[j]))
			{
				//turn human into a zombie

				//push back a new zombie in _zombies vector
				m_zombies.push_back(new Zombie);
				//initialize the new zombie and add it
				m_zombies.back()->init(ZOMBIE_SPEED, m_humans[j]->getPosition());

				//delete the human
				delete m_humans[j];
				//back up the humans vector by one
				m_humans[j] = m_humans.back();
				m_humans.pop_back();
			}			
		}

		//collide with player
		if (m_zombies[i]->collideWithAgent(m_player))
		{
			GameEngine::fatalError("YOU LOSE!");
		}
		
	}

	//update human collisions
	for (int i = 0; i < m_humans.size(); i++)
	{
		//collide with other humans
		for (int j = i+1; j < m_humans.size(); j++)
		{

			m_humans[i]->collideWithAgent(m_humans[j]);
		}
		//_humans[i]->update(_levels[_currentLevel]->getLevelData(), _humans, _zombies);
	}

	

	

}

//update all bullets
void MainGame::updateBullets(float deltaTime)
{
	//update and collide with world
	//for all bullets in game
	for (int i = 0; i < m_bullets.size();)
	{
		//if update returns true, bullet collided with wall
		//update each bullet
		if (m_bullets[i].update(m_levels[m_currentLevel]->getLevelData(), deltaTime))
		{
			//moves current bullet to the last element
			m_bullets[i] = m_bullets.back();
			//pops off the last element
			m_bullets.pop_back();
		}
		else
		{
			//only increment when a bullet wasnt removed
			i++;
		}
	}

	bool wasBulletRemoved;

	//Collide with humans and zombies
	for (int i = 0; i < m_bullets.size(); i++)
	{
		wasBulletRemoved = false;
		//loop through zombies
		for (int j = 0; j < m_zombies.size();)
		{
			//if bullet collides with a zombie
			if (m_bullets[i].collideWithAgent(m_zombies[j]))
			{
				//add blood
				addBlood(m_bullets[i].getPosition(), 5);

				//damage zombie, kill it if out of health
				//if this is true, zombie died
				if (m_zombies[j]->applyDamage(m_bullets[i].getDamage()))
				{
					//if zombie died...
					//delete zombie data
					delete m_zombies[j];
					//move empty element to last index
					m_zombies[j] = m_zombies.back();
					//pop off last index
					m_zombies.pop_back();
					//count another killed zombie
					m_numZombiesKilled++;
					
				}
				else
				{
					//increment j if a zombie was not removed
					j++;
				}


				//remove bullet
				//moves current bullet to the last element
				m_bullets[i] = m_bullets.back();
				//pops off the last element
				m_bullets.pop_back();
				//set bool to indicate removal of bullet
				wasBulletRemoved = true;
				//decrement i to be sure we dont skip bullets
				i--;
				//if a bullet is removed, stop looping through zombies 
				break;
			}
			else
			{
				//if no zombies were collided with, increment j
				j++;
			}
		}
		//loop through humans only if bullet still exists
		if (wasBulletRemoved == false)
		{
			//loop through all humans, starting at 1 to skip player
			for (int j = 1; j < m_humans.size();)
			{
				//if bullet collides with a human
				if (m_bullets[i].collideWithAgent(m_humans[j]))
				{
					//add blood
					addBlood(m_bullets[i].getPosition(), 5);

					//damage human, kill it if out of health
					//if this is true, human died
					if (m_humans[j]->applyDamage(m_bullets[i].getDamage()))
					{
						//if human died...
						//delete human data
						delete  m_humans[j];
						//move empty element to last index
						m_humans[j] = m_humans.back();
						//pop off last index
						m_humans.pop_back();
						//count another killed human
						m_numHumansKilled++;
						
					}
					else
					{
						//increment j if a human was not removed
						j++;
					}


					//remove bullet
					//moves current bullet to the last element
					m_bullets[i] = m_bullets.back();
					//pops off the last element
					m_bullets.pop_back();
					//set bool to indicate removal of bullet
					wasBulletRemoved = true;
					//decrement i to be sure we dont skip bullets
					i--;
					//if a bullet is removed, stop looping through humans 
					break;
				}
				else
				{
					//if no humans were collided with, increment j
					j++;
				}
			}
		}
	}
		
}


void MainGame::checkVictory()
{
	// TODO: support for multiple levels
	// _currentLevel ++; initLevel(...);


	//if all zombies are dead, we win
	if (m_zombies.empty())
	{
		std::printf("*** You win!!! *** \n You killed %d civilians and %d zombies. \n There are %d/%d civilians remaining.\n",
			m_numHumansKilled, m_numZombiesKilled ,m_humans.size()-1, m_levels[m_currentLevel] -> getNumHumans());
		GameEngine::fatalError("");
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
			m_gameState = GameState::EXIT;
			break;
		case SDL_MOUSEMOTION:
			m_inputManager.setMouseCoords(evnt.motion.x, evnt.motion.y);
			break;
		case SDL_KEYDOWN:
			m_inputManager.pressKey(evnt.key.keysym.sym);
			break;
		case SDL_KEYUP:
			m_inputManager.releaseKey(evnt.key.keysym.sym);
			break;
		case SDL_MOUSEBUTTONDOWN:
			m_inputManager.pressKey(evnt.button.button);
			break;
		case SDL_MOUSEBUTTONUP:
			m_inputManager.releaseKey(evnt.button.button);
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


	m_textureProgram.use();


	//draw game here
	glActiveTexture(GL_TEXTURE0);
	//make sure shader uses texture 0
	GLint textureUniform = m_textureProgram.getUniformLocation("mySampler");
	glUniform1i(textureUniform,0);

	//grab the camera matrix
	glm::mat4 projectionMatrix = m_camera.getCameraMatrix();
	GLint pUniform = m_textureProgram.getUniformLocation("P");
	glUniformMatrix4fv(pUniform, 1, GL_FALSE, &projectionMatrix[0][0]);

	//draw level
	m_levels[m_currentLevel]-> draw();

	//begin drawing agents
	m_agentSpriteBatch.begin();

	//agent dimensions
	const glm::vec2 agentDims(AGENT_RADIUS*2);

	//draw all humans including player
	for (int i = 0; i < m_humans.size(); i++)
	{
		//if the agent is in view of the camera (for camera culling)
		if (m_camera.isBoxInView(m_humans[i]->getPosition(), agentDims))
		{
			//draw the agent
			m_humans[i]->draw(m_agentSpriteBatch);
		}
		
	}

	//draw all zombies 
	for (int i = 0; i < m_zombies.size(); i++)
	{
		//if the agent is in view of the camera (for camera culling)
		if (m_camera.isBoxInView(m_zombies[i]->getPosition(), agentDims))
		{
			//draw the agent
			m_zombies[i]->draw(m_agentSpriteBatch);
		}
	}

	//draw all bullets
	for (int i = 0; i < m_bullets.size(); i++)
	{
		m_bullets[i].draw(m_agentSpriteBatch);
	}

	//end drawing agents
	m_agentSpriteBatch.end();

	//render all agents
	m_agentSpriteBatch.renderBatch();

	//render particles
	m_particleEngine.draw(&m_agentSpriteBatch);


	//draw heads up display
	drawHud();

	m_textureProgram.unuse();

	//swap buffer and draw everything to screen
	m_window.swapBuffer();
}

void MainGame::drawHud()
{
	//char buffer to fill
	char buffer[256];

	//grab the camera matrix
	glm::mat4 projectionMatrix = m_hudCamera.getCameraMatrix();
	GLint pUniform = m_textureProgram.getUniformLocation("P");
	glUniformMatrix4fv(pUniform, 1, GL_FALSE, &projectionMatrix[0][0]);

	//begin drawing sprite batch
	m_hudSpriteBatch.begin();

	//sprintf prints into a buffer instead of to screen
	//print number of humans remaining
	sprintf_s(buffer,"Num Humans %d", m_humans.size());

	m_spriteFont->draw(m_hudSpriteBatch, buffer, glm::vec2(0,0),
		glm::vec2(0.5), 0.0f, GameEngine::ColorRGBA8(255,255,255,255));

	//sprintf prints into a buffer instead of to screen
	//print number of humans remaining
	sprintf_s(buffer, "Num Zombies %d", m_zombies.size());

	m_spriteFont->draw(m_hudSpriteBatch, buffer, glm::vec2(0, 36),
		glm::vec2(0.5), 0.0f, GameEngine::ColorRGBA8(255, 255, 255, 255));

	//end drawing sprite batch
	m_hudSpriteBatch.end();
	//render sprite batch
	m_hudSpriteBatch.renderBatch();
}

void MainGame::addBlood(const glm::vec2& position, int numParticles)
{
	//generate random angle
	static std::mt19937 randEngine(time(nullptr));
	static std::uniform_real_distribution<float> randAngle(0.0f, 360.0f);//,2*M_PI );

	//starting velocity
	glm::vec2 velocity(2.0f, 0.0f);
	//starting color
	GameEngine::ColorRGBA8 color(255,0,0,255);
	
	for (int i = 0; i < numParticles; i++)
	{
		m_bloodParticleBatch->addParticle(position, glm::rotate(velocity, randAngle(randEngine)), color, 30.0f);
	}
	
}

