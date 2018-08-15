#pragma once
#include "Button.h"
#include "UIWidget.h"

enum CheckBoxStatus
{
	UNCHECKED_NORMAL = 0,
	UNCHECKED_HOVERED,
	CHECKED_HOVERED,
	CHECKED_NORMAL
};

class CheckBox :
	public UIWidget
{
public:
	CheckBox(const char* name, Vector2D anchorPoint, Vector2D offset, GLuint texture, Vector2D size, bool checked);
	virtual ~CheckBox();

	bool OnReleased()override;
	bool OnHovered()override;
	bool OnUnHovered()override;

private:
	Sprite * mSprite;
};

