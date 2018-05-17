#ifndef  _TEXTURE2D_H
#define _TEXTURE2D_H
#include <Windows.h>
#include <SDL.h>
#include <SDL_image.h>


#include "Commons.h"


class Texture2D
{

public:
	Texture2D();
	~Texture2D();

	static GLuint LoadTexture2D(const char* path);
	static GLuint LoadTexture2DRaw(const char* path, int width, int height);
};


#endif // ! _TEXTURE2D_H