#version 130
//The vertex shader operates on each vertex

//input data from the VBO. Each vertex is 2 floats
in vec2 vertexPosition;

//input data for color, pass to fragment shader
in vec4 vertexColor;

in vec2 vertexUV;

out vec2 fragmentPosition;
//output for color, must match input on fragment shader
out vec4 fragmentColor;

out vec2 fragmentUV;

void main()
{
	//set the x,y position on the screen
	gl_Position.xy = vertexPosition;
	//the z position is zero since we are in 2d
	gl_Position.z = 0.0;
	
	//indicate that the coordiantes are normalized
	gl_Position.w = 1.0;
	
	
	fragmentPosition = vertexPosition;
	fragmentColor = vertexColor;
	//1 - vertex y to flip it
	fragmentUV = vec2(vertexUV.x, 1.0 - vertexUV.y);

}