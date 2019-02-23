#include "Slider.h"
#include <iostream>


Slider::Slider(const char * name, Vector2D anchorPoint, Vector2D offset, Vector2D barSize, GLuint barTexture, Vector2D thumbSize, GLuint thumbTexture, int value, int maxValue)
	:UIWidget(name, anchorPoint, offset)
{
	mBarWidth = barSize.x;
	mMaxValue = maxValue;

	AddComponent<Sprite>(barTexture, barSize.x, barSize.y);

	mThumbSprite = new Sprite(this, thumbTexture, thumbSize.x, thumbSize.y, 1, 2);
	AddComponent(mThumbSprite);

	mCollisionBox = new Box2D(this, barSize.x, thumbSize.y, { 0,0 });
	AddComponent(mCollisionBox);

	SetValue(value);
}

Slider::~Slider()
{
}

void Slider::OnDragged(Vector2D mousePosition)
{
	UIWidget::OnDragged(mousePosition);

	float mouseRelativePosition = (mousePosition.x - GetTransform()->mPosition.x) + (mBarWidth / 2);
	
	SetValue((int)(mouseRelativePosition/mBarWidth) * mMaxValue);
}

bool Slider::OnHovered()
{
	if (UIWidget::OnHovered())
	{
		mThumbSprite->SetCurrentFrame(HOVERED);
		return true;
	}

	return false;
}

bool Slider::OnUnHovered()
{
	if (UIWidget::OnUnHovered())
	{
		mThumbSprite->SetCurrentFrame(NORMAL);
		return true;
	}

	return false;
}

void Slider::SetValue(int value)
{
	if (value > mMaxValue)
	{
		mWidgetData.value = mMaxValue;
	}
	else if (value < 0)
	{
		mWidgetData.value = 0;
	}
	else
	{
		mWidgetData.value = value;
	}

	mThumbSprite->SetOffset(Vector2D((((float)mWidgetData.value/mMaxValue) * mBarWidth) - (mBarWidth / 2), 0));
}
