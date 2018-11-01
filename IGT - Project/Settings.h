#pragma once

#include "Camera.h"
#include "Messaging.h"
#include <SDL.h>

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

	void SetCamera(Camera* camera) { mCamera = camera; }
	Camera* GetCamera() { return mCamera; }
	
	int GetScreenWidth() { return mScreen_Width; }
	int GetScreenHeight() { return mScreen_Height; }
	int GetOrthoWidth() { return mOrtho_Width; }
	int GetOrthoHeight() { return mOrtho_Height; }
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

	//int mDefaultOrtho_Width = 960;
	//int mDefaultOrtho_Height = 540;
	int mDefaultOrtho_Width = 480;
	int mDefaultOrtho_Height = 270;

	float mScreen_Scale;
	float mZoom;

	SDL_DisplayMode mCurrentMode;

	bool mFullscreen;

	bool mVSYNC;

	Camera* mCamera;
};