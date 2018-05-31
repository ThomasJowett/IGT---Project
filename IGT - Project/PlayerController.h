#ifndef _PLAYERCONTROLLER_H
#define _PLAYERCONTROLLER_H
#include "iInput.h"

#include <SDL.h>
#include <vector>
#include "Vector.h"

class PlayerController
{
public:
	PlayerController(int controllerID, iInput* pawn);
	~PlayerController();

	virtual void Update(std::vector<SDL_Event> events);
private:
	iInput * mPawn;
	int mControllerID;

	const float mJoystickDeadZone = 0.2f;

	Vector2D mLeftStick;
	Vector2D mRightStick;

	float mLeftTrigger = 0.0f;
	float mRightTrigger = 0.0f;
};

#endif // !_PLAYERCONTROLLER_H