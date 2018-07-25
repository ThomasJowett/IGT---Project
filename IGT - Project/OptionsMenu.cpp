#include "OptionsMenu.h"
#include "Texture2D.h"
#include "Settings.h"



OptionsMenu::OptionsMenu(Transform * transform)
	:UIMenu("Options Menu", transform, false)
{
	CreateButtons();
}

OptionsMenu::~OptionsMenu()
{
}

void OptionsMenu::CreateButtons()
{
	GLuint ButtonTexture = Texture2D::LoadTexture2D("Images/ButtonsMenu.png");
	GLuint ArrowRightTexture = Texture2D::LoadTexture2D("Images/Arrow_Right.png");
	GLuint ArrowLeftTexture = Texture2D::LoadTexture2D("Images/Arrow_Left.png");



	Button* button = new Button(ButtonTexture, { 0,25 }, { 100, 20 }, "FullScreen Resolution");
	button->mButtonID = CHANGE_RESOLUTION;
	button->AddObserver(this);
	mButtons.emplace_back(button);

	button = new Button(ArrowRightTexture, { 55,25 }, { 7, 11 }, " ");
	button->mButtonID = 3;
	button->AddObserver(this);
	mButtons.emplace_back(button);

	button = new Button(ArrowLeftTexture, { -55,25 }, { 7, 11 }, " ");
	button->mButtonID = 4;
	button->AddObserver(this);
	mButtons.emplace_back(button);

	button = new Button(ButtonTexture, { 0,0 }, { 100, 20 }, "Back");
	button->mButtonID = 1;
	button->AddObserver(this);
	mButtons.emplace_back(button);
}

void OptionsMenu::OnNotify(ButtonEvent event, int ID)
{
	switch (event)
	{
	case ButtonEvent::ON_HOVERED:
		switch (ID)
		{
		case CHANGE_RESOLUTION:
			break;
		}
		break;
	case ButtonEvent::ON_CLICKED:
		switch (ID)
		{
		case CHANGE_RESOLUTION:
			//TODO:get a list of available resolutions and go through them
			Settings::GetInstance()->SetResolution(2560, 1440);
			break;
		case 1:
			MenuManager::GetInstance()->ChangeToPreviousMenu();
			break;
		}
	}
}

void OptionsMenu::Up()
{
	mButtons[mCurrentButton]->OnUnHovered();

	if(mCurrentButton != 0)
		mCurrentButton--;

	if (mCurrentButton < 0);
}

void OptionsMenu::Down()
{
}

void OptionsMenu::Left()
{
}

void OptionsMenu::Right()
{
}
