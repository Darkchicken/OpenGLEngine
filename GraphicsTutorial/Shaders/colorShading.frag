#version 130
//The fragment shader operates on each pixel in a given polygon

in vec2 fragmentPosition;
//input from vertex shader output
in vec4 fragmentColor;

in vec2 fragmentUV;

//this is the 4 component float vector that 
//gets outputted to the screen for each pixel
out vec4 color;

//textures are samplers
uniform sampler2D mySampler;


void main()
{
	//look up texture "mySampler" and coordinates from fragmentUV
	//vec4 for rgba
	vec4 textureColor = texture(mySampler, fragmentUV);
	
	color = fragmentColor * textureColor;
}