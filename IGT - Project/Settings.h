#pragma once

#include "Camera.h"
#include <SDL.h>

class Settings
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

	void SetCamera(Camera* camera) { mCamera = camera; }
	
	int GetScreenWidth() { return mScreen_Width; }
	int GetScreenHeight() { return mScreen_Height; }
	float GetScreenScale() { return mScreen_Scale; }

	bool GetIsFullScreen() { return mFullscreen; }
	bool GetIsVsync() { return mVSYNC; }

private:
	Settings();
	~Settings();
	
	int mScreen_Width;
	int mScreen_Height;

	int mOrtho_Width;
	int mOrtho_Height;

	float mScreen_Scale;
	float mZoom;

	SDL_DisplayMode mCurrentMode;

	bool mFullscreen;

	bool mVSYNC;

	Camera* mCamera;
};