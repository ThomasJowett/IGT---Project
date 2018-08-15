#include "UIWidget.h"
#include "Settings.h"
#include <iostream>


UIWidget::UIWidget(const char* name, Vector2D anchorPoint, Vector2D offset)
	:GameObject(name, new Transform()), mAnchorPoint(anchorPoint), mOffset(offset)
{
	//Find corners of screen and set transform to be the correct place

	CalculateScreenPosition(Vector2D(Settings::GetInstance()->GetScreenWidth(), Settings::GetInstance()->GetScreenHeight()));

	Settings::GetInstance()->AddObserver(this);

	mIsFocusable = true;
}


UIWidget::~UIWidget()
{
	Settings::GetInstance()->RemoveObserver(this);
}

bool UIWidget::OnPressed()
{
	if (mWidgetStatus == NORMAL || mWidgetStatus == HOVERED)
	{
		mWidgetStatus = PRESSED;

		Notify(WidgetEvent::ON_PRESSED, mWidgetData);

		return true;
	}
	
	return false;
}

bool UIWidget::OnReleased()
{
	if (mWidgetStatus == PRESSED)
	{
		mWidgetStatus = NORMAL;
		Notify(WidgetEvent::ON_CLICKED, mWidgetData);
		return true;
	}

	return false;
}

bool UIWidget::OnHovered()
{
	if (mWidgetStatus == NORMAL)
	{
		mWidgetStatus = HOVERED;

		Notify(WidgetEvent::ON_HOVERED, mWidgetData);

		return true;
	}

	return false;
}

bool UIWidget::OnUnHovered()
{
	if (mWidgetStatus == HOVERED || mWidgetStatus == PRESSED)
	{
		mWidgetStatus = NORMAL;

		Notify(WidgetEvent::ON_UNHOVERED, mWidgetData);

		return true;
	}

	return false;
}

void UIWidget::OnDragged(Vector2D mousePosition)
{
	Notify(WidgetEvent::ON_DRAGGED, mWidgetData);
}

bool UIWidget::ContainsMouse(Vector2D mousePosition)
{
	if (mCollisionBox)
		return mCollisionBox->ContainsPoint(mousePosition);

	return false;
}

void UIWidget::OnNotify(SettingsEvent event, Vector2D data)
{
	switch (event)
	{
	case SettingsEvent::ON_RESOLUTION_CHANGE:
		CalculateScreenPosition(data);
		break;
	default:
		break;
	}
}

void UIWidget::CalculateScreenPosition(Vector2D resolution)
{
	float screenScale = Settings::GetInstance()->GetScreenScale();
	float screenWidth = resolution.x / screenScale;
	float screenHeight = resolution.y / screenScale;

	GetTransform()->mPosition.x = ((screenWidth * mAnchorPoint.x) - (screenWidth / 2)) + mOffset.x;
	GetTransform()->mPosition.y = ((screenHeight * (1 - mAnchorPoint.y)) - (screenHeight / 2)) + mOffset.y;
}
