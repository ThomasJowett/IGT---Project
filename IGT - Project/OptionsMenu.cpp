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

	Button* button = new Button(ButtonTexture, { 0,25 }, { 100, 20 }, "Change Resolution");
	button->mButtonID = CHANGE_RESOLUTION;
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
			MenuManager::GetInstance()->ChangeToMenu(0);
			break;
		}
	}
}
