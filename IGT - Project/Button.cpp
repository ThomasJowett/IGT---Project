#include "Button.h"



Button::Button(GLuint texture, Vector2D position, Vector2D size, const char* text)
{
	mSprite = new Sprite(this, texture, size.x, size.y, 1, 3);
	GetTransform()->mPosition = Vector3D(position.x, position.y, 10);
	mCollisionBox = new Box2D(this, size.x, size.y, { 0,0 });
	mText = new TextRender(this, "Fonts/nokiafc22.ttf", 8);
	mText->UpdateText(text, { 255,255,255 }, 0, -size.y/3, CENTER);
}

Button::~Button()
{
	delete mSprite;
	delete mCollisionBox;
}

void Button::OnClicked()
{
	if (mButtonStatus == NORMAL || mButtonStatus == HOVERED)
	{
		mButtonStatus = CLICKED;
		mSprite->SetCurrentFrame(CLICKED);
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
		mText->UpdateText({ 255, 255, 255 });
	}
}

void Button::Render(Shader * shader)
{
	GetTransform()->UpdateWorldMatrix();
	shader->UpdateWorld(GetTransform());

	mSprite->Render(shader);
	mText->Render(shader);
}
