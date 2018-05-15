#ifndef _TEXTRENDER_H
#define _TEXTRENDER_H

#include <SDL_ttf.h>

enum ALIGNMENT
{
	CENTER = 0,
	RIGHT,
	LEFT
};

class TextRender
{
public:
	TextRender(const char* path, int pointSize);
	~TextRender();

	void LoadFont(const char* path, int pointSize);
	bool DisplayText(const char* text, SDL_Colour textColour, int x, int y, ALIGNMENT alignment);

private:
	TTF_Font * mFont;
};

#endif // !_TEXTRENDER_H