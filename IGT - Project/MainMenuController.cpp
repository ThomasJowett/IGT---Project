#include "MainMenuController.h"

#include <iostream>

MainMenuController::MainMenuController()
{
}


MainMenuController::~MainMenuController()
{
}

void MainMenuController::MoveUp(float scale)
{
	std::cout << "up\n";
}

void MainMenuController::MoveRight(float scale)
{
	std::cout << "right\n";
}

void MainMenuController::Up()
{
	std::cout << "up\n";
}

void MainMenuController::Down()
{
	std::cout << "down\n";
}

void MainMenuController::Start()
{
	std::cout << "start\n";
}

void MainMenuController::Select()
{
	std::cout << "select\n";
}

void MainMenuController::AButton()
{
	std::cout << "A\n";
}
