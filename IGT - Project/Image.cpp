#include "Image.h"
#include "Sprite.h"




Image::Image(const char * name, Vector2D anchorPoint, Vector2D offset, GLuint texture, Vector2D size)
	:UIWidget(name, anchorPoint, offset)
{
	AddComponent<Sprite>(texture, size.x, size.y);
}

Image::Image(const char * name, Vector2D anchorPoint, Vector2D offset, GLuint texture, Vector2D size, int tilesWide, int tilesTall)
	: UIWidget(name, anchorPoint, offset)
{
	AddComponent<Sprite>(texture, size.x, size.y, tilesWide, tilesTall);
}

Image::~Image()
{
}
