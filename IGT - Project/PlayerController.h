#ifndef _PLAYERCONTROLLER_H
#define _PLAYERCONTROLLER_H

#include <SDL.h>
#include <vector>

class PlayerController
{
public:
	PlayerController(int controllerID);
	~PlayerController();

	virtual void Update(std::vector<SDL_Event> events);
private:
	int mControllerID;

	const int mJoystickDeadZone = 2000;

	float mLeftStickAxis_X = 0.0f;
	float mLeftStickAxis_Y = 0.0f;

	float mRightStickAxis_X = 0.0f;
	float mRightStickAxis_Y = 0.0f;
};

#endif // !_PLAYERCONTROLLER_H