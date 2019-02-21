#pragma once

//#include "Camera.h"
#include "Messaging.h"
#include "Commons.h"
#include <SDL.h>
#include <vector>

constexpr int DEFAULT_ORTHO_WIDTH = 480;
constexpr int DEFAULT_ORTHO_HEIGHT = 270;

enum class SettingsEvent { ON_RESOLUTION_CHANGE};

class Settings
	:public Subject<SettingsEvent, Vector2D>
{
public:

	static Settings* GetInstance();

	void SaveSettings();

	void LoadSettings();

	void ApplySettings();

	void SetResolution(int width, int height);
	void SetScreenScale(float scale);	
	void SetFullScreen(bool isFullscreen);
	void SetDisplayMode(SDL_DisplayMode mode);
	void SetVsync(bool isVsyncEnabled);
	
	int GetScreenWidth() { return mScreen_Width; }
	int GetScreenHeight() { return mScreen_Height; }

	float GetScreenOrthoWidth() { return mScreenOrthoWidth; }
	float GetScreenOrthoHeight() { return mScreenOrthoHeight; }

	float GetScreenScale() { return mScreen_Scale; }

	bool GetIsFullScreen() { return mFullscreen; }
	bool GetIsVsync() { return mVSYNC; }

private:
	Settings();
	~Settings();
	
	int mScreen_Width = 1920;
	int mScreen_Height = 1080;

	float mScreenOrthoWidth;
	float mScreenOrthoHeight;

	float mScreen_Scale;
	float mZoom = 1;

	SDL_DisplayMode mCurrentMode;

	bool mFullscreen;

	bool mVSYNC = true;

	//Camera* mCamera;
};