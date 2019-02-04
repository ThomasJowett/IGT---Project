#include "Button.h"
#include "Cursor.h"


Button::Button(GLuint texture, Vector2D anchorPoint, Vector2D offset, Vector2D size, const char* text)
	:UIWidget(text, anchorPoint, offset)
{
	mSprite = new Sprite(this, texture, size.x, size.y, 1, 3);
	AddComponent(mSprite);
	
	mCollisionBox = new Box2D(this, size.x, size.y, { 0,0 });
	AddComponent(mCollisionBox);

	mText = new TextRender(this, "Fonts/nokiafc22.ttf", 8);
	mText->UpdateText(text, mNormalColour, 0, -size.y/3, CENTER);
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
			mText->UpdateText(mPressedColour);

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
			mText->UpdateText(mNormalColour);

		return true;
	}

	return false;
}

bool Button::OnHovered()
{
	if (UIWidget::OnHovered())
	{
		Cursor::CustomCursorType("Cursor_Pointer");
		mSprite->SetCurrentFrame(HOVERED);
		if (mText)
			mText->UpdateText(mHoveredColour);
		return true;
	}

	return false;
}

bool Button::OnUnHovered()
{
	if (UIWidget::OnUnHovered())
	{
		Cursor::CustomCursorType("Cursor_Arrow");
		mSprite->SetCurrentFrame(NORMAL);
		if (mText)
			mText->UpdateText(mNormalColour);
		return true;
	}

	return false;
}
