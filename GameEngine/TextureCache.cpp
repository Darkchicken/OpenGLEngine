#include "TextureCache.h"
#include "ImageLoader.h"
#include <iostream>

namespace GameEngine
{
	TextureCache::TextureCache()
	{
	}


	TextureCache::~TextureCache()
	{
	}

	GLTexture TextureCache::getTexture(std::string texturePath)
	{
		//look for a texture stored in map with key texturePath
		//auto makes it into returned type
		//in this case-- std::map<std::string, GLTexture>::iterator
		auto mit = _textureMap.find(texturePath);

		//check if texture is not in map
		//if not, load the texture
		if (mit == _textureMap.end())
		{
			//load new texture
			GLTexture newTexture = ImageLoader::LoadPNG(texturePath);

			//make a pair of path and texture
			//insert into texture map
			_textureMap.insert(make_pair(texturePath, newTexture));

			std::cout << "Loaded Texture\n";
			return newTexture;
		}
		//if in map, mit is pointing to texture
		//use arrow to access elements, because mit is a pointer
		std::cout << "Used Cached Texture\n";

		return mit->second;
	}
}
