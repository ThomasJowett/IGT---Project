#include "Cursor.h"
#include <Windows.h>
#include <filesystem>
#include <SDL_image.h>
#include "Debug.h"
#include "Commons.h"

extern std::string gApplicationLocation;

void Cursor::LoadCursors()
{
	Cursor::CreateCustomCursor(SDL_SYSTEM_CURSOR_ARROW, gApplicationLocation + "/Images/Cursor_Arrow.png", 0, 0);
	Cursor::CreateCustomCursor(SDL_SYSTEM_CURSOR_CROSSHAIR, gApplicationLocation + "/Images/Cursor_Crosshairs.png", 32, 32);
	Cursor::CreateCustomCursor(SDL_SYSTEM_CURSOR_HAND, gApplicationLocation + "/Images/Cursor_Pointer.png", 0, 0);
}

//load from a .cur file
bool Cursor::CreateCustomCursor(SDL_SystemCursor cursorType, std::string filePath)
{
	HCURSOR CursorHandle = LoadCursorFromFileA(filePath.c_str());

	SetCursor(CursorHandle);

	ICONINFO info = { 0 };

	::GetIconInfo(CursorHandle, &info);

	SDL_Surface* surface = NULL;
	Uint8 *bits = NULL;
	Uint8 *temp = NULL;
	BITMAP bm;

	GetObject(info.hbmColor, sizeof(bm), &bm);

	surface = SDL_CreateRGBSurface(SDL_SWSURFACE, bm.bmWidth, bm.bmHeight, bm.bmBitsPixel, 0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000);

	bits = new Uint8[bm.bmWidthBytes * bm.bmHeight];
	temp = new Uint8[bm.bmWidthBytes * bm.bmHeight];

	memcpy(temp, bm.bmBits, bm.bmWidthBytes*bm.bmHeight);

	//unfortunately, both the vertical orientation and colordata are reversed
	//so now we will put them back in the right order

	//first flip image over
	//this is probably not the fastest/best way to do this
	Uint8 *ptemp;
	Uint8 *pbits = bits;
	for (int j = bm.bmHeight - 1; j >= 0; j--)
	{
		ptemp = temp + j * bm.bmWidthBytes;
		for (int x = 0; x < bm.bmWidthBytes; x++)
		{
			*pbits = *ptemp;
			pbits++;
			ptemp++;
		}
	}
	
	//Now reverse BGR data to be RGB
	//for (int i = 0; i < bm.bmWidthBytes*bm.bmHeight; i += 3)
	//{
	//	Uint8* temp;
	//	temp = bits;
	//	bits = &bits[i + 2];
	//	bits[i + 2] = temp;
	//}

	//Now just copy bits onto surface
	if (SDL_MUSTLOCK(surface)) SDL_LockSurface(surface);
	memcpy(surface->pixels, bits, bm.bmWidthBytes*bm.bmHeight);
	if (SDL_MUSTLOCK(surface)) SDL_UnlockSurface(surface);

	delete[] bits;
	delete[] temp;
	
	//set the pixels of the surface to the bitmap in info

	//gCursor = SDL_CreateColorCursor(surface, info.xHotspot, info.yHotspot);

	//SDL_SetCursor(gCursor);

	return false;
}

//load from a .png file
bool Cursor::CreateCustomCursor(SDL_SystemCursor cursorType, std::string filePath, int hot_x, int hot_y)
{
	SDL_Surface* pSurface = nullptr;

	pSurface = IMG_Load(filePath.c_str());

	if (!pSurface)
	{
		SDL_FreeSurface(pSurface);
		DBG_OUTPUT("Error: Could Not Load Cursor Image: %s\n", SDL_GetError());
		printf("Error: Could Not Load Cursor Image: %s\n", SDL_GetError());
		return false;
	}

	SDL_Rect targetDimensions;
	targetDimensions.x = 0;
	targetDimensions.y = 0;
	targetDimensions.w = (int)(64);
	targetDimensions.h = (int)(64);

	if (SDL_BlitSurface(pSurface, NULL, pSurface, NULL) < 0)
	{
		DBG_OUTPUT("Error: Did not blit surface: %s\n", SDL_GetError());
		printf("Error: Did not blit surface: %s\n", SDL_GetError());
		SDL_FreeSurface(pSurface);
		return false;
	}

	SDL_Surface *pScaleSurface = SDL_CreateRGBSurface(
		pSurface->flags,
		targetDimensions.w,
		targetDimensions.h,
		pSurface->format->BitsPerPixel,
		pSurface->format->Rmask,
		pSurface->format->Gmask,
		pSurface->format->Bmask,
		pSurface->format->Amask);

	SDL_FillRect(pScaleSurface, &targetDimensions, SDL_MapRGBA(pScaleSurface->format, 255, 0, 0, 0));

	if (SDL_BlitScaled(pSurface, NULL, pScaleSurface, NULL) < 0) {

		DBG_OUTPUT("Error: Did not scale surface: %s\n", SDL_GetError());
		printf("Error: Did not scale surface: %s\n", SDL_GetError());
		SDL_FreeSurface(pSurface);
		SDL_FreeSurface(pScaleSurface);
		pScaleSurface = NULL;
		return false;
	}

	SDL_Cursor* newCursor = nullptr;
	newCursor = SDL_CreateColorCursor(pScaleSurface, hot_x, hot_y);

	if (!newCursor)
	{
		SDL_FreeCursor(newCursor);
		return false;
	}

	SDL_FreeSurface(pSurface);
	SDL_FreeSurface(pScaleSurface);

	std::string name = Util::SplitString(filePath, '/').back();

	name = Util::SplitString(name, '.')[0];

	mCustomCursors.insert(std::pair<std::string, SDL_Cursor*>{name, newCursor});

	mCustomCursors[name] = newCursor;

	return true;
}

void Cursor::SystemCursorType(SDL_SystemCursor cursorType)
{
	if (!Cursor::mSystemCursors[cursorType])
		Cursor::mSystemCursors[cursorType] = SDL_CreateSystemCursor(cursorType);

	SDL_SetCursor(Cursor::mSystemCursors[cursorType]);
}

void Cursor::CustomCursorType(std::string name)
{
	if (mCustomCursors.find(name) != mCustomCursors.end())
		SDL_SetCursor(Cursor::mCustomCursors.at(name));
}

void Cursor::ReleaseCursors()
{
	for (SDL_Cursor* cursor : mSystemCursors)
	{
		if(cursor)
			SDL_FreeCursor(cursor);
	}
}
