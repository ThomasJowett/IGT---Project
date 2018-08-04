#include "OptionsMenu.h"
#include "Texture2D.h"
#include "Settings.h"
#include "CheckBox.h"


OptionsMenu::OptionsMenu(Transform * transform)
	:UIMenu("Options Menu", transform, false)
{
	Component* component;

	GLuint LeftBumber = Texture2D::LoadTexture2D("Images/Button_LB.png");
	GLuint RightBumber = Texture2D::LoadTexture2D("Images/Button_RB.png");
	component = new Sprite(this, LeftBumber, 16, 16, Vector2D(-212, 90));
	AddComponent(component);
	component = new Sprite(this, RightBumber, 16, 16, Vector2D(212, 90));
	AddComponent(component);
	//GamePlay------------------------------------------------------------------------------------------------------------

	//Video Settings------------------------------------------------------------------------------------------------------
	component = new TextRender(this, "Fonts/nokiafc22.ttf", 8, "Fullscreen Resolution", { 255,255,255 }, -100, 20, CENTER);
	AddComponent(component);
	mVideoComponents.push_back(component);

	mResolutionsText = new TextRender(this, "Fonts/nokiafc22.ttf", 8, "2560 x 1440 @ 60Hz", {255, 255, 255}, 90, 20, CENTER);
	AddComponent(mResolutionsText);
	mVideoComponents.push_back(mResolutionsText);

	component = new TextRender(this, "Fonts/nokiafc22.ttf", 8, "Fullscreen", { 255,255,255 }, -100, 0, CENTER);
	AddComponent(component);
	mVideoComponents.push_back(component);

	component = new TextRender(this, "Fonts/nokiafc22.ttf", 8, "V - Sync", { 255,255,255 }, -100, -20, CENTER);
	AddComponent(component);
	mVideoComponents.push_back(component);

	//Audio---------------------------------------------------------------------------------------------------------------

	//Controls------------------------------------------------------------------------------------------------------------
	//xbox controller image
	GLuint ControllerTexture = Texture2D::LoadTexture2D("Images/Controller.png");
	component = new Sprite(this, ControllerTexture, 147, 108, Vector2D(-120, -50));
	AddComponent(component);
	mControlsComponents.push_back(component);
	component->SetActive(false);

	CreateButtons();
	GetAvailableResolutions();

	GetCurrentResolution();
}

OptionsMenu::~OptionsMenu()
{
}

