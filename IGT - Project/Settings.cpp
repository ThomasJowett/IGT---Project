#include "Settings.h"
#include <SDL.h>
#include <iostream>
#include <fstream>
#include <vector>

extern SDL_Window* gWindow;
extern SDL_GLContext gGLContext;

static Settings* instance = 0;

static inline std::vector<std::string> SplitString(const std::string &s, char delim);

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

			const char* lineCStr = line.c_str();

			if (lineCStr == "Display")
				std::cout << lineCStr << std::endl;
		}
	}
	else
	{
		std::cerr << "Unable to locate Settings.ini\n";
	}

	mOrtho_Width = 480;
	mOrtho_Height = 270;

	mScreen_Width = 1920;
	mScreen_Height = 1080;
	mFullscreen = false;
	mFPS = 60;
	mVSYNC = true;
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

	mScreen_Scale = (float)(mScreen_Width + mScreen_Height) / (float)(mOrtho_Width + mOrtho_Height);

	SetVsync(mVSYNC);

	mCamera->Orthographic((mScreen_Width/mScreen_Scale), (mScreen_Height/mScreen_Scale), 0, 1000);
	glViewport(0, 0, mScreen_Width, mScreen_Height);

	SaveSettings();
}

void Settings::SetResolution(int width, int height)
{
	mScreen_Width = width;
	mScreen_Height = height;
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

static inline std::vector<std::string> SplitString(const std::string &s, char delim)
{
	std::vector<std::string> elems;

	const char* cstr = s.c_str();
	unsigned int strLength = s.length();
	unsigned int start = 0;
	unsigned int end = 0;

	while (end <= strLength)
	{
		while (end <= strLength)
		{
			if (cstr[end] == delim)
				break;
			end++;
		}

		elems.push_back(s.substr(start, end - start));
		start = end + 1;
		end = start;
	}

	return elems;
}
