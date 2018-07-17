#pragma once
#include "iComponents.h"
#include "Sprite.h"
#include <vector>
#include <memory>

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

template<typename T>
class Animator
	:public iUpdateable
{
public:
	Animator(GameObject* parent)
		:iUpdateable(parent)
	{
		mSprite = GetParent()->GetComponent<Sprite>();
	}
	virtual ~Animator() 
	{ 
		mAnimations.clear(); 
	}

	virtual void Enter(T state) = 0;
	virtual void During(T state) = 0;
	virtual void Exit(T state) = 0;

	void ChangeState(T state)
	{
		if (state != mCurrentState)
		{
			mPreviousState = mCurrentState;
			if (mCurrentState)
				Exit(mCurrentState);
			mCurrentState = state;
			mTimeInCurrentState = 0.0f;
			Enter(mCurrentState);
		}
	}

	void RevertToPreviousState()
	{
		ChangeState(mPreviousState);
	}

	Component* Clone()override = 0;

	virtual void Update(float deltaTime) override
	{
		if (mCurrentState)
		{
			mTimeInCurrentState += deltaTime;
			During(mCurrentState);
		}

		mAnimations[(int)mCurrentState]->Update(deltaTime);
	}

	float GetTimeInCurrentState()const { return mTimeInCurrentState; }
	T GetState()const { return mCurrentState; }
	T GetPreviousState()const { return mCurrentState; }

	bool IsInState(T state)const
	{
		return (state == mCurrentState);
	}

private:
	T mCurrentState;
	T mPreviousState;

	float mTimeInCurrentState;

protected:
	Sprite* mSprite;
	std::vector<std::unique_ptr<Animation>> mAnimations;
};