#pragma once
#include "GameScreen.h"
#include "Animator.h"
class GameScreenLevel1 :
	public GameScreen
{
public:
	GameScreenLevel1();
	~GameScreenLevel1();

	void Update(float deltaTime, std::vector<SDL_Event> events)override;

private:
	Animation* PlayerAnimation;
};

