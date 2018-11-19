#include "Animator.h"

Animation::Animation(int startFrame, int frameCount, float holdTime, Sprite* sprite)
{	
	mSprite = sprite;

	if (mStartFrame > mSprite->GetNumberOfFrames())
	{
		mStartFrame = 0;
		std::cerr << "Error: Animation start frame is after sprites last tile\n";
	}
	else
		mStartFrame = startFrame;

	mCurrentFrame = mStartFrame;
	if (frameCount + mStartFrame > mSprite->GetNumberOfFrames())
	{
		std::cerr << "Error: Animation has more frames than sprite has Tiles\n";
		mFrameCount = 1;
	}
	else
		mFrameCount = frameCount;

	mHoldTime = holdTime;

	mSprite->SetCurrentFrame(mCurrentFrame);
}

void Animation::Update(float deltaTime)
{
	mCurrentFrameTime += deltaTime;
	while (mCurrentFrameTime >= mHoldTime)
	{
		Advance();
		mCurrentFrameTime -= mHoldTime;
	}
}

float Animation::GetAnimationLength()
{
	return mHoldTime * mFrameCount;
}

void Animation::Advance()
{
	if (++mCurrentFrame >= mStartFrame + mFrameCount)
	{
		mCurrentFrame = mStartFrame;
		Notify(AnimationNotify::ANIM_END, 0);
	}

	mSprite->SetCurrentFrame(mCurrentFrame);
}
