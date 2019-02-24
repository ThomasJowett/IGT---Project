#include "PlayerController.h"
#include "Settings.h"


PlayerController::PlayerController(int controllerID, iInput* pawn)
	:mControllerID(controllerID), mPawn(pawn)
{
	if (mPawn)
		mPawn->OnPossessed(mControllerID);
}


PlayerController::~PlayerController()
{
	if (mPawn)
		mPawn->OnUnPossesed();
}

void PlayerController::Update(std::vector<SDL_Event> events)
{
	if (!mPawn)
		return;

	SDL_PumpEvents();
	const Uint8* currentKeyStates = SDL_GetKeyboardState(nullptr);

	for (SDL_Event e : events)
	{
		if (e.adevice.which == mControllerID)
		{
			if (e.type == SDL_CONTROLLERAXISMOTION)
			{
				//Motion on controller
				if (e.caxis.axis == 0) { mLeftStick.x = (e.caxis.value) / 32767.0f; }
				else if (e.caxis.axis == 1) { mLeftStick.y = -((e.caxis.value) / 32767.0f); }
				else if (e.caxis.axis == 2) { mRightStick.x = (e.caxis.value) / 32767.0f; }
				else if (e.caxis.axis == 3) { mRightStick.y = (e.caxis.value) / 32767.0f; }
				else if (e.caxis.axis == 4) { mLeftTrigger = ((e.caxis.value) / 32767.0f); }
				else if (e.caxis.axis == 5) { mRightTrigger = ((e.caxis.value) / 32767.0f); }
			}
			else if (e.type == SDL_CONTROLLERBUTTONDOWN)
			{
				switch (e.cbutton.button)
				{
				case SDL_CONTROLLER_BUTTON_A:
					mPawn->AButtonDown();
					break;
				case SDL_CONTROLLER_BUTTON_B:
					mPawn->BButtonDown();
					break;
				case SDL_CONTROLLER_BUTTON_X:
					mPawn->XButtonDown();
					break;
				case SDL_CONTROLLER_BUTTON_Y:
					mPawn->YButtonDown();
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
					if (!mPawn)
						return;
					break;
				case SDL_CONTROLLER_BUTTON_BACK:
					mPawn->Select();
					break;
				}
			}
			else if (e.type == SDL_CONTROLLERBUTTONUP)
			{
				switch (e.cbutton.button)
				{
				case SDL_CONTROLLER_BUTTON_A:
					mPawn->AButtonUp();
					break;
				case SDL_CONTROLLER_BUTTON_B:
					mPawn->BButtonUp();
					break;
				case SDL_CONTROLLER_BUTTON_X:
					mPawn->XButtonUp();
					break;
				case SDL_CONTROLLER_BUTTON_Y:
					mPawn->YButtonUp();
					break;
				}
			}
		}

		if (mControllerID == 0)
		{
			if (e.type == SDL_KEYDOWN)
			{
				switch (e.key.keysym.sym)
				{
				case SDLK_UP:
					mPawn->Up();
					break;
				case SDLK_DOWN:
					mPawn->Down();
					break;
				case SDLK_LEFT:
					mPawn->Left();
					break;
				case SDLK_RIGHT:
					mPawn->Right();
					break;
				case SDLK_RETURN:
					mPawn->AButtonDown();
					break;
				case SDLK_BACKSPACE:
					mPawn->BButtonDown();
					break;
				case SDLK_ESCAPE:
					mPawn->Start();
					if (!mPawn)
						return;
					break;
				case SDLK_TAB:
					mPawn->Select();
					break;
				default:
					break;
				}
			}

			else if (e.type == SDL_KEYUP)
			{
				switch (e.key.keysym.sym)
				{
				case SDLK_RETURN:
					mPawn->AButtonUp();
					break;
				case SDLK_BACKSPACE:
					mPawn->BButtonUp();
					break;
				default:
					break;
				}
			}

			else if (e.type == SDL_MOUSEMOTION)
			{
				//Adjust raw mouse position to screen space
				mPawn->MousePosition((float)(e.motion.x / Settings::GetInstance()->GetScreenScale() - ((Settings::GetInstance()->GetScreenWidth() / Settings::GetInstance()->GetScreenScale()) / 2)),
					(float)(Settings::GetInstance()->GetScreenHeight() / Settings::GetInstance()->GetScreenScale() - (e.motion.y / Settings::GetInstance()->GetScreenScale() + (Settings::GetInstance()->GetScreenHeight() / (float)Settings::GetInstance()->GetScreenScale() / 2))));

				mPawn->LookRight((float)e.motion.xrel);
				mPawn->LookUp((float)e.motion.yrel);
			}

			else if (e.type == SDL_MOUSEBUTTONDOWN)
			{
				switch (e.button.button)
				{
				case SDL_BUTTON_LEFT:
					mPawn->MouseLeftClick();
					break;
				case SDL_BUTTON_RIGHT:
					mPawn->MouseRightClick();
					break;
				}
			}
			else if (e.type == SDL_MOUSEBUTTONUP)
			{
				switch (e.button.button)
				{
				case SDL_BUTTON_LEFT:
					mPawn->MouseLeftUnClick();
					break;
				case SDL_BUTTON_RIGHT:
					mPawn->MouseRightUnClick();
					break;
				}
			}
		}
	}

	if (mControllerID == 0)
	{
		if (currentKeyStates[SDL_SCANCODE_A]) { mPawn->MoveRight(-1.0f); }
		if (currentKeyStates[SDL_SCANCODE_D]) { mPawn->MoveRight(1.0f); }
		if (currentKeyStates[SDL_SCANCODE_W]) { mPawn->MoveUp(1.0f); }
		if (currentKeyStates[SDL_SCANCODE_S]) { mPawn->MoveUp(-1.0f); }
		if (currentKeyStates[SDL_SCANCODE_SPACE]) { mPawn->AButtonDown(); }
		if (currentKeyStates[SDL_SCANCODE_1]) { mPawn->BButtonDown(); }
		if (currentKeyStates[SDL_SCANCODE_2]) { mPawn->XButtonDown(); }
		if (currentKeyStates[SDL_SCANCODE_3]) { mPawn->YButtonDown(); }
		if (currentKeyStates[SDL_SCANCODE_LSHIFT]) { mPawn->BButtonDown(); }
		if (currentKeyStates[SDL_SCANCODE_R]) { mPawn->XButtonDown(); }
		if (currentKeyStates[SDL_SCANCODE_F]) { mPawn->YButtonDown(); }
		if (currentKeyStates[SDL_SCANCODE_Q]) { mPawn->LeftBumper(); }
		if (currentKeyStates[SDL_SCANCODE_E]) { mPawn->RightBumper(); }

		//Switch to wire Frame mode for debugging
		if (currentKeyStates[SDL_SCANCODE_F1]) { glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); }
		if (currentKeyStates[SDL_SCANCODE_F2]) { glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); }
	}

	//Deal with movement

	if (mLeftStick.SqrMagnitude() > mJoystickDeadZone)
	{
		mPawn->MoveRight(mLeftStick.x);
		mPawn->MoveUp(mLeftStick.y);
	}

	if (mRightStick.SqrMagnitude() > mJoystickDeadZone)
	{
		mPawn->LookRight(mRightStick.x);
		mPawn->LookUp(-mRightStick.y);
	}

	mPawn->LeftTrigger(mLeftTrigger);
	mPawn->RightTrigger(mRightTrigger);
}

void PlayerController::PossesPawn(iInput * pawn)
{
	if (pawn != mPawn && mPawn)
	{
		mPawn->OnUnPossesed();
		mPawn = pawn;
	}
}

void PlayerController::UnPossesPawn()
{
	if(mPawn)
		mPawn->OnUnPossesed();
	mPawn = nullptr;
}
