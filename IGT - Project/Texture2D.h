#ifndef  _TEXTURE2D_H
#define _TEXTURE2D_H
#include <Windows.h>
#include <SDL.h>
#include <SDL_image.h>
#include <vector>
#include <map>

#include "Commons.h"


class Texture2D
{
public:
	static GLuint LoadTexture2DRaw(const char* path, int width, int height);
	static void DeleteTexture(const char* path);
	static void DeleteAllTextures();
	static GLuint GetTexture2D(const char* path);

private:
	static GLuint LoadTexture2D(const char* path);
};


#endif // ! _TEXTURE2D_H