#include "Level.h"

#include <GameEngine/Errors.h>
#include <GameEngine/ResourceManager.h>

#include <fstream>
#include <iostream>

Level::Level(const std::string& fileName)
{


	//create new file input stream
	std::ifstream file;
	//use ifstream file to open fileName
	file.open(fileName);
	//if file is unable to open (error checking)
	if (file.fail())
	{
		//output error
		GameEngine::fatalError("Failed to open "+fileName);
	}
	//temp string to hold data (will be thrown out)
	std::string temp;
	//put first string into temp (just humans label)
	//put second string (int for number of humans) into _numHumans
	file >> temp >> _numHumans;
	//use this line to throw out the first line from the data
	std::getline(file,temp);
	//read each line of level file, store into temp
	while (std::getline(file, temp))
	{
		//push each line of temp onto _levelData vector
		_levelData.push_back(temp);
	}

	//initialize spritebatch for level sprites
	_spriteBatch.init();
	_spriteBatch.begin();

	//texture coordiantes for all textures
	glm::vec4 uvRect(0.0f,0.0f,1.0f,1.0f);

	//color
	GameEngine::Color whiteColor;
	whiteColor.r = 255;
	whiteColor.g = 255;
	whiteColor.b = 255;
	whiteColor.a = 255;

	//render all tiles
	for (int y = 0; y < _levelData.size(); y++)///< loop through y data of level data
	{
		for (int x = 0; x < _levelData[y].size(); x++)///< loop through x data of level data
		{
			//grab the tile at this index
			char tile = _levelData[y][x];

			//get destination rectangle (xPos, yPox, width, hight)
			glm::vec4 destRect(x *TILE_WIDTH, y*TILE_WIDTH , TILE_WIDTH, TILE_WIDTH);

			//process the tile
			switch (tile)
			{
			case 'R': ///< this is a red brick tile
				_spriteBatch.draw(
					destRect, 
					uvRect, 
					GameEngine::ResourceManager::getTexture("Textures/red_bricks.png").id, ///< texture id
					0.0f, ///< depth
					whiteColor	///< color
					);
				break;
			case 'G': ///< this is a glass tile
				_spriteBatch.draw(
					destRect,
					uvRect,
					GameEngine::ResourceManager::getTexture("Textures/glass.png").id, ///< texture id
					0.0f, ///< depth
					whiteColor	///< color
					);
				break;
			case 'L': ///< this is a light brick tile
				_spriteBatch.draw(
					destRect,
					uvRect,
					GameEngine::ResourceManager::getTexture("Textures/blood_bricks.png").id, ///< texture id
					0.0f, ///< depth
					whiteColor	///< color
					);
				break;
			case '@': ///< this is the player location
				//set to dot to avoid collisions with @
				_levelData[y][x] = '.';
				_startPlayerPos.x = x*TILE_WIDTH;
				_startPlayerPos.y = y*TILE_WIDTH;
				break;
			case 'Z': ///< this is the zombie location
			    //set to dot to avoid collisions with Z
				_levelData[y][x] = '.';
				_zombieStartPositions.emplace_back(x*TILE_WIDTH, y*TILE_WIDTH);
				break;
			case '.': ///< this is an empty tile
				//do nothing
				break;

			default:
				std::printf("Unexpected symbol %c at (%d,%d)", tile, x, y);
				break;
			}

		}
	}
	_spriteBatch.end();
	
	
}


Level::~Level()
{
}

void Level::draw()
{ 
	//render loaded spritebatch in constructor
	_spriteBatch.renderBatch();
}
