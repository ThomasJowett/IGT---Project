#include "CheckBox.h"



CheckBox::CheckBox(const char* name, Vector2D anchorPoint, Vector2D offset, GLuint texture, Vector2D size, bool checked)
	:UIWidget(name, anchorPoint, offset)
{
	mWidgetData.value = checked;

	mSprite = new Sprite(this, texture, size.x, size.y, 1, 4);
	AddComponent(mSprite);

	mCollisionBox = new Box2D(this, size.x, size.y, { 0,0 });
	AddComponent(mCollisionBox);

	if (mWidgetData.value)
		mSprite->SetCurrentFrame(CHECKED_NORMAL);
}

CheckBox::~CheckBox()
{
}

bool CheckBox::OnReleased()
{
	if (UIWidget::OnReleased())
	{	
		if (mWidgetData.value == 1)
		{
			mSprite->SetCurrentFrame(UNCHECKED_HOVERED);
			mWidgetData.value = 0;
		}
		else
		{
			mSprite->SetCurrentFrame(CHECKED_HOVERED);
			mWidgetData.value = 1;
		}

		return true;
	}
	return false;
}

bool CheckBox::OnHovered()
{
	if (UIWidget::OnHovered())
	{
		if (mWidgetData.value)
			mSprite->SetCurrentFrame(CHECKED_HOVERED);
		else
			mSprite->SetCurrentFrame(UNCHECKED_HOVERED);
		return true;
	}

	return false;
}

bool CheckBox::OnUnHovered()
{
	if (UIWidget::OnUnHovered())
	{
		if (mWidgetData.value)
			mSprite->SetCurrentFrame(CHECKED_NORMAL);
		else
			mSprite->SetCurrentFrame(UNCHECKED_NORMAL);
		return true;
	}

	return false;
}