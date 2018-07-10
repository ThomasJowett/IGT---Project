#pragma once
#include "iComponents.h"
#include "Sprite.h"
#include <vector>

class Animation;

class Animator :
	public iUpdateable
{
public:
	Animator(GameObject* parent, int animationCount);
	virtual ~Animator();

	void Update(float deltatime)override;
	Component* Clone()override;
	
private:
	Animation * mCurrentAnimation;
	Sprite* mSprite;
};

class Animation
{
public:
	Animation(int startFrame, int frameCount, float holdTime, Sprite* sprite);
	void Update(float deltaTime);
private:
	void Advance();
	Sprite* mSprite;
	int mStartFrame;
	int mFrameCount;
	int mCurrentFrame;
	float mHoldTime;
	float mCurrentFrameTime = 0.0f;
};