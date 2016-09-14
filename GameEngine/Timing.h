#pragma once

namespace GameEngine
{

	class FPSLimiter
	{
	public:
		FPSLimiter();

		void init(float maxFPS);

		void setMaxFPS(float maxFPS);

		void begin();

		//end will return current FPS
		float end();

	private:
		void CalculateFPS();

		float _fps;
		float _frameTime;
		float _maxFPS;
		unsigned int _startTicks;

	};


}
