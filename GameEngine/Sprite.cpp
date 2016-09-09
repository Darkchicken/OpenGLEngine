#include "Sprite.h"
#include "Vertex.h"
#include "ResourceManager.h"

#include <cstddef>

namespace GameEngine
{
	Sprite::Sprite()
	{
		//start by setting vertex buffer id to 0 (like a null)
		_vboID = 0;
	}


	Sprite::~Sprite()
	{
		if (_vboID != 0)
		{
			glDeleteBuffers(1, &_vboID);
		}
	}

	void Sprite::init(float x, float y, int width, int height, std::string texturePath)
	{
		_x = x;
		_y = y;
		_width = width;
		_height = height;

		_texture = ResourceManager::getTexture(texturePath);

		//check if vertex buffer id has not been allocated
		if (_vboID == 0)
		{
			//assign a vertex buffer id to this sprite
			//changes vboid to the new id (passed by reference to change original variable)
			glGenBuffers(1, &_vboID);
		}
		Vertex vertexData[6];

		//first triangle
		vertexData[0].setPosition(x + _width, y + _height);
		vertexData[0].setUV(1.0f, 1.0f);

		vertexData[1].setPosition(x, y + _height);
		vertexData[1].setUV(0.0f, 1.0f);

		vertexData[2].setPosition(x, y);
		vertexData[2].setUV(0.0f, 0.0f);

		//second triangle
		vertexData[3].setPosition(x, y);
		vertexData[3].setUV(0.0f, 0.0f);

		vertexData[4].setPosition(x + _width, y);
		vertexData[4].setUV(1.0f, 0.0f);

		vertexData[5].setPosition(x + _width, y + _height);
		vertexData[5].setUV(1.0f, 1.0f);

		//set vertex colors
		for (int i = 0; i < 6; i++)
		{
			vertexData[i].setColor(255, 255, 0, 255);
		}

		vertexData[1].setColor(0, 0, 255, 255);


		vertexData[4].setColor(0, 255, 0, 255);

		//set a buffer active
		glBindBuffer(GL_ARRAY_BUFFER, _vboID);
		//upload data, must pass a pointer to beginning of data (but arrays can be used as pointers)
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);
		//unbind buffer
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void Sprite::draw()
	{
		//bind texture of this sprite
		glBindTexture(GL_TEXTURE_2D, _texture.id);

		//set a buffer active
		glBindBuffer(GL_ARRAY_BUFFER, _vboID);
		//send on vertex attrib array for position, 0 is for first element
		glEnableVertexAttribArray(0);

		//this is the position attribute pointer (last value is the byte offset of the data, its 0 here because the position data is the first data)
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));

		//this is the color attribute pointer 
		glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void*)offsetof(Vertex, color));

		//this is the UV attribute pointer 
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));

		//actually draw data (mode to draw in, first element, number of elements to draw (6 verticies))
		glDrawArrays(GL_TRIANGLES, 0, 6);

		glDisableVertexAttribArray(0);
		//unbind buffer
		glBindBuffer(GL_ARRAY_BUFFER, 0);

	}
}
