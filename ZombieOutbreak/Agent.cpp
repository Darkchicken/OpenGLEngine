#include "Agent.h"
#include "Level.h"
#include <GameEngine/ResourceManager.h>


Agent::Agent()
{
}


Agent::~Agent()
{
}


void Agent::collideWithLevel(const std::vector<std::string>& levelData)
{
	//vector of all positions of tiles that you can collide with
	std::vector<glm::vec2> collideTilePositions;

	//check 4 corners
	//1st corner
	checkTilePosition(levelData, collideTilePositions, _position.x, _position.y);
	//2nd corner
	checkTilePosition(levelData, collideTilePositions, _position.x + AGENT_WIDTH, _position.y);
	//3rd corner
	checkTilePosition(levelData, collideTilePositions, _position.x, _position.y + AGENT_WIDTH);
	//4th corner
	checkTilePosition(levelData, collideTilePositions, _position.x+AGENT_WIDTH, _position.y + AGENT_WIDTH);

	


	
}

void Agent::draw(GameEngine::SpriteBatch& _spriteBatch)
{

	static int textureID = GameEngine::ResourceManager::getTexture("Textures/circle.png").id;
	const glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);


	glm::vec4 destRect;
	destRect.x = _position.x;
	destRect.y = _position.y;
	destRect.z = AGENT_WIDTH;
	destRect.w = AGENT_WIDTH;
	_spriteBatch.draw(destRect,uvRect, textureID, 0, _color);
}

void Agent::checkTilePosition(const std::vector<std::string>& levelData,std::vector<glm::vec2>& collideTilePositions, float cornerX, float cornerY)
{
	//first corner (convert position to int)
	glm::vec2 cornerPos = glm::vec2(floor(cornerX / (float)TILE_WIDTH),
		floor(cornerY / (float)TILE_WIDTH)) *(float)TILE_WIDTH;

	//check if tile is collidable (if its not an empty space)
	if (levelData[cornerPos.y][cornerPos.x] != '.')
	{
		collideTilePositions.push_back(cornerPos + glm::vec2(TILE_WIDTH / 2.0f));
	}
}

