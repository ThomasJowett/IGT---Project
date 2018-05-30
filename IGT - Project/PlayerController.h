#ifndef _PLAYERCONTROLLER_H
#define _PLAYERCONTROLLER_H
#include "iInput.h"

#include <SDL.h>
#include <vector>

class PlayerController
{
public:
	PlayerController(int controllerID, iInput* pawn);
	~PlayerController();

	virtual void Update(std::vector<SDL_Event> events);
private:
	iInput * mPawn;
	int mControllerID;

	const int mJoystickDeadZone = 2000;

	float mLeftStickAxis_X = 0.0f;
	float mLeftStickAxis_Y = 0.0f;

	float mRightStickAxis_X = 0.0f;
	float mRightStickAxis_Y = 0.0f;

	float mRightTrigger = 0.0f;
	float mLeftTrigger = 0.0f;
};

#endif // !_PLAYERCONTROLLER_H