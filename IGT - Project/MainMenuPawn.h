#pragma once
#include "iInput.h"
#include "Button.h"
#include <vector>
#include "GameObject.h"
#include "UIMenu.h"
#include "Messaging.h"
#include "MenuManager.h"

class MainMenuPawn :
	public iInput
{
public:
	MainMenuPawn();
	virtual ~MainMenuPawn();

	void Up()override;
	void Down()override;
	void Left()override;
	void Right()override;

	void Start()override;
	void Select()override;

	void AButtonDown()override;
	void AButtonUp()override;
	void BButtonDown()override;
	void BButtonUp()override;

	void RightBumper()override;
	void LeftBumper()override;

	void MousePosition(float x, float y)override;

	virtual void MouseLeftClick()override;
	virtual void MouseLeftUnClick()override;
	virtual void MouseRightClick()override {}
	virtual void MouseRightUnClick()override {}

	virtual void OnPossessed(int controllerID)override;
	virtual void OnUnPossesed()override;

private:

	bool mLeftMouseDown = false;
	Vector2D mMousePosition;
};