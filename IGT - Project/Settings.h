#pragma once

#include "Camera.h"

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

	void SetCamera(Camera* camera) { mCamera = camera; }
	
	int GetScreenWidth() { return mScreen_Width; }
	int GetScreenHeight() { return mScreen_Height; }
	float GetScreenScale() { return mScreen_Scale; }

	bool GetIsFullScreen() { return mFullscreen; }

private:
	Settings();
	~Settings();
	
	int mScreen_Width;
	int mScreen_Height;

	float mScreen_Scale;
	float mZoom;

	bool mFullscreen;

	int mFPS;

	bool mVSYNC;

	Camera* mCamera;
};