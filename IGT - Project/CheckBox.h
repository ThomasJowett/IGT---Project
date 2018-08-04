#pragma once
#include "Button.h"

enum CheckBoxStatus
{
	UNCHECKED_NORMAL = 0,
	UNCHECKED_HOVERED,
	CHECKED_HOVERED,
	CHECKED_NORMAL
};

class CheckBox :
	public Button
{
public:
	CheckBox(GLuint texture, Vector2D position, Vector2D size, bool checked);
	virtual ~CheckBox();

	void OnClicked()override;
	void Execute()override;
	void OnHovered()override;
	void OnUnHovered()override;

private:
	bool mChecked;
};

