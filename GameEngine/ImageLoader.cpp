#include "ImageLoader.h"
#include "IOManager.h"
#include "picoPNG.h"
#include "EngineErrors.h"
namespace GameEngine
{
	GLTexture ImageLoader::LoadPNG(std::string filePath)
	{
		//initialize all variables in struct to 0
		GLTexture texture = {};

		std::vector<unsigned char> in;
		std::vector<unsigned char> out;
		unsigned long width, height;

		if (IOManager::readFileToBuffer(filePath, in) == false)
		{
			fatalError("Failed to load PNG file to buffer");
		}
		int errorCode = decodePNG(out, width, height, &(in[0]), in.size(), 0);

		if (errorCode != 0)
		{
			fatalError("decodePNG failed with error: " + std::to_string(errorCode));
		}
		//generate opengl texture object (1 texture, address of texture.id)
		glGenTextures(1, &(texture.id));
		//bind texture
		glBindTexture(GL_TEXTURE_2D, texture.id);
		//upload image data to texture
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &(out[0]));
		//set parameters for texture
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);//mip-mapping, use linear interpolation
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		//mip mapping (reduces resolution as image is made smaller on screen)
		glGenerateMipmap(GL_TEXTURE_2D);
		//unbind texture (bind to 0)
		glBindTexture(GL_TEXTURE_2D, 0);

		texture.width = width;
		texture.height = height;
		//return texture data
		return texture;
	}
}
