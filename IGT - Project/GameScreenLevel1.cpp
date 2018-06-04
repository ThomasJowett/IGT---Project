#include "GameScreenLevel1.h"
#include "Constants.h"


GameScreenLevel1::GameScreenLevel1()
{
	mShader = new BasicShader();
	mCamera.Orthographic(Vector3D(0, 0, 100), SCREEN_WIDTH / SCREEN_SCALE, SCREEN_HEIGHT / SCREEN_SCALE, 0, 1000);
}


GameScreenLevel1::~GameScreenLevel1()
{
}
