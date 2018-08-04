#pragma once

#include "Collider.h"
#include "Sprite.h"
#include "Vector.h"
#include "TextRender.h"
#include "Messaging.h"

enum Status
{
	NORMAL = 0,
	HOVERED,
	CLICKED
};

enum class ButtonEvent { ON_HOVERED, ON_CLICKED};

class Button
	:public Subject<ButtonEvent, int> , public GameObject
{
public:
	Button(GLuint texture, Vector2D position, Vector2D size, const char* text);
	Button(GLuint texture, Vector2D position, Vector2D size);
	Button(GLuint texture, Vector2D position);
	~Button();

	virtual void OnClicked();
	virtual void Execute();
	virtual void OnHovered();
	virtual void OnUnHovered();

	Box2D* GetCollisionBox() { return mCollisionBox; }

	int mButtonID;

private:
	Box2D* mCollisionBox;
	TextRender* mText;

protected:
	Sprite * mSprite;
	Status mButtonStatus = NORMAL;
};
