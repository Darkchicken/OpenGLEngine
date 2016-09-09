#pragma once

#include <GL/glew.h>

struct Positon //makes a struct of type Position
{
	//8 bytes
	float x;
	float y;
}; 


struct Color
{
	//4 bytes (r,g,b,a)
	GLubyte r;
	GLubyte g;
	GLubyte b;
	GLubyte a;
};

struct UV
{
	float u;
	float v;
};


struct Vertex
{
	//vertex has a position and color
	Positon position;

	Color color;
	//uv texture coordinates
	UV uv;

	//set position of vertex
	void setPosition(float x, float y)
	{
		position.x = x;
		position.y = y;
	}

	//set colors of the vertex 
	void setColor(GLubyte r, GLubyte g, GLubyte b, GLubyte a)
	{
		color.r = r;
		color.g = g;
		color.b = b;
		color.a = a;
	}
	//set UV of vertex
	void setUV(float u, float v )
	{
		uv.u = u;
		uv.v = v;
	}

	
	
	
};
