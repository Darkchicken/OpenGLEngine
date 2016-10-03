#pragma once

#include <GameEngine/GLSLProgram.h>
#include <GameEngine/Window.h>
#include <GameEngine/Camera2D.h>
#include <GameEngine/InputManager.h>
#include <GameEngine/SpriteBatch.h>
#include <GameEngine/SpriteFont.h>
#include <GameEngine/AudioEngine.h>
#include <GameEngine/ParticleEngine2D.h>
#include <GameEngine/ParticleBatch2D.h>

#include <vector>

#include "Player.h"
#include "Level.h"
#include "Bullet.h"

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
	void updateAgents(float deltaTime);

	//update all bullets
	void updateBullets(float deltaTime);

	//checks if the player has won
	void checkVictory();

	//handles player input
	void processInput();

	//draws game to screen
	void drawGame();

	//draws HUD to screen
	void drawHud();

	//creates blood particle effect
	void addBlood(const glm::vec2& position, int numParticles);


	//Member variables
	GameEngine::Window m_window; //creates window for game
	GameEngine::GLSLProgram m_textureProgram; //handles shaders
	GameEngine::InputManager m_inputManager; //handles input
	GameEngine::Camera2D m_camera; //handles main camera
	GameEngine::Camera2D m_hudCamera; //handles HUD camera
	GameEngine::SpriteBatch m_agentSpriteBatch; ///<draws all agents

	GameEngine::SpriteBatch m_hudSpriteBatch; ///<draws UI font

	GameEngine::ParticleEngine2D m_particleEngine;
	GameEngine::ParticleBatch2D* m_bloodParticleBatch;

	//vector of all levels in game (pointers to level object)
	std::vector<Level*> m_levels;
	//vector of all Humans in game (pointers to human object)
	std::vector < Human*> m_humans;
	//vector of all Zombies in game (pointers to zombie object)
	std::vector < Zombie*> m_zombies;
	//vector of all bullets in game 
	std::vector < Bullet> m_bullets;

	Player* m_player;

	int m_screenWidth = 1024;
	int m_screenHeight = 768;

	float m_fps;
	int m_currentLevel;

	int m_numHumansKilled; ///<humans killed by player
	int m_numZombiesKilled; ///< zombies killed by player

	GameEngine::SpriteFont* m_spriteFont;

	GameEngine::AudioEngine m_audioEngine;

	GameState m_gameState;
	


};
