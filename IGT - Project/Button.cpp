#include "Button.h"



Button::Button(GLuint texture, Vector2D anchorPoint, Vector2D offset, Vector2D size, const char* text)
	:UIWidget(text, anchorPoint, offset)
{
	mSprite = new Sprite(this, texture, size.x, size.y, 1, 3);
	AddComponent(mSprite);
	
	mCollisionBox = new Box2D(this, size.x, size.y, { 0,0 });
	AddComponent(mCollisionBox);

	mText = new TextRender(this, "Fonts/nokiafc22.ttf", 8);
	mText->UpdateText(text, { 255,255,255 }, 0, -size.y/3, CENTER);
	AddComponent(mText);
}

Button::Button(GLuint texture, Vector2D anchorPoint, Vector2D offset, Vector2D size)
	:UIWidget("Button", anchorPoint, offset)
{
	mSprite = new Sprite(this, texture, size.x, size.y, 1, 3);
	AddComponent(mSprite);

	mCollisionBox = new Box2D(this, size.x, size.y, { 0,0 });
	AddComponent(mCollisionBox);

	mText = nullptr;
}

Button::~Button()
{
}

bool Button::OnPressed()
{
	if (UIWidget::OnPressed())
	{
		mSprite->SetCurrentFrame(PRESSED);
		if (mText)
			mText->UpdateText({ 0, 0, 255 });

		return true;
	}

	return false;
}

bool Button::OnReleased()
{
	if (UIWidget::OnReleased())
	{
		mSprite->SetCurrentFrame(NORMAL);
		if (mText)
			mText->UpdateText({ 255, 255, 255 });

		return true;
	}

	return false;
}

bool Button::OnHovered()
{
	if (UIWidget::OnHovered())
	{
		mSprite->SetCurrentFrame(HOVERED);
		if (mText)
			mText->UpdateText({ 255, 255, 255 });
		return true;
	}

	return false;
}

bool Button::OnUnHovered()
{
	if (UIWidget::OnUnHovered())
	{
		mSprite->SetCurrentFrame(NORMAL);
		if (mText)
			mText->UpdateText({ 255, 255, 255 });
		return true;
	}

	return false;
}
