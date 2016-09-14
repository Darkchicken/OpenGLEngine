
#include "Timing.h"
#include <SDL/SDL.h>

namespace GameEngine
{

	
	
		FPSLimiter::FPSLimiter()
		{

		}

		void FPSLimiter::init(float maxFPS)
		{
			setMaxFPS(maxFPS);

		}
		void FPSLimiter::setMaxFPS(float maxFPS)
		{
			_maxFPS = maxFPS;
		}

		void FPSLimiter::begin()
		{

			_startTicks = SDL_GetTicks();
		}

		//end will return current FPS
		float FPSLimiter::end()
		{
			CalculateFPS();
			//time it took to run code in game loop
			float frameTicks = SDL_GetTicks() - _startTicks;

			//limit FPS to max fps
			// ms/s divided by f/s = ms/f
			if (1000.0f / _maxFPS > frameTicks)
			{
				SDL_Delay(1000.0f / _maxFPS - frameTicks);
			}

			return _fps;
		}

		void FPSLimiter::CalculateFPS()
		{
			//number of frames to average
			static const int NUM_SAMPLES = 10;
			//constant num samples because you get an error without it
			static float frameTimes[NUM_SAMPLES];
			//where are you in frametimes buffer
			static int currentFrame = 0;

			//get current ticks, previous ticks
			static float previousTicks = SDL_GetTicks();
			float currentTicks;
			currentTicks = SDL_GetTicks();

			_frameTime = currentTicks - previousTicks;
			frameTimes[currentFrame % NUM_SAMPLES] = _frameTime;

			//set prev ticks to current ticks for next iteration
			previousTicks = currentTicks;

			int count;

			//increment current frame
			currentFrame++;


			if (currentFrame < NUM_SAMPLES)
			{
				count = currentFrame;
			}
			else
			{
				count = NUM_SAMPLES;
			}
			//on each call to CalculateFPS, set frameTimeAverage to 0...
			float frameTimeAverage = 0;
			//then add all frame times together...
			for (int i = 0; i < count; i++)
			{
				frameTimeAverage += frameTimes[i];
			}
			//and average them
			frameTimeAverage /= count;

			//then calculate frames per second
			if (frameTimeAverage > 0)
			{
				//convert frame time to f/s
				_fps = 1000.0f / frameTimeAverage;
			}
			else
			{
				_fps = 60.0f;
			}
		}
	

}