void OptionsMenu::CreateButtons()
{
	GLuint ButtonTexture = Texture2D::LoadTexture2D("Images/ButtonsMenu.png");
	GLuint ArrowRightTexture = Texture2D::LoadTexture2D("Images/Arrow_Right.png");
	GLuint ArrowLeftTexture = Texture2D::LoadTexture2D("Images/Arrow_Left.png");
	GLuint CheckBoxTexture = Texture2D::LoadTexture2D("Images/CheckBox.png");

	Button* button = new Button(ButtonTexture, { -154,90 }, { 100, 20 }, "Gameplay");
	button->mButtonID = GAMEPLAY;
	button->AddObserver(this);
	mButtons.emplace_back(button);

	button = new Button(ButtonTexture, { -52,90 }, { 100, 20 }, "Video");
	button->mButtonID = VIDEO;
	button->AddObserver(this);
	mButtons.emplace_back(button);

	button = new Button(ButtonTexture, { 52,90 }, { 100, 20 }, "Audio");
	button->mButtonID = AUDIO;
	button->AddObserver(this);
	mButtons.emplace_back(button);

	button = new Button(ButtonTexture, { 154,90 }, { 100, 20 }, "Controls");
	button->mButtonID = CONTROLS;
	button->AddObserver(this);
	mButtons.emplace_back(button);

	button = new Button(ArrowRightTexture, { 150,24 }, { 7, 11 });
	button->mButtonID = RES_ARROW_RIGHT;
	button->AddObserver(this);
	mButtons.emplace_back(button);
	mVideoButtons.push_back(button);

	button = new Button(ArrowLeftTexture, { 20,24 }, { 7, 11 });
	button->mButtonID = RES_ARROW_LEFT;
	button->AddObserver(this);
	mButtons.emplace_back(button);
	mVideoButtons.push_back(button);

	button = new CheckBox(CheckBoxTexture, Vector2D(80, 5), Vector2D(20, 20), Settings::GetInstance()->GetIsFullScreen());
	button->mButtonID = FULLSCREEN;
	button->AddObserver(this);
	mButtons.emplace_back(button);
	mVideoButtons.push_back(button);

	button = new CheckBox(CheckBoxTexture, Vector2D(80, -15), Vector2D(20, 20), true);
	button->mButtonID = VSYNC;
	button->AddObserver(this);
	mButtons.emplace_back(button);
	mVideoButtons.push_back(button);

	button = new Button(ButtonTexture, { 154,-100 }, { 100, 20 }, "Back");
	button->mButtonID = BACK;
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
		case GAMEPLAY:
			SwitchMenu(GAMEPLAY);
			break;
		case VIDEO:
			SwitchMenu(VIDEO);
			break;
		case AUDIO:
			SwitchMenu(AUDIO);
			break;
		case CONTROLS:
			SwitchMenu(CONTROLS);
			break;
		default:
			break;
		}
		break;
	case ButtonEvent::ON_CLICKED:
		switch (ID)
		{
		case GAMEPLAY:
			break;
		case VIDEO:
			//TODO:get a list of available resolutions and go through them
		case AUDIO:
			break;
		case CONTROLS:
			break;
		case RES_ARROW_LEFT:
			ResolutionSelector(mCurrentResolution+1);
			break;
		case RES_ARROW_RIGHT:
			ResolutionSelector(mCurrentResolution-1);
			break;
		case FULLSCREEN:
			Settings::GetInstance()->SetFullScreen(!(Settings::GetInstance()->GetIsFullScreen()));
			break;
		case VSYNC:
			Settings::GetInstance()->SetVsync(!(Settings::GetInstance()->GetIsVsync()));
			break;
		case BACK:
			MenuManager::GetInstance()->ChangeToPreviousMenu();
			Settings::GetInstance()->ApplySettings();
			break;
		default:
			break;
		}
	}
}

