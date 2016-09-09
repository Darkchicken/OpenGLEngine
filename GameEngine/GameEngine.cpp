#include <SDL/SDL.h>
#include <GL/glew.h>
#include "GameEngine.h"
namespace GameEngine
{
	int init()
	{
		//initialize SDL
		SDL_Init(SDL_INIT_EVERYTHING);

		//makes it so you have 2 windows, one you draw to while you clear the other
		//prevents flickering
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

		return 0;
	}
}