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
	MainMenuPawn(UIMenu* menu, GameObject* cursor);
	virtual ~MainMenuPawn();

	void Up()override;
	void Down()override;
	void Left()override {}
	void Right()override {}

	void Start()override;
	void Select()override;

	void AButtonDown()override;
	void AButtonUp()override;
	void BButtonDown()override;
	void BButtonUp()override;

	void MousePosition(float x, float y)override;

	virtual void MouseLeftClick()override;
	virtual void MouseLeftUnClick()override;
	virtual void MouseRightClick()override {}
	virtual void MouseRightUnClick()override {}

private:
	GameObject* mCursor;

	int mCurrentButton;

	bool mLeftMouseDown = false;
	Vector2D mMousePosition;
};