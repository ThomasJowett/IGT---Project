#pragma once
#include "UIWidget.h"
#include "Sprite.h"

class Slider :
	public UIWidget
{
public:
	Slider(const char* name, Vector2D anchorPoint, Vector2D offset, Vector2D barSize, GLuint barTexture, Vector2D thumbSize, GLuint thumbTexture, int initialvalue, int maxValue);
	virtual ~Slider();
	void OnDragged(Vector2D mousePosition)override;
	void SetValue(int value);

private:
	int mMaxValue;
	Sprite* mThumbSprite;

	float mBarWidth;
};