void OptionsMenu::Up()
{
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

void OptionsMenu::SwitchMenu(OptionsMenuButtons menu)
{
	switch (menu)
	{
	case GAMEPLAY:
		for (Component* component : mGameplayComponents)
		{
			component->SetActive(true);
		}
		for (Button* button : mGameplayButtons)
		{
			button->SetActive(true);
		}
		for (Component* component : mVideoComponents)
		{
			component->SetActive(false);
		}
		for (Button* button : mVideoButtons)
		{
			button->SetActive(false);
		}
		for (Component* component : mAudioComponents)
		{
			component->SetActive(false);
		}
		for (Button* button : mAudioButtons)
		{
			button->SetActive(false);
		}
		for (Component* component : mControlsComponents)
		{
			component->SetActive(false);
		}
		for (Button* button : mControlsButtons)
		{
			button->SetActive(false);
		}
		break;
	case VIDEO:
		for (Component* component : mVideoComponents)
		{
			component->SetActive(true);
		}
		for (Button* button : mVideoButtons)
		{
			button->SetActive(true);
		}
		for (Component* component : mGameplayComponents)
		{
			component->SetActive(false);
		}
		for (Button* button : mGameplayButtons)
		{
			button->SetActive(false);
		}
		for (Component* component : mAudioComponents)
		{
			component->SetActive(false);
		}
		for (Button* button : mAudioButtons)
		{
			button->SetActive(false);
		}
		for (Component* component : mControlsComponents)
		{
			component->SetActive(false);
		}
		for (Button* button : mControlsButtons)
		{
			button->SetActive(false);
		}
		break;
	case AUDIO:
		for (Component* component : mVideoComponents)
		{
			component->SetActive(false);
		}
		for (Button* button : mVideoButtons)
		{
			button->SetActive(false);
		}
		for (Component* component : mGameplayComponents)
		{
			component->SetActive(false);
		}
		for (Button* button : mGameplayButtons)
		{
			button->SetActive(false);
		}
		for (Component* component : mAudioComponents)
		{
			component->SetActive(true);
		}
		for (Button* button : mAudioButtons)
		{
			button->SetActive(true);
		}
		for (Component* component : mControlsComponents)
		{
			component->SetActive(false);
		}
		for (Button* button : mControlsButtons)
		{
			button->SetActive(false);
		}
		break;
	case CONTROLS:
		for (Component* component : mGameplayComponents)
		{
			component->SetActive(false);
		}
		for (Button* button : mGameplayButtons)
		{
			button->SetActive(false);
		}
		for (Component* component : mVideoComponents)
		{
			component->SetActive(false);
		}
		for (Button* button : mVideoButtons)
		{
			button->SetActive(false);
		}
		for (Component* component : mAudioComponents)
		{
			component->SetActive(false);
		}
		for (Button* button : mAudioButtons)
		{
			button->SetActive(false);
		}
		for (Component* component : mControlsComponents)
		{
			component->SetActive(true);
		}
		for (Button* button : mControlsButtons)
		{
			button->SetActive(true);
		}
		break;
	default:
		break;
	}
}

void OptionsMenu::GetAvailableResolutions()
{
	int display_count = SDL_GetNumVideoDisplays();

	SDL_Log("Number of displays: %i", display_count);

	for (int display_index = 0; display_index < display_count; display_index++)
	{
		SDL_Log("Display %i:", display_index);

		int modes_count = SDL_GetNumDisplayModes(display_index);

		for (int mode_index = 0; mode_index <= modes_count; mode_index++)
		{
			SDL_DisplayMode mode = { SDL_PIXELFORMAT_UNKNOWN, 0, 0, 0, 0 };

			if (SDL_GetDisplayMode(display_index, mode_index, &mode) == 0)
			{
				SDL_Log(" %i bpp\t%i x %i @ %iHz",
					SDL_BITSPERPIXEL(mode.format), mode.w, mode.h, mode.refresh_rate);

				mResolutions.push_back(mode);
			}
		}
	}
}

//Gets the current display mode of the monitor
//Assumes that a list of available resolutions has been got
void OptionsMenu::GetCurrentResolution()
{
	SDL_DisplayMode currentDisplayMode;

	SDL_GetCurrentDisplayMode(0, &currentDisplayMode);

	std::string text = std::to_string(currentDisplayMode.w);
	text.append(" x " + std::to_string(currentDisplayMode.h) + " @" + std::to_string(currentDisplayMode.refresh_rate) + "Hz");

	mResolutionsText->UpdateText(text.c_str());

	mCurrentResolution = 0;

	for (SDL_DisplayMode mode : mResolutions)
	{
		if (currentDisplayMode.w == mode.w 
			&& currentDisplayMode.h == mode.h
			&& currentDisplayMode.refresh_rate == mode.refresh_rate
			&& currentDisplayMode.format == mode.format)
		{
			break;
		}

		mCurrentResolution++;
	}

	Settings::GetInstance()->SetDisplayMode(currentDisplayMode);
}

void OptionsMenu::ResolutionSelector(int resolutionIndex)
{
	if (resolutionIndex > mResolutions.size() -1)
	{
		mCurrentResolution = 0;
	}
	else if (resolutionIndex < 0)
	{
		mCurrentResolution = mResolutions.size() -1;
	}
	else
	{
		mCurrentResolution = resolutionIndex;
	}

	std::string text = std::to_string(mResolutions[mCurrentResolution].w);
	text.append(" x " + std::to_string(mResolutions[mCurrentResolution].h) + " @" + std::to_string(mResolutions[mCurrentResolution].refresh_rate) + "Hz");
	mResolutionsText->UpdateText(text.c_str());

	Settings::GetInstance()->SetDisplayMode(mResolutions[mCurrentResolution]);
}
