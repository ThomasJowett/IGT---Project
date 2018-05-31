#include "MainMenuPawn.h"

#include <iostream>

MainMenuPawn::MainMenuPawn()
{
}


MainMenuPawn::~MainMenuPawn()
{
}

void MainMenuPawn::MoveUp(float scale)
{
	std::cout << scale << std::endl;
}

void MainMenuPawn::MoveRight(float scale)
{
	std::cout << scale << std::endl;
}

void MainMenuPawn::LookUp(float scale)
{
	//std::cout << scale << std::endl;
}

void MainMenuPawn::LookRight(float scale)
{
	//std::cout << scale << std::endl;
}

void MainMenuPawn::Up()
{
	std::cout << "up\n";
}

void MainMenuPawn::Down()
{
	std::cout << "down\n";
}

void MainMenuPawn::Start()
{
	std::cout << "start\n";
}

void MainMenuPawn::Select()
{
	std::cout << "select\n";
}

void MainMenuPawn::AButton()
{
	std::cout << "A\n";
}

void MainMenuPawn::RightTrigger(float scale)
{
	std::cout << scale << std::endl;
}

void MainMenuPawn::LeftTrigger(float scale)
{
	std::cout << scale << std::endl;
}

void MainMenuPawn::MousePosition(int x, int y)
{
	//std::cout << x <<" "<< y << std::endl;
}
