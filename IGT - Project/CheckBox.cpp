#include "CheckBox.h"



CheckBox::CheckBox(GLuint texture, Vector2D position, Vector2D size, bool checked)
	:Button(texture, position)
{
	mChecked = checked;

	if (mChecked)
		mSprite->SetCurrentFrame(CHECKED_NORMAL);
}

CheckBox::~CheckBox()
{
}

void CheckBox::OnClicked()
{
	if (mButtonStatus == NORMAL || mButtonStatus == HOVERED)
	{
		mButtonStatus = CLICKED;
		if (mChecked)
			mSprite->SetCurrentFrame(UNCHECKED_HOVERED);
		else
			mSprite->SetCurrentFrame(CHECKED_HOVERED);
	}
}

void CheckBox::Execute()
{
	Button::Execute();
	mChecked = !mChecked;
}

void CheckBox::OnHovered()
{
	if (mButtonStatus == NORMAL || mButtonStatus == CLICKED)
	{
		mButtonStatus = HOVERED;
		if (mChecked)
			mSprite->SetCurrentFrame(CHECKED_HOVERED);
		else
			mSprite->SetCurrentFrame(UNCHECKED_HOVERED);

		Notify(ButtonEvent::ON_HOVERED, mButtonID);
	}
}

void CheckBox::OnUnHovered()
{
	if (mButtonStatus == HOVERED || mButtonStatus == CLICKED)
	{
		mButtonStatus = NORMAL;
		if (mChecked)
			mSprite->SetCurrentFrame(CHECKED_NORMAL);
		else
			mSprite->SetCurrentFrame(UNCHECKED_NORMAL);
	}
}