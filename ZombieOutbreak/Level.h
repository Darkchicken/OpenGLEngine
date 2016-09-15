#pragma once

#include <string>
#include <vector>

#include <GameEngine/SpriteBatch.h>

const int TILE_WIDTH = 64; //pixels


class Level
{
public:
	//level is loaded in constructor (from file)
	Level(const std::string& fileName);

	~Level();

	//draw level function
	void draw();

	//Getters
	int getWidth() const { return _levelData[0].size(); }
	int getHeight() const { return _levelData.size(); }
	glm::vec2 getStartPlayerPos()const { return _startPlayerPos; };
	const std::vector<glm::vec2>& getStartZombiePos() const { return _zombieStartPositions; };
	const std::vector<std::string>& getLevelData() const { return _levelData; };

	int getNumHumans() const { return _numHumans; };



private:

	std::vector<std::string> _levelData;///< array of strings to store level data

	//will be built once, then constantly rendered
	GameEngine::SpriteBatch _spriteBatch; ///< The spritebatch containing all level sprites

	int _numHumans; ///< the number of humans to randomly distribute in the level

	//ivec is integer vector
	glm::vec2 _startPlayerPos;///< The starting position of the player

	std::vector<glm::vec2> _zombieStartPositions; ///< All starting positions for zombies
};

