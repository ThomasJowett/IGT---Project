#include "SoundManager.h"

#include <iostream>
#include <vector>

SoundManager * SoundManager::instance = 0;
const char* SoundManager::mCurrentMusicFilename = "";

void SoundManager::InitAudioDevice()
{
	if (SDL_Init(SDL_INIT_AUDIO) != -1)
	{
		if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
		{
			std::cerr << "Mixer could not initialise. Error: " << Mix_GetError() << std::endl;
		}
	}
	else
	{
		std::cerr << "Error initializing SDL audio subsystem. Error: " << SDL_GetError();
	}
}

SoundManager::SoundManager()
{
	instance = this;
	SoundManager::InitAudioDevice();
}

SoundManager::~SoundManager()
{
	Mix_CloseAudio();

	std::map<std::string, Mix_Chunk*>::iterator i;
	for (i = mSoundEffects.begin(); i != mSoundEffects.end(); i++)
	{
		Mix_FreeChunk(i->second);
	}
	mSoundEffects.clear();
}

void SoundManager::PlayMusic(const char* filename)
{
	if (mCurrentMusicFilename != filename)
	{
		LoadMusic(filename);
	}

	if (Mix_PlayingMusic() == 0)
	{
		Mix_PlayMusic(mMusic, -1);
	}
}

void SoundManager::PauseMusic()
{
	if (Mix_PlayingMusic() == 1)
	{
		Mix_PauseMusic();
	}
}

void SoundManager::PlayPauseMusic()
{
	if (Mix_PlayingMusic() == 1)
	{
		if (Mix_PausedMusic() == 1)
		{
			Mix_ResumeMusic();
		}
		else
		{
			Mix_PauseMusic();
		}
	}
}

void SoundManager::StopMusic()
{
	Mix_HaltMusic();
	mCurrentMusicFilename = "";
}

bool SoundManager::PlaySoundEffect(const std::string filename, int channel, int repeat)
{
	if (mSoundEffects.find(filename) != mSoundEffects.end())
	{
		Mix_Volume(Mix_PlayChannel(channel, mSoundEffects.at(filename), repeat), (float)mSoundEffectsVolume * ((float)mMasterVolume / (float)SDL_MIX_MAXVOLUME));
		return true;
	}
	else
	{
		if (LoadSoundEffect(filename))
			PlaySoundEffect(filename, channel, repeat);
		else
			return false;
	}
	return true;
}

bool SoundManager::PlaySoundAtLocation(const std::string filename, int channel, int repeat, Vector2D location)
{

	return false;
}

void SoundManager::LoadMusic(const char* filename)
{
	mMusic = Mix_LoadMUS(filename);
	if (mMusic == nullptr)
	{
		std::cerr << "Failed to load background music! Error: " << Mix_GetError() << std::endl;
	}
	else
	{
		mCurrentMusicFilename = filename;
	}
}

bool SoundManager::LoadSoundEffect(std::string filename)
{
	Mix_Chunk* sound = Mix_LoadWAV(filename.c_str());
	if (sound == nullptr)
	{
		std::cerr << "Failed to load Sound effect! Error: " << Mix_GetError() << std::endl;
		return false;
	}
	else
	{
		mSoundEffects.insert(std::pair<std::string, Mix_Chunk*>{ filename, sound });
		return true;
	}
}

void SoundManager::SetSoundEffectVolume(unsigned int volume)
{
	if (volume <= SDL_MIX_MAXVOLUME)
	{
		mSoundEffectsVolume = volume;
	}
}

void SoundManager::SetMusicVolume(unsigned int volume)
{
	if (volume <= SDL_MIX_MAXVOLUME)
	{
		mMusicVolume = volume;
		Mix_VolumeMusic((int)((float)mMusicVolume * (float)((float)mMasterVolume / (float)SDL_MIX_MAXVOLUME)));
	}
}

void SoundManager::SetMasterVolume(unsigned int volume)
{
	if (volume <= SDL_MIX_MAXVOLUME)
	{
		mMasterVolume = volume;

		Mix_VolumeMusic((int)((float)mMusicVolume * (float)((float)mMasterVolume / (float)SDL_MIX_MAXVOLUME)));
	}
}