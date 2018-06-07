#ifndef _TEXTRENDER_H
#define _TEXTRENDER_H

#include <SDL_ttf.h>
#include "Mesh.h"
#include "iComponents.h"

enum ALIGNMENT
{
	CENTER = 0,
	RIGHT,
	LEFT
};

class TextRender: public iRenderable
{
public:
	TextRender(GameObject* parent, const char* path, int pointSize);
	~TextRender();

	void LoadFont(const char* path, int pointSize);
	void Render(Shader* shader) override;
	bool UpdateText(const char* text, SDL_Colour textColour, int x, int y, ALIGNMENT alignment);
	bool UpdateText(const char* text);
	bool UpdateText(SDL_Colour textColour);
	bool UpdateText(int x, int y);
	bool UpdateText(ALIGNMENT alignment);

	const char* GetText() { return mText; }

	Component* Clone()override;

private:
	TTF_Font * mFont;
	const char* mText;
	SDL_Colour mTextColour;
	Vector2D mOffset;
	ALIGNMENT mAlignment;

	GLuint mTextureID;
	Mesh* mMesh;

	const char* mPath;
	int mPointSize;
};

#endif // !_TEXTRENDER_H