#include "Agent.h"
#include "Level.h"
#include <GameEngine/ResourceManager.h>
#include <algorithm>


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

	//do the collision
	for (int i = 0; i < collideTilePositions.size(); i++)
	{
		collideWithTile(collideTilePositions[i]);
	}

	


	
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
		floor(cornerY / (float)TILE_WIDTH));


	//prevent agents from being placed outside of world
	if (cornerPos.x <0 || cornerPos.x > levelData[0].length() - 1 ||
		cornerPos.y <0 || cornerPos.y > levelData.size() - 1)
	{
		return;
	}

	//check if tile is collidable (if its not an empty space)
	if (levelData[cornerPos.y][cornerPos.x] != '.')
	{
		collideTilePositions.push_back(cornerPos * (float)TILE_WIDTH + glm::vec2(TILE_WIDTH / 2.0f));
	}
}

//Access alighned bounding box (AABB) collision
void Agent::collideWithTile(glm::vec2 tilePosition)
{
	const float AGENT_RADIUS = (float)AGENT_WIDTH / 2.0f;
	const float TILE_RADIUS = (float)TILE_WIDTH / 2.0f;
	//minimum distance between agent and tile without a collision
	const float MIN_DISTANCE = AGENT_RADIUS + TILE_RADIUS;

	//player position from center
	glm::vec2 centerPlayerPos = _position + glm::vec2(AGENT_RADIUS);
	//find vector between center of player and center of tile
	glm::vec2 distVec = centerPlayerPos- tilePosition;

	//check x collision
	float xDepth = MIN_DISTANCE - abs(distVec.x);
	//check y collision
	float yDepth = MIN_DISTANCE - abs(distVec.y);

	//if this is true, we are colliding
	if (xDepth > 0 || yDepth > 0)
	{
		//use max to make sure neither value is less than 0
		if (std::max(xDepth,0.0f) < std::max(yDepth, 0.0f))
		{
			if (distVec.x < 0)
			{
				//push out in -x direction
				_position.x -= xDepth;
			}
			else
			{
				//push out in x direction
				_position.x += xDepth;
			}
			
		}
		else
		{
			if (distVec.y < 0)
			{
				//push out in -y direction
				_position.y -= yDepth;
			}
			else
			{
				//push out in y direction
				_position.y += yDepth;
			}
		}
	}



}

