#include "EngineErrors.h"
#include <cstdlib>
#include <iostream>
#include <SDL/SDL.h>
namespace GameEngine
{
	void fatalError(std::string errorString)
	{
		std::cout << errorString << std::endl;
		std::cout << "Enter any key to quit" << std::endl;
		int temp;
		std::cin >> temp;
		//shut down sdl
		SDL_Quit();
		//exit game
		exit(1);
	}
}