#pragma once
#include <GL/glew.h>
#include "GLTexture.h"
#include <string>

class Sprite
{
public:
	Sprite();
	~Sprite();

	void init(float x, float y, int width, int height, std::string texturePath);

	void draw();

private:
	int _x;
	int _y;
	int _width;
	int _height;
	//GL unsigned in guaranteed to be 32 bits
	GLuint _vboID;

	//texture info
	GLTexture _texture;

	
};

