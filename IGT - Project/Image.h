#pragma once
#include "UIWidget.h"
class Image :
	public UIWidget
{
public:
	Image(const char* name, Vector2D anchorPoint, Vector2D offset, GLuint texture, Vector2D size);
	Image(const char* name, Vector2D anchorPoint, Vector2D offset, GLuint texture, Vector2D size, int tilesWide, int tilesTall);
	virtual ~Image();
};

