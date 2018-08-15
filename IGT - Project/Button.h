#pragma once

#include "Collider.h"
#include "Sprite.h"
#include "Vector.h"
#include "TextRender.h"
#include "Messaging.h"
#include "UIWidget.h"

enum class ButtonEvent { ON_HOVERED, ON_CLICKED};

class Button
	:public UIWidget
{
public:
	Button(GLuint texture, Vector2D anchorPoint, Vector2D offset, Vector2D size, const char* text);
	Button(GLuint texture, Vector2D anchorPoint, Vector2D offset, Vector2D size);
	~Button();

	virtual bool OnPressed() override;
	virtual bool OnReleased() override;
	virtual bool OnHovered()override;
	virtual bool OnUnHovered()override;

private:
	TextRender* mText;

protected:
	Sprite * mSprite;
};
