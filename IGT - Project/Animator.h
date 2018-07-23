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

	float GetAnimationLength();
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
		if(GetParent())
			mSprite = GetParent()->GetComponent<Sprite>();
	}
	virtual ~Animator() 
	{ 
		mAnimations.clear(); 
	}

	virtual void Enter(T state) = 0;
	virtual void During(T state, float deltaTime) = 0;
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
		//If the animator has been cloned then it will not have had a parent during construction
		if (!mSprite)
		{
			mSprite = GetParent()->GetComponent<Sprite>();
			CreateAnimations();
		}

		mTimeInCurrentState += deltaTime;
		During(mCurrentState, deltaTime);
	}

	float GetTimeInCurrentState()const { return mTimeInCurrentState; }
	T GetState()const { return mCurrentState; }
	T GetPreviousState()const { return mPreviousState; }

	bool IsInState(T state)const
	{
		return (state == mCurrentState);
	}

	virtual void CreateAnimations() {};

private:
	T mCurrentState;
	T mPreviousState;

	float mTimeInCurrentState;

protected:
	Sprite* mSprite;
	std::vector<std::unique_ptr<Animation>> mAnimations;
};