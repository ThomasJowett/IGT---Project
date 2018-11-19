#include "Settings.h"
#include "SoundManager.h"
#include <SDL.h>
#include <iostream>
#include <fstream>
#include <vector>

extern SDL_Window* gWindow;
extern SDL_GLContext gGLContext;

static Settings* instance = 0;

Settings * Settings::GetInstance()
{
	if (instance == 0)
	{
		return new Settings();
	}
	return instance;
}

//writes the settings out to Settings.ini file
void Settings::SaveSettings()
{
	std::ofstream file;
	file.open("Settings.ini");

	file << "[DISPLAY]" << std::endl;
	file << "SCREEN_WIDTH=" << mScreen_Width << std::endl;
	file << "SCREEN_HEIGHT=" << mScreen_Height << std::endl;
	file << "ZOOM=" << mZoom << std::endl;
	file << "FULLSCREEN=" << mFullscreen << std::endl;
	file << "V-SYNC=" << mVSYNC << std::endl;

	file << "[GAMEPLAY]" << std::endl;

	file << "[AUDIO]" << std::endl;
	file << "MUSIC=" << SoundManager::GetInstance()->GetMusicVolume() << std::endl;
	file << "SFX=" << SoundManager::GetInstance()->GetSoundEffectVolume() << std::endl;
	file << "MASTER=" << SoundManager::GetInstance()->GetMasterVolume() << std::endl;

	file.close();
}

//Loads settings from Settings.ini
void Settings::LoadSettings()
{
	std::ifstream file;
	file.open("Settings.ini");

	std::string line;

	if (file.is_open())
	{
		while (file.good())
		{
			getline(file, line);

			std::vector<std::string> lineSplit = Util::SplitString(line, '=');

			//Graphical Settings------------------------------------------------------------
			if (lineSplit[0] == "SCREEN_WIDTH")
			{
				mScreen_Width = atoi(lineSplit[1].c_str());
			}
			else if (lineSplit[0] == "SCREEN_HEIGHT")
			{
				mScreen_Height = atoi(lineSplit[1].c_str());
			}
			else if (lineSplit[0] == "ZOOM")
			{
				mZoom = (float)atof(lineSplit[1].c_str());
			}
			else if (lineSplit[0] == "FULLSCREEN")
			{
				mFullscreen = atoi(lineSplit[1].c_str());
			}
			else if (lineSplit[0] == "V-SYNC")
			{
				mVSYNC = atoi(lineSplit[1].c_str());
			}
			//Gameplay Settings-------------------------------------------------------------

			//Sound Settings----------------------------------------------------------------
			else if (lineSplit[0] == "MUSIC")
			{
				SoundManager::GetInstance()->SetMusicVolume(atoi(lineSplit[1].c_str()));
			}
			else if (lineSplit[0] == "SFX")
			{
				SoundManager::GetInstance()->SetSoundEffectVolume(atoi(lineSplit[1].c_str()));
			}
			else if (lineSplit[0] == "MASTER")
			{
				SoundManager::GetInstance()->SetMasterVolume(atoi(lineSplit[1].c_str()));
			}
		}
	}
	else
	{
		std::cerr << "Unable to locate Settings.ini\n";
	}

	file.close();

	SDL_GetCurrentDisplayMode(0, &mCurrentMode);

	mCurrentMode.h = mScreen_Height;
	mCurrentMode.w = mScreen_Width;
}

void Settings::ApplySettings()
{
	if (mFullscreen)
	{
		SDL_SetWindowFullscreen(gWindow, SDL_WINDOW_FULLSCREEN);

		SDL_SetWindowDisplayMode(gWindow, &mCurrentMode);

		mScreen_Height = mCurrentMode.h;
		mScreen_Width = mCurrentMode.w;
	}

	SDL_SetWindowSize(gWindow, mScreen_Width, mScreen_Height);

	mScreen_Scale = (float)(mScreen_Width + mScreen_Height) / (float)((mZoom * DEFAULT_ORTHO_WIDTH) + mZoom * DEFAULT_ORTHO_HEIGHT);

	SetVsync(mVSYNC);

	mCamera->Orthographic((mScreen_Width/mScreen_Scale), (mScreen_Height/mScreen_Scale), 0, 100);
	glViewport(0, 0, mScreen_Width, mScreen_Height);

	Notify(SettingsEvent::ON_RESOLUTION_CHANGE, Vector2D((float)mScreen_Width, (float)mScreen_Height));

	SaveSettings();
}

void Settings::SetResolution(int width, int height)
{
	mScreen_Width = width;
	mScreen_Height = height;

	ApplySettings();
}

void Settings::SetScreenScale(float scale)
{
	mScreen_Scale = scale;
}

void Settings::SetFullScreen(bool isFullscreen)
{
	if (mFullscreen != isFullscreen)
	{
		SDL_SetWindowFullscreen(gWindow, isFullscreen);
		mFullscreen = isFullscreen;
	}
}

void Settings::SetDisplayMode(SDL_DisplayMode mode)
{
	mCurrentMode = mode;
}

void Settings::SetVsync(bool isVsyncEnabled)
{
	if (isVsyncEnabled)
	{
		if (SDL_GL_SetSwapInterval(-1) == -1)
		{
			SDL_GL_SetSwapInterval(1);
		}

		mVSYNC = true;
	}
	else
	{
		SDL_GL_SetSwapInterval(0);

		mVSYNC = false;
	}
}

Settings::Settings()
{
	instance = this;
	LoadSettings();
}


Settings::~Settings()
{
}