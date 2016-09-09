#pragma once

#include <GL/glew.h>

namespace GameEngine
{
	struct GLTexture
	{
		GLuint id;
		//get dimensions of texture
		int width;
		int height;
	};
}
