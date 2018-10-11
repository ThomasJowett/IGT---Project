#pragma once

#include "TextRender.h"
class FPS
{
public:
	FPS();
	~FPS();

	void Render(Shader* shader);
	void Update(float deltaTime);
private:
	TextRender* mTextRender;

	int mInitialTime, mFinalTime, mFrameCount;
	char FPSc_str[64] = { " " };

	Matrix4x4 mScreenPosition;
};

