#pragma once
#include "iComponents.h"
#include "Sprite.h"
#include <vector>
#include <memory>
#include "Messaging.h"

enum class AnimationNotify {ANIM_END};

class Animation
	:public Subject<AnimationNotify, int>
{
public:
	Animation(int startFrame, int frameCount, float holdTime, Sprite* sprite);
	void Update(float deltaTime);

	float GetAnimationLength();
private:
	void Advance();
	Sprite* mSprite;
	unsigned int mStartFrame;
	unsigned int mFrameCount;
	unsigned int mCurrentFrame;
	float mHoldTime;
	float mCurrentFrameTime = 0.0f;
};

template<typename T>
class Animator
	:public iUpdateable, public Observer<AnimationNotify, int>
{
public:
	Animator(GameObject* parent)
		:iUpdateable(parent)
	{
		if(GetParent())
			mSprite = GetParent()->GetComponent<Sprite>();
		mPreviousState = (T)0;
	}
	virtual ~Animator() 
	{ 
		mAnimations.clear(); 
	}

	virtual void Enter(T state) = 0;
	virtual void During(T state, float deltaTime) = 0;
	virtual void Exit(T state) = 0;

	virtual void OnNotify(AnimationNotify notify, int chanel) {}

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

	virtual void SetParent(GameObject* parent)
	{
		Component::SetParent(parent);

		if (GetParent())
			mSprite = GetParent()->GetComponent<Sprite>();
	}

private:
	T mCurrentState;
	T mPreviousState;

	float mTimeInCurrentState;

protected:
	Sprite* mSprite;
	std::vector<std::unique_ptr<Animation>> mAnimations;
};
