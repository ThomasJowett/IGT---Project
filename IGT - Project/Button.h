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
	~Button();

	void OnClicked();
	void Execute();
	void OnHovered();
	void OnUnHovered();

	Box2D* GetCollisionBox() { return mCollisionBox; }

	void Render(Shader* shader);

	int mButtonID;

private:
	Sprite* mSprite;
	Box2D* mCollisionBox;
	TextRender* mText;

	Status mButtonStatus = NORMAL;
};

