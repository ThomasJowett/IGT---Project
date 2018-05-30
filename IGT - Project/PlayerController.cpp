#include "PlayerController.h"


PlayerController::PlayerController(int controllerID, iInput* pawn)
	:mControllerID(controllerID), mPawn(pawn)
{
}


PlayerController::~PlayerController()
{
}

void PlayerController::Update(std::vector<SDL_Event> events)
{
	SDL_PumpEvents();
	const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);

	for (SDL_Event e : events)
	{
		if (e.adevice.which == mControllerID)
		{
			if (e.type == SDL_CONTROLLERAXISMOTION)
			{
				//Motion on controller
				if (e.caxis.axis == 0) { mLeftStickAxis_X = (e.caxis.value) / 32767.0f; }
				else if (e.caxis.axis == 1) { mLeftStickAxis_Y = (e.caxis.value) / 32767.0f; }
				else if (e.caxis.axis == 2) { mRightStickAxis_X = (e.caxis.value) / 32767.0f; }
				else if (e.caxis.axis == 3) { mRightStickAxis_Y = (e.caxis.value) / 32767.0f; }
				else if (e.caxis.axis == 4) { mLeftTrigger = (e.caxis.value) / 32767.0f; }
				else if (e.caxis.axis == 5) { mRightTrigger = (e.caxis.value) / 32767.0f; }
			}
			else if (e.type == SDL_CONTROLLERBUTTONDOWN)
			{
				switch (e.cbutton.button)
				{
				case SDL_CONTROLLER_BUTTON_A:
					mPawn->AButton();
					break;
				case SDL_CONTROLLER_BUTTON_B:
					mPawn->BButton();
					break;
				case SDL_CONTROLLER_BUTTON_X:
					mPawn->XButton();
					break;
				case SDL_CONTROLLER_BUTTON_Y:
					mPawn->YButton();
					break;
				case SDL_CONTROLLER_BUTTON_DPAD_UP:
					mPawn->Up();
					break;
				case SDL_CONTROLLER_BUTTON_DPAD_DOWN:
					mPawn->Down();
					break;
				case SDL_CONTROLLER_BUTTON_DPAD_LEFT:
					mPawn->Left();
					break;
				case SDL_CONTROLLER_BUTTON_DPAD_RIGHT:
					mPawn->Right();
					break;
				case SDL_CONTROLLER_BUTTON_LEFTSHOULDER:
					mPawn->LeftBumper();
					break;
				case SDL_CONTROLLER_BUTTON_RIGHTSHOULDER:
					mPawn->RightBumper();
					break;
				case SDL_CONTROLLER_BUTTON_START:
					mPawn->Start();
					break;
				case SDL_CONTROLLER_BUTTON_BACK:
					mPawn->Select();
					break;
				}
			}
		}
	}

	if ((mLeftStickAxis_X*mLeftStickAxis_X) + (mLeftStickAxis_Y*mLeftStickAxis_Y) > mJoystickDeadZone / 32767.0f)
	{
		mPawn->MoveRight(mLeftStickAxis_X);
		mPawn->MoveUp(-mLeftStickAxis_Y);
	}

	if ((mRightStickAxis_X*mLeftStickAxis_X) + (mRightStickAxis_Y*mLeftStickAxis_Y) > mJoystickDeadZone / 32767.0f)
	{
		mPawn->LookRight(mRightStickAxis_X);
		mPawn->LookUp(-mRightStickAxis_Y);
	}
}
