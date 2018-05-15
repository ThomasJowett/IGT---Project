#include "GameScreenMenu.h"

#include <iostream>

GameScreenMenu::GameScreenMenu() : GameScreen()
{
	mTextRender = new TextRender("Fonts/nokiafc22.ttf", 24);
}


GameScreenMenu::~GameScreenMenu()
{
}

void GameScreenMenu::Render()
{
	mTextRender->DisplayText("Hello World", SDL_Colour{255, 255, 255}, 960, 540, CENTER);
}

void GameScreenMenu::Update(float deltaTime, std::vector<SDL_Event> e)
{
}
