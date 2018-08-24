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

	bool OnPressed() override;
	bool OnReleased() override;
	bool OnHovered()override;
	bool OnUnHovered()override;

private:
	TextRender* mText;
	Sprite * mSprite;
	
	SDL_Colour mNormalColour = { 65, 172, 223 };
	SDL_Colour mHoveredColour = { 255, 255, 255 };
	SDL_Colour mPressedColour = { 0, 0, 255 };
};
