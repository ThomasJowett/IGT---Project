#ifndef _TEXTRENDER_H
#define _TEXTRENDER_H

#include <SDL_ttf.h>
#include "Mesh.h"
#include "iComponents.h"

#define CLASS_DECLARATION( classname )                                                      \
public:                                                                                     \
    static const std::size_t Type;                                                          \
    virtual bool IsClassType( const std::size_t classType ) const override;                 \

enum ALIGNMENT
{
	CENTER = 0,
	RIGHT,
	LEFT
};

class TextRender: public iRenderable
{
	CLASS_DECLARATION(TextRender)
public:
	TextRender(const char* path, int pointSize);
	~TextRender();

	void LoadFont(const char* path, int pointSize);
	void Render(Shader* shader) override;
	bool UpdateText(const char* text, SDL_Colour textColour, int x, int y, ALIGNMENT alignment);

	const char* GetText() { return mText; }

private:
	TTF_Font * mFont;
	const char* mText;
	SDL_Colour mTextColour;
	ALIGNMENT mAlignment;

	GLuint mTextureID;
	Mesh* mMesh;
};

#endif // !_TEXTRENDER_H