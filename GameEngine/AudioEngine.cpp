#include "AudioEngine.h"
#include "EngineErrors.h"

namespace GameEngine
{
	void SoundEffect::play(int loops /* = 0 */)
	{
		//play the sound effect chunk
		if (Mix_PlayChannel(-1, m_chunk, loops) == -1)
		{
			if (Mix_PlayChannel(0, m_chunk, loops) == -1)
			{
				fatalError("Mix PlayChannel error: " + std::string(Mix_GetError()));
			}
		}
	}

	void Music::play(int loops /* =-1 */)
	{
		Mix_PlayMusic(m_music, loops);
	}

	void Music::pause()
	{
		Mix_PauseMusic();
	}
	void Music::stop()
	{
		Mix_HaltMusic();
	}
	void Music::resume()
	{
		Mix_ResumeMusic();
	}

	AudioEngine::AudioEngine()
	{
		//empty
	}


	AudioEngine::~AudioEngine()
	{
		destroy();
	}

	//initialize audio
	void AudioEngine::init()
	{
		//Parameter can be a bitwise combination of MIX_INIT_FAC,
		//MIX_INIT_MOD, MIX_INIT_MP3, MIX_INIT_OGG
		if (Mix_Init(MIX_INIT_MP3 | MIX_INIT_OGG) == -1)
		{
			fatalError("Mix Init error: " + std::string(Mix_GetError()));
		}

		if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 1024) == -1)
		{
			fatalError("Mix OpenAudio error: " + std::string(Mix_GetError()));
		}

		m_isInitialized = true;
	}
	//release resources
	void AudioEngine::destroy()
	{
		//only destroy if the audio engine has been initialized fully
		if (m_isInitialized == true)
		{
			m_isInitialized = false;
			
			Mix_Quit();
		}
	}

	//caches sound files (loads once, if used again returns a pointer)
	SoundEffect AudioEngine::loadSoundEffect(const std::string& filePath)
	{
		//try to find the audio in the cache
		auto it = m_effectMap.find(filePath);

		SoundEffect effect;

		//if it didnt find the sound clip
		if (it == m_effectMap.end())
		{
			//failed to find it, must load
			Mix_Chunk* chunk = Mix_LoadWAV(filePath.c_str());
			//check for errors
			if (chunk == nullptr)
			{
				fatalError("Mix LoadWAV error: " + std::string(Mix_GetError()));
			}
			effect.m_chunk = chunk;
			m_effectMap[filePath] = chunk;
		}
		else//<found it, already cached
		{
			//store the currently pointed to chunk as the effect
			effect.m_chunk = it->second;
		}

		return effect;
		
	}
	//caches music files (loads once, if used again returns a pointer)
	Music AudioEngine::loadMusic(const std::string& filePath)
	{
		//try to find the audio in the cache
		auto it = m_musicMap.find(filePath);

		Music music;

		//if it didnt find the sound clip
		if (it == m_musicMap.end())
		{
			//failed to find it, must load
			Mix_Music* mixMusic = Mix_LoadMUS(filePath.c_str());
			//check for errors
			if (mixMusic == nullptr)
			{
				fatalError("Mix LoadMUS error: " + std::string(Mix_GetError()));
			}
			music.m_music = mixMusic;
			m_musicMap[filePath] = mixMusic;
		}
		else//<found it, already cached
		{
			//store the currently pointed to chunk as the effect
			music.m_music = it->second;
		}

		return music;
	}
}
