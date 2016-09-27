#pragma once

#include <SDL/SDL_mixer.h>
#include <string>
#include <map>

namespace GameEngine
{
	//a class for sound effect objects
	class SoundEffect
	{
	public:
		friend class AudioEngine; //audio engine is an exception to private rule
		///plays the sound effect file
		/// @param loops: if loops = -1, loop forever 
		/// otherwise play it loops +1 times
		void play(int loops = 0);

	private:
		//stores a mix chunk to be used as a sound effect object
		Mix_Chunk* m_chunk = nullptr;
	};
	//a class for music objects
	class Music
	{
	public:
		friend class AudioEngine; //audio engine is an exception to private rule
		///plays the music file
		/// @param loops: if loops = -1, loop forever 
		/// otherwise play it loops times
		void play(int loops = -1);

		//applies to whatever music is currently playing
		static void pause();
		static void stop();
		static void resume();
	private:
		//stores a mix music to be used as a music object
		Mix_Music* m_music = nullptr;
	};

	class AudioEngine
	{
	public:
		AudioEngine();
		~AudioEngine();
		//initialize audio
		void init();
		//release resources
		void destroy();

		//caches sound files (loads once, if used again returns a pointer)
		SoundEffect loadSoundEffect(const std::string& filePath);
		//caches music files (loads once, if used again returns a pointer)
		Music loadMusic(const std::string& filePath);
	private:
		//cache for sound effects
		std::map<std::string, Mix_Chunk*> m_effectMap;
		//cache for music
		std::map<std::string, Mix_Music*> m_musicMap;

		//check if the audio engine is initialized
		bool m_isInitialized = false;
	};
}

