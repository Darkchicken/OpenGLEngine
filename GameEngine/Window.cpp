#include "Window.h"
#include "Errors.h"

namespace GameEngine
{
	Window::Window()
	{
	}


	Window::~Window()
	{
	}

	int Window::create(std::string windowName, int screenWidth, int screenHeight, unsigned int currentFlags)
	{
		Uint32 flags = SDL_WINDOW_OPENGL;
		//if user selected invisible
		if (currentFlags & INVISIBLE)
		{
			//or the bit onto flags
			flags |= SDL_WINDOW_HIDDEN;
		}
		if (currentFlags & FULLSCREEN)
		{
			flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
		}
		if (currentFlags & BORDERLESS)
		{
			flags |= SDL_WINDOW_BORDERLESS;
		}

		//Creates Window
		_sdlWindow = SDL_CreateWindow(windowName.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenWidth, screenHeight, SDL_WINDOW_OPENGL);
		if (_sdlWindow == nullptr)
		{
			fatalError("SDL window could not be opened!");
		}

		SDL_GLContext glContext = SDL_GL_CreateContext(_sdlWindow);
		if (glContext == nullptr)
		{
			fatalError("SDL GL Context could not be created!");
		}
		//set up glew
		GLenum error = glewInit();

		if (error != GLEW_OK)
		{
			fatalError("Could not initialize GLEW!");
		}

		//Check openGL version and display it
		std::printf("***	OpenGL Version: %s	***\n", glGetString(GL_VERSION));

		//set background color
		glClearColor(0.0f, 0.0f, 1.0f, 1.0f);

		//enable VSYNC (1 is on, 0 is off)
		SDL_GL_SetSwapInterval(1);

		//enable alpha blend
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		return 0;
	}

	void Window::swapBuffer()
	{
		SDL_GL_SwapWindow(_sdlWindow);
	}
}
