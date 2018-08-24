#pragma once
#include "UIWidget.h"
#include "TextRender.h"

class TextBox
	:public UIWidget
{
public:
	TextBox(const char* name, Vector2D anchorPoint, Vector2D offset, 
		const char * fontPath, int pointSize, const char * text, SDL_Colour textColour, ALIGNMENT alignment);
	~TextBox();
};
