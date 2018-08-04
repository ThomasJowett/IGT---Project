#include "Button.h"



Button::Button(GLuint texture, Vector2D position, Vector2D size, const char* text)
	:GameObject(text, new Transform())
{	
	GetTransform()->mPosition = Vector3D(position.x, position.y, 10);

	mSprite = new Sprite(this, texture, size.x, size.y, 1, 3);
	AddComponent(mSprite);
	
	mCollisionBox = new Box2D(this, size.x, size.y, { 0,0 });
	AddComponent(mCollisionBox);

	mText = new TextRender(this, "Fonts/nokiafc22.ttf", 8);
	mText->UpdateText(text, { 255,255,255 }, 0, -size.y/3, CENTER);
	AddComponent(mText);
}

Button::Button(GLuint texture, Vector2D position, Vector2D size)
	:GameObject("Button", new Transform())
{
	GetTransform()->mPosition = Vector3D(position.x, position.y, 10);

	mSprite = new Sprite(this, texture, size.x, size.y, 1, 3);
	AddComponent(mSprite);

	mCollisionBox = new Box2D(this, size.x, size.y, { 0,0 });
	AddComponent(mCollisionBox);

	mText = nullptr;
}

Button::Button(GLuint texture, Vector2D position)
	:GameObject("CheckBox", new Transform())
{
	GetTransform()->mPosition = Vector3D(position.x, position.y, 10);

	mSprite = new Sprite(this, texture, 20, 20, 1, 4);
	AddComponent(mSprite);

	mCollisionBox = new Box2D(this, 20, 20, { 0,0 });
	AddComponent(mCollisionBox);

	mText = nullptr;
}

Button::~Button()
{
}

void Button::OnClicked()
{
	if (mButtonStatus == NORMAL || mButtonStatus == HOVERED)
	{
		mButtonStatus = CLICKED;
		mSprite->SetCurrentFrame(CLICKED);
		if(mText)
			mText->UpdateText({ 0, 0, 255 });
	}
}

void Button::Execute()
{
	if(mButtonStatus == CLICKED)
		Notify(ButtonEvent::ON_CLICKED, mButtonID);
}

void Button::OnHovered()
{
	if (mButtonStatus == NORMAL || mButtonStatus == CLICKED)
	{
		mButtonStatus = HOVERED;
		mSprite->SetCurrentFrame(HOVERED);
		if (mText)
			mText->UpdateText({ 255, 255, 255 });

		Notify(ButtonEvent::ON_HOVERED, mButtonID);
	}
}

void Button::OnUnHovered()
{
	if (mButtonStatus == HOVERED || mButtonStatus == CLICKED)
	{
		mButtonStatus = NORMAL;
		mSprite->SetCurrentFrame(NORMAL);
		if (mText)
			mText->UpdateText({ 255, 255, 255 });
	}
}
