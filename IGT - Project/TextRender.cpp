#include "TextRender.h"
#include <iostream>
#include "Commons.h"

TextRender::TextRender(GameObject* parent, const char * path, int pointSize)
	:iRenderable(parent)
{
	if (TTF_Init() != 0)
	{
		std::cerr << "TTF_OpenFont() Failed: " << TTF_GetError() << std::endl;
	}

	LoadFont(path, pointSize);
}

TextRender::~TextRender()
{
	glDeleteTextures(1, &mTextureID);
	if (mMesh) delete mMesh;
}

void TextRender::LoadFont(const char * path, int pointSize)
{
	mPath = path;
	mPointSize = pointSize;

	mFont = TTF_OpenFont(path, pointSize);
	if (mFont == NULL)
	{
		std::cerr << "TTF_OpenFont()Failed: " << TTF_GetError() << std::endl;
	}
}

void TextRender::Render(Shader * shader)
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mTextureID);

	if(mMesh) mMesh->Draw();
	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

bool TextRender::UpdateText(const char * text, SDL_Colour textColour, int x, int y, ALIGNMENT alignment)
{
	if (!mFont)
		LoadFont("Fonts/nokiafc22.ttf", 8);

	if (text != mText || alignment != mAlignment 
		|| x != mOffset.x || y != mOffset.y
		|| textColour.r != mTextColour.r 
		|| textColour.g != mTextColour.g
		|| textColour.b != mTextColour.b
		|| textColour.a != mTextColour.a)
	{
		mText = text;
		mTextColour = textColour;
		mOffset = Vector2D( x,y );
		mAlignment = alignment;
	}
	else
	{
		return true;
	}

	SDL_Surface* surface = TTF_RenderText_Blended(mFont, text, textColour);

	if (surface == NULL)
	{
		std::cerr << "Could not create surface from text ERROR: " << TTF_GetError() << std::endl;
		return false;
	}

	if(!mTextureID)
		glGenTextures( 1, &mTextureID);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mTextureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);

	if (mTextureID == NULL)
	{
		std::cerr << "Unable to create texture from surface. Error: " << SDL_GetError() << std::endl;
		return false;
	}

	unsigned int indices[] =
	{
		0,1,2,
		0,2,3
	};

	Vertex vertices[4];

	switch (alignment)
	{
	case CENTER:
		vertices[0] = Vertex(Vector3D(x - (surface->w / 2), y, 8), Vector2D(0, 1));
		vertices[1] = Vertex(Vector3D(x + (surface->w / 2), y, 8), Vector2D(1, 1));
		vertices[2] = Vertex(Vector3D(x + (surface->w / 2), y + surface->h, 8), Vector2D(1, 0));
		vertices[3] = Vertex(Vector3D(x - (surface->w / 2), y + surface->h, 8), Vector2D(0, 0));
		break;
	case LEFT:
		vertices[0] = Vertex(Vector3D(x - surface->w, y, 8), Vector2D(0, 1));
		vertices[1] = Vertex(Vector3D(x, y, 8), Vector2D(1, 1));
		vertices[2] = Vertex(Vector3D(x, y + surface->h, 8), Vector2D(1, 0));
		vertices[3] = Vertex(Vector3D(x - surface->w, y + surface->h, 8), Vector2D(0, 0));
		break;
	case RIGHT:
		vertices[0] = Vertex(Vector3D(x, y, 8), Vector2D(0, 1));
		vertices[1] = Vertex(Vector3D(x + surface->w, y, 8), Vector2D(1, 1));
		vertices[2] = Vertex(Vector3D(x + surface->w, y + surface->h, 8), Vector2D(1, 0));
		vertices[3] = Vertex(Vector3D(x, y + surface->h, 8), Vector2D(0, 0));;
		break;
	}

	if (mMesh) delete mMesh;

	mMesh = new Mesh(vertices, 4, indices, 6);

	SDL_FreeSurface(surface);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);

	return true;
}

bool TextRender::UpdateText(const char * text)
{
	return UpdateText(text, mTextColour, mOffset.x, mOffset.y, mAlignment);
}

bool TextRender::UpdateText(SDL_Colour textColour)
{
	return UpdateText(mText, textColour, mOffset.x, mOffset.y, mAlignment);
}

bool TextRender::UpdateText(int x, int y)
{
	return UpdateText(mText, mTextColour, x, y, mAlignment);
}

bool TextRender::UpdateText(ALIGNMENT alignment)
{
	return UpdateText(mText, mTextColour, mOffset.x, mOffset.y, alignment);
}

Component * TextRender::Clone()
{
	return new TextRender(nullptr, mPath, mPointSize);
}
