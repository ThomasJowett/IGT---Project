#include "FPS.h"

#include <time.h>
#include <string>
#include "Settings.h"

FPS::FPS()
{
	mTextRender = new TextRender(nullptr, "Fonts/nokiafc22.ttf", 8);
	//mTextRender->UpdateText({ 255,255,255 });

	float screenScale = Settings::GetInstance()->GetScreenScale();
	float screenWidth = Settings::GetInstance()->GetScreenWidth() / screenScale;
	float screenHeight = Settings::GetInstance()->GetScreenHeight() / screenScale;

	Vector2D position;
	Vector2D offset = Vector2D(20, -20);

	position.x = ((screenWidth * -0.5f) - (screenWidth / 2)) + offset.x;
	position.y = ((screenHeight * (1 - +0.5f)) - (screenHeight / 2)) + offset.y;

	mScreenPosition = Matrix4x4::Translate(Vector3D(position.x, position.y, 0.0f));
}


FPS::~FPS()
{
}

void FPS::Render(Shader* shader)
{
	float screenScale = Settings::GetInstance()->GetScreenScale();
	float screenWidth = Settings::GetInstance()->GetScreenWidth() / screenScale;
	float screenHeight = Settings::GetInstance()->GetScreenHeight() / screenScale;

	Vector2D position;
	Vector2D offset = Vector2D(20, -15);

	position.x = ((screenWidth * -0.0f) - (screenWidth / 2)) + offset.x;
	position.y = ((screenHeight * (1 - +0.0f)) - (screenHeight / 2)) + offset.y;

	mScreenPosition = Matrix4x4::Translate(Vector3D(position.x, position.y, 0.0f));

	shader->UpdateMatrixUniform(MODEL_U, mScreenPosition, true);
	mTextRender->Render(shader);
}

void FPS::Update(float deltaTime)
{

	mFrameCount++;
	mFinalTime = time(NULL);

	if (mFinalTime - mInitialTime >= 1)
	{
		sprintf_s(FPSc_str, "%i", mFrameCount / (mFinalTime - mInitialTime));
		mTextRender->UpdateText(FPSc_str);
		mTextRender->UpdateText({ 255,255,255 });
		mFrameCount = 0;
		mInitialTime = mFinalTime;
	}
}
