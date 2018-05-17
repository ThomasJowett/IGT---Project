#include "TextRender.h"
#include <iostream>
#include "Commons.h"

TextRender::TextRender(const char * path, int pointSize)
{
	if (TTF_Init() != 0)
	{
		std::cerr << "TTF_OpenFont() Failed: " << TTF_GetError() << std::endl;
	}

	LoadFont(path, pointSize);
}

TextRender::~TextRender()
{
}

void TextRender::LoadFont(const char * path, int pointSize)
{
	mFont = TTF_OpenFont(path, pointSize);
	if (mFont == NULL)
	{
		std::cerr << "TTF_OpenFont()Failed: " << TTF_GetError() << std::endl;
	}
}

bool TextRender::DisplayText(const char * text, SDL_Colour textColour, int x, int y, ALIGNMENT alignment)
{
	if (!mFont)
		LoadFont("Fonts/nokiafc22.ttf", 24);

	GLuint texture;

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	SDL_Surface* surface = TTF_RenderText_Blended(mFont, text, textColour);
		
	if (surface == NULL)
	{
		std::cerr << "Could not create surface from text ERROR: " << TTF_GetError() << std::endl;
		return false;
	}

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);

	if (texture == NULL)
	{
		std::cerr << "Unable to create texture from surface. Error: " << SDL_GetError() << std::endl;
		return false;
	}
	
	SDL_Rect renderLocation;

	switch (alignment)
	{
	case CENTER:
		renderLocation = { x - (surface->w/2), y, surface->w, surface->h };
		break;
	case LEFT:
		renderLocation = { x,y,surface->w, surface->h };
		break;
	case RIGHT:
		renderLocation = { x - (surface->w), y, surface->w, surface->h };
		break;
	}

	SDL_FreeSurface(surface);
	glDeleteTextures(1, &texture);
	
	return true;
}
