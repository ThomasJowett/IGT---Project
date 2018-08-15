#ifndef _SOUNDMANAGER_H
#define	_SOUNDMANAGER_H

#include <SDL.h>
#include <SDL_mixer.h>
#include <map>

#include "Commons.h"
#include "Vector.h"

class SoundManager
{
public:
	~SoundManager();

	static SoundManager * GetInstance()
	{
		if (instance == 0)
		{
			instance = new SoundManager();
		}
		return instance;
	}

	void PlayMusic(const char* filename);
	void PauseMusic();
	void PlayPauseMusic();
	void StopMusic();
	bool PlaySoundEffect(const std::string filename, int channel, int repeat);
	bool PlaySoundAtLocation(const std::string filename, int channel, int repeat, Vector2D location);
	void LoadMusic(const char* filename);
	bool LoadSoundEffect(std::string filename);

	void SetSoundEffectVolume(unsigned int volume);
	void SetMusicVolume(unsigned int volume);
	void SetMasterVolume(unsigned int volume);

	int GetMusicVolume() const { return mMusicVolume; }
	int GetSoundEffectVolume() const { return mSoundEffectsVolume; }
	int GetMasterVolume()const { return mMasterVolume; }

private:
	void LoadSettings();
	void SaveSettings();
	static const char* mCurrentMusicFilename;
	Mix_Music* mMusic;

	static SoundManager * instance;

	std::map<std::string, Mix_Chunk* > mSoundEffects;

	static void InitAudioDevice();

	SoundManager();

	int mMusicVolume;
	float mSoundEffectsVolume;
	float mMasterVolume;
};

#endif