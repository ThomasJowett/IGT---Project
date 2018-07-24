#include "Settings.h"
#include <SDL.h>
#include <iostream>

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
}

//Loads settings from Settings.ini
void Settings::LoadSettings()
{
	mScreen_Width = 1280;
	mScreen_Height = 720;
	mFullscreen = false;
	mFPS = 60;
	mVSYNC = true;
}

void Settings::ApplySettings()
{
	//TODO: write function that changes the ortho width depending on screen size so that it maintains aspect ratio
	//if (mScreen_Width >= 1920)
		//mOrtho_Width = 480;
	//else
	//	mOrtho_Width = 

	//if (mScreen_Height >= 1080)
		//mOrtho_Height = 240;

	mScreen_Scale =  (float)(mScreen_Width)/(float)(480);

	if(mFullscreen)
		SDL_SetWindowFullscreen(gWindow, SDL_WINDOW_FULLSCREEN);

	SDL_SetWindowSize(gWindow, mScreen_Width, mScreen_Height);

	//mCamera->Orthographic(mCamera->GetTransform()->mPosition,
	//	(mScreen_Width / mScreen_Scale), (mScreen_Height / mScreen_Scale), 0, 1000);
	mCamera->Orthographic((mScreen_Width/mScreen_Scale), (mScreen_Height/mScreen_Scale), 0, 1000);
	glViewport(0, 0, mScreen_Width, mScreen_Height);
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

Settings::Settings()
{
	instance = this;
	LoadSettings();
}


Settings::~Settings()
{
}
