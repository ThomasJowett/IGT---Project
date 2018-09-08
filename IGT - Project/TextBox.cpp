#include "TextBox.h"

TextBox::TextBox(const char* name, Vector2D anchorPoint, Vector2D offset, const char * fontPath, int pointSize, const char * text, SDL_Colour textColour, ALIGNMENT alignment)
	:UIWidget(name, anchorPoint, offset)
{
	AddComponent<TextRender>(fontPath, pointSize, text, textColour, 0, 0, alignment);
	SetIsFocusable(false);
}

TextBox::~TextBox()
{
}
