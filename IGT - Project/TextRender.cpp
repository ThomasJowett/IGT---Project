#include "TextRender.h"
#include <iostream>
#include "Commons.h"

extern std::string gApplicationLocation;

TextRender::TextRender(GameObject* parent, const char * path, int pointSize)
	:iRenderable(parent)
{
	if (TTF_Init() != 0)
	{
		std::cerr << "TTF_OpenFont() Failed: " << TTF_GetError() << std::endl;
	}

	LoadFont(path, pointSize);
}

TextRender::TextRender(GameObject * parent, const char * path, int pointSize, const char * text, SDL_Colour textColour, int x, int y, ALIGNMENT alignment)
	:iRenderable(parent)
{
	if (TTF_Init() != 0)
	{
		std::cerr << "TTF_OpenFont() Failed: " << TTF_GetError() << std::endl;
	}

	LoadFont(path, pointSize);

	UpdateText(text, textColour, x, y, alignment);
}

TextRender::~TextRender()
{
	glDeleteTextures(1, &mTextureID);
	if (mMesh) delete mMesh;
	TTF_CloseFont(mFont);
}

void TextRender::LoadFont(const char * path, int pointSize)
{
	std::string full_path = gApplicationLocation + '/' + path;
	mPath = full_path.c_str();
	mPointSize = pointSize;

	mFont = TTF_OpenFont(full_path.c_str(), pointSize);
	if (mFont == nullptr)
	{
		std::cerr << "TTF_OpenFont()Failed: " << TTF_GetError() << std::endl;
	}
}

void TextRender::Render(Shader * shader)
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mTextureID);

	shader->Updatefloat4(1.0f, 1.0f, 1.0f, 1.0f);
	if(mMesh) mMesh->Draw();
	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

bool TextRender::UpdateText()
{
	if (!mFont)
		LoadFont("Fonts/nokiafc22.ttf", 8);

	if (mText)
	{
		SDL_Surface* surface = TTF_RenderText_Blended(mFont, mText, mTextColour);

		if (surface == nullptr)
		{
			std::cerr << "Could not create surface from text ERROR: " << TTF_GetError() << std::endl;
			return false;
		}


		if (!mTextureID)
			glGenTextures(1, &mTextureID);
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

		switch (mAlignment)
		{
		case CENTER:
			vertices[0] = Vertex(Vector3D(mOffset.x - (surface->w / 2), mOffset.y, 8), Vector2D(0, 1));
			vertices[1] = Vertex(Vector3D(mOffset.x + (surface->w / 2), mOffset.y, 8), Vector2D(1, 1));
			vertices[2] = Vertex(Vector3D(mOffset.x + (surface->w / 2), mOffset.y + surface->h, 8), Vector2D(1, 0));
			vertices[3] = Vertex(Vector3D(mOffset.x - (surface->w / 2), mOffset.y + surface->h, 8), Vector2D(0, 0));
			break;
		case LEFT:
			vertices[0] = Vertex(Vector3D(mOffset.x - surface->w, mOffset.y, 8), Vector2D(0, 1));
			vertices[1] = Vertex(Vector3D(mOffset.x, mOffset.y, 8), Vector2D(1, 1));
			vertices[2] = Vertex(Vector3D(mOffset.x, mOffset.y + surface->h, 8), Vector2D(1, 0));
			vertices[3] = Vertex(Vector3D(mOffset.x - surface->w, mOffset.y + surface->h, 8), Vector2D(0, 0));
			break;
		case RIGHT:
			vertices[0] = Vertex(Vector3D(mOffset.x, mOffset.y, 8), Vector2D(0, 1));
			vertices[1] = Vertex(Vector3D(mOffset.x + surface->w, mOffset.y, 8), Vector2D(1, 1));
			vertices[2] = Vertex(Vector3D(mOffset.x + surface->w, mOffset.y + surface->h, 8), Vector2D(1, 0));
			vertices[3] = Vertex(Vector3D(mOffset.x, mOffset.y + surface->h, 8), Vector2D(0, 0));;
			break;
		}

		if (mMesh) delete mMesh;

		mMesh = new Mesh(vertices, 4, indices, 6);

		SDL_FreeSurface(surface);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, 0);

		return true;
	}
	
	return false;
}

bool TextRender::UpdateText(const char * text)
{
	mText = text;
	return UpdateText();
}

bool TextRender::UpdateText(SDL_Colour textColour)
{
	if (textColour.r != mTextColour.r
		|| textColour.g != mTextColour.g
		|| textColour.b != mTextColour.b
		|| textColour.a != mTextColour.a)
	{
		mTextColour = textColour;
		return UpdateText();
	}
	else
	{
		return true;
	}
}

bool TextRender::UpdateText(int x, int y)
{
	if (x != mOffset.x || y != mOffset.y)
	{
		mOffset.x = (float)x;
		mOffset.y = (float)y;
		return UpdateText();
	}
	else
	{
		return true;
	}
}

bool TextRender::UpdateText(ALIGNMENT alignment)
{
	if (alignment != mAlignment)
	{
		mAlignment = alignment;
		return UpdateText();
	}
	else
		return true;
}

Component * TextRender::Clone()
{
	TextRender* newText = new TextRender(nullptr, mPath, mPointSize);
	newText->UpdateText(mText, mTextColour, (int)mOffset.x, (int)mOffset.y, mAlignment);
	return newText;
}

bool TextRender::UpdateText(const char * text, SDL_Colour textColour, int x, int y, ALIGNMENT alignment)
{
	if (text != mText || alignment != mAlignment
		|| x != mOffset.x || y != mOffset.y
		|| textColour.r != mTextColour.r
		|| textColour.g != mTextColour.g
		|| textColour.b != mTextColour.b
		|| textColour.a != mTextColour.a)
	{
		mText = text;
		mTextColour = textColour;
		mOffset = Vector2D((float)x, (float)y);
		mAlignment = alignment;
		return UpdateText();
	}
	else
	{
		return true;
	}
}
