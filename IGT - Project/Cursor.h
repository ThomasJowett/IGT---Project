#pragma once
#include <SDL.h>
#include <string>
#include <map>

namespace Cursor
{
	void LoadCursors();

	bool CreateCustomCursor(SDL_SystemCursor cursorType, std::string filePath);

	bool CreateCustomCursor(SDL_SystemCursor cursorType, std::string filePath, int hot_x, int hot_y);

	void SystemCursorType(SDL_SystemCursor cursorType);

	void CustomCursorType(std::string name);

	void ReleaseCursors();

	static SDL_Cursor* mSystemCursors[SDL_NUM_SYSTEM_CURSORS] = {};

	static std::map<std::string, SDL_Cursor*> mCustomCursors;

	//TODO: resize cursors when screen size changes
}