#include "Agent.h"
#include <GameEngine/ResourceManager.h>


Agent::Agent()
{
}


Agent::~Agent()
{
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
