#include "ResourceManager.h"

//have to declare static variables again
TextureCache ResourceManager::_textureCache;

GLTexture ResourceManager::getTexture(std::string texturePath)
{
	return _textureCache.getTexture(texturePath);
}