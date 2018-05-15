#include "TextRender.h"
#include <iostream>

extern SDL_Renderer* gRenderer;

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

	SDL_Surface* surface = TTF_RenderText_Solid(mFont, text, textColour);
		
	if (surface == NULL)
	{
		std::cerr << "TTF_RendererText_Solid() Failed: " << TTF_GetError() << std::endl;
		return false;
	}

	SDL_Texture* texture = SDL_CreateTextureFromSurface(gRenderer, surface);

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
	

	//Render to Screen
	SDL_RenderCopyEx(gRenderer, texture, NULL, &renderLocation, 0.0f, NULL, SDL_FLIP_NONE);

	SDL_FreeSurface(surface);
	SDL_DestroyTexture(texture);

	return true;
}
