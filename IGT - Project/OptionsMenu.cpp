#include "OptionsMenu.h"
#include "Texture2D.h"
#include "Settings.h"
#include "SoundManager.h"
#include "CheckBox.h"
#include "Slider.h"
#include "Image.h"
#include "TextBox.h"


OptionsMenu::OptionsMenu(Transform * transform)
	:UIMenu("Options Menu", transform, false)
{
	mResolutionsText = new TextRender(this, "Fonts/nokiafc22.ttf", 8, "2560 x 1440 @ 60Hz", {255, 255, 255}, 90, 20, CENTER);
	AddComponent(mResolutionsText);
	mVideoComponents.push_back(mResolutionsText);	

	CreateWidgets();

	SwitchMenu(VIDEO);

	GetAvailableResolutions();

	GetCurrentResolution();
}

OptionsMenu::~OptionsMenu()
{
}

void OptionsMenu::CreateWidgets()
{
	GLuint ButtonTexture = Texture2D::LoadTexture2D("Images/ButtonsMenu.png");
	GLuint ArrowRightTexture = Texture2D::LoadTexture2D("Images/Arrow_Right.png");
	GLuint ArrowLeftTexture = Texture2D::LoadTexture2D("Images/Arrow_Left.png");
	GLuint CheckBoxTexture = Texture2D::LoadTexture2D("Images/CheckBox.png");
	GLuint SliderBarTexture = Texture2D::LoadTexture2D("Images/SliderBar.png");
	GLuint SliderThumbTexture = Texture2D::LoadTexture2D("Images/SliderThumb.png");

	UIWidget* widget = new Button(ButtonTexture, { 0.5,0.0 }, { -154,-20 }, { 100, 20 }, "Gameplay");
	widget->mWidgetData.ID = GAMEPLAY;
	widget->AddObserver(this);
	widget->SetIsFocusable(false);
	mWidgets.emplace_back(widget);
	AddChild(widget);

	widget = new Button(ButtonTexture, { 0.5,0.0 }, { -52,-20 }, { 100, 20 }, "Video");
	widget->mWidgetData.ID = VIDEO;
	widget->AddObserver(this);
	widget->SetIsFocusable(false);
	mWidgets.emplace_back(widget);
	AddChild(widget);

	widget = new Button(ButtonTexture, { 0.5,0.0 }, { 52,-20 }, { 100, 20 }, "Audio");
	widget->mWidgetData.ID = AUDIO;
	widget->AddObserver(this);
	widget->SetIsFocusable(false);
	mWidgets.emplace_back(widget);
	AddChild(widget);

	widget = new Button(ButtonTexture, { 0.5,0.0 }, { 154,-20 }, { 100, 20 }, "Controls");
	widget->mWidgetData.ID = CONTROLS;
	widget->AddObserver(this);
	widget->SetIsFocusable(false);
	mWidgets.emplace_back(widget);
	AddChild(widget);

	widget = new Image("RightBumber", { 0.5,0.0 }, { 214, -20 }, Texture2D::LoadTexture2D("Images/Button_RB.png"), { 16,16 });
	widget->AddObserver(this);
	mWidgets.emplace_back(widget);
	AddChild(widget);

	widget = new Image("LeftBumber", { 0.5,0.0 }, { -214, -20 }, Texture2D::LoadTexture2D("Images/Button_LB.png"), { 16,16 });
	widget->AddObserver(this);
	mWidgets.emplace_back(widget);
	AddChild(widget);

	//Video Settings------------------------------------------------------------------------------------------------------

	widget = new TextBox("Fullscreen Resolution Text", { 0.5,0.5 }, { -100, 20 }, "Fonts/nokiafc22.ttf", 8, "Fullscreen Resolution", { 255,255,255 }, CENTER);
	widget->AddObserver(this);
	mWidgets.emplace_back(widget);
	mVideoButtons.push_back(widget);
	AddChild(widget);

	widget = new TextBox("Fullscreen Text", { 0.5,0.5 }, { -100, 0 }, "Fonts/nokiafc22.ttf", 8, "Fullscreen", { 255,255,255 }, CENTER);
	widget->AddObserver(this);
	mWidgets.emplace_back(widget);
	mVideoButtons.push_back(widget);
	AddChild(widget);

	widget = new TextBox("Vsync Text", { 0.5,0.5 }, { -100, -20 }, "Fonts/nokiafc22.ttf", 8, "V - Sync", { 255,255,255 }, CENTER);
	widget->AddObserver(this);
	mWidgets.emplace_back(widget);
	mVideoButtons.push_back(widget);
	AddChild(widget);

	widget = new Button(ArrowRightTexture, { 0.5,0.5 }, { 150, 26 }, { 7, 11 });
	widget->mWidgetData.ID = RES_ARROW_RIGHT;
	widget->AddObserver(this);
	mWidgets.emplace_back(widget);
	mVideoButtons.push_back(widget);
	AddChild(widget);

	widget = new Button(ArrowLeftTexture, { 0.5,0.5 }, { 20,26 }, { 7, 11 });
	widget->mWidgetData.ID = RES_ARROW_LEFT;
	widget->AddObserver(this);
	mWidgets.emplace_back(widget);
	mVideoButtons.push_back(widget);
	AddChild(widget);

	widget = new CheckBox("FullScreen CheckBox", { 0.5,0.5 }, Vector2D(80, 5), CheckBoxTexture, Vector2D(20, 20),  Settings::GetInstance()->GetIsFullScreen());
	widget->mWidgetData.ID = FULLSCREEN;
	widget->AddObserver(this);
	mWidgets.emplace_back(widget);
	mVideoButtons.push_back(widget);
	AddChild(widget);

	widget = new CheckBox("Vsync CheckBox", { 0.5,0.5 }, Vector2D(80, -15), CheckBoxTexture, Vector2D(20, 20), Settings::GetInstance()->GetIsVsync());
	widget->mWidgetData.ID = VSYNC;
	widget->AddObserver(this);
	mWidgets.emplace_back(widget);
	mVideoButtons.push_back(widget);
	AddChild(widget);

	//Audio---------------------------------------------------------------------------------------------------------------
	widget = new TextBox("Master Volume Text", { 0.5,0.5 }, { -100, 20 }, "Fonts/nokiafc22.ttf", 8, "Master Volume", { 255,255,255 }, CENTER);
	widget->AddObserver(this);
	mWidgets.emplace_back(widget);
	mAudioButtons.push_back(widget);
	AddChild(widget);

	widget = new TextBox("Music Volume Text", { 0.5,0.5 }, { -100, 0 }, "Fonts/nokiafc22.ttf", 8, "Music Volume", { 255,255,255 }, CENTER);
	widget->AddObserver(this);
	mWidgets.emplace_back(widget);
	mAudioButtons.push_back(widget);
	AddChild(widget);

	widget = new TextBox("Sound Effect Volume Text", { 0.5,0.5 }, { -100, -20 }, "Fonts/nokiafc22.ttf", 8, "Sound Effect Volume", { 255,255,255 }, CENTER);
	widget->AddObserver(this);
	mWidgets.emplace_back(widget);
	mAudioButtons.push_back(widget);
	AddChild(widget);

	widget = new Slider("MasterVolume Slider", { 0.5,0.5 }, { 100, 24 }, { 114, 4 }, SliderBarTexture, { 15,12 }, SliderThumbTexture, SoundManager::GetInstance()->GetMasterVolume(), 128);
	widget->mWidgetData.ID = VOL_MASTER;
	widget->AddObserver(this);
	mWidgets.emplace_back(widget);
	mAudioButtons.push_back(widget);
	AddChild(widget);

	widget = new Slider("MusicVolume Slider", { 0.5,0.5 }, { 100, 4 }, { 114, 4 }, SliderBarTexture, { 15,12 }, SliderThumbTexture, SoundManager::GetInstance()->GetMusicVolume(), 128);
	widget->mWidgetData.ID = VOL_MUSIC;
	widget->AddObserver(this);
	mWidgets.emplace_back(widget);
	mAudioButtons.push_back(widget);
	AddChild(widget);

	widget = new Slider("SoundEffectVolume Slider", { 0.5,0.5 }, { 100, -14 }, { 114, 4 }, SliderBarTexture, { 15,12 }, SliderThumbTexture, SoundManager::GetInstance()->GetSoundEffectVolume(), 128);
	widget->mWidgetData.ID = VOL_SOUND;
	widget->AddObserver(this);
	mWidgets.emplace_back(widget);
	mAudioButtons.push_back(widget);
	AddChild(widget);

	//Controls------------------------------------------------------------------------------------------------------------
	widget = new Image("Controller", { 0.25,0.5 }, { 0,0 }, Texture2D::LoadTexture2D("Images/Controller.png"), { 147, 108 });
	mWidgets.emplace_back(widget);
	widget->AddObserver(this);
	mControlsButtons.push_back(widget);
	AddChild(widget);

	widget = new TextBox("Move Text", { 0.75,0.5 }, { -20,44 }, "Fonts/nokiafc22.ttf", 8, "Move", { 255,255,255 }, CENTER);
	mWidgets.emplace_back(widget);
	widget->AddObserver(this);
	mControlsButtons.push_back(widget);
	AddChild(widget);

	widget = new Image("W", { 0.75,0.5 }, { 28,50 }, Texture2D::LoadTexture2D("Images/Key_W.png"), { 16, 16 });
	mWidgets.emplace_back(widget);
	widget->AddObserver(this);
	mControlsButtons.push_back(widget);
	AddChild(widget);

	widget = new Image("A", { 0.75,0.5 }, { 44,50 }, Texture2D::LoadTexture2D("Images/Key_A.png"), { 16, 16 });
	mWidgets.emplace_back(widget);
	widget->AddObserver(this);
	mControlsButtons.push_back(widget);
	AddChild(widget);

	widget = new Image("S", { 0.75,0.5 }, { 60,50 }, Texture2D::LoadTexture2D("Images/Key_S.png"), { 16, 16 });
	mWidgets.emplace_back(widget);
	widget->AddObserver(this);
	mControlsButtons.push_back(widget);
	AddChild(widget);

	widget = new Image("D", { 0.75,0.5 }, { 76,50 }, Texture2D::LoadTexture2D("Images/Key_D.png"), { 16, 16 });
	mWidgets.emplace_back(widget);
	widget->AddObserver(this);
	mControlsButtons.push_back(widget);
	AddChild(widget);

	widget = new Button(ButtonTexture, { 1.0f, 1.0f }, { -65, 20 }, { 100, 20 }, "Back");
	widget->mWidgetData.ID = BACK;
	widget->AddObserver(this);
	mWidgets.emplace_back(widget);
	AddChild(widget);
}

void OptionsMenu::OnNotify(WidgetEvent event, WidgetEventData data)
{
	switch (event)
	{
	case WidgetEvent::ON_HOVERED:
		switch (data.ID)
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
	case WidgetEvent::ON_CLICKED:
		switch (data.ID)
		{
		case GAMEPLAY:
			break;
		case VIDEO:
			break;
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
			Settings::GetInstance()->SetFullScreen(!data.value);
			break;
		case VSYNC:
			Settings::GetInstance()->SetVsync(!data.value);
			break;
		case VOL_MASTER:
			SoundManager::GetInstance()->SetMasterVolume(data.value);
			break;
		case VOL_MUSIC:
			SoundManager::GetInstance()->SetMusicVolume(data.value);
			break;
		case VOL_SOUND:
			SoundManager::GetInstance()->SetSoundEffectVolume(data.value);
			break;
		case BACK:
			MenuManager::GetInstance()->ChangeToPreviousMenu();
			Settings::GetInstance()->ApplySettings();
			break;
		default:
			break;
		}
	case WidgetEvent::ON_DRAGGED:
		switch (data.ID)
		{
		case VOL_MASTER:
			SoundManager::GetInstance()->SetMasterVolume(data.value);
			break;
		case VOL_MUSIC:
			SoundManager::GetInstance()->SetMusicVolume(data.value);
			break;
		case VOL_SOUND:
			SoundManager::GetInstance()->SetSoundEffectVolume(data.value);
			break;
		}
		break;
	}
}

//void OptionsMenu::Up()
//{
//	mCurrentOption--;
//
//	if (mCurrentOption < 0)
//		mCurrentOption = 0;
//
//	switch (mCurrentMenu)
//	{
//	case GAMEPLAY:
//		break;
//	case VIDEO:
//		if (mCurrentOption == 1)
//		{
//			GetCurrentWidget()->OnUnHovered();
//			SetCurrentWidget(6);
//			GetCurrentWidget()->OnHovered();
//		}
//		else if (mCurrentOption == 2)
//		{
//			GetCurrentWidget()->OnUnHovered();
//			SetCurrentWidget(7);
//			GetCurrentWidget()->OnHovered();
//		}
//		else
//		{
//			GetCurrentWidget()->OnUnHovered();
//		}
//
//		break;
//	case AUDIO:
//		break;
//	case CONTROLS:
//		break;
//	default:
//		break;
//	}
//}

//void OptionsMenu::Down()
//{
//	mCurrentOption++;
//
//	switch (mCurrentMenu)
//	{
//	case GAMEPLAY:
//		break;
//	case VIDEO:
//		if (mCurrentOption > 2)
//			mCurrentOption = 2;
//
//		if (mCurrentOption == 1)
//		{
//			GetCurrentWidget()->OnUnHovered();
//			SetCurrentWidget(6);
//			GetCurrentWidget()->OnHovered();
//		}
//		else if (mCurrentOption == 2)
//		{
//			GetCurrentWidget()->OnUnHovered();
//			SetCurrentWidget(7);
//			GetCurrentWidget()->OnHovered();
//		}
//		else
//		{
//			GetCurrentWidget()->OnUnHovered();
//		}
//
//		break;
//	case AUDIO:
//		break;
//	case CONTROLS:
//		break;
//	default:
//		break;
//	}
//}

void OptionsMenu::Left()
{
	switch (mCurrentMenu)
	{
	case GAMEPLAY:
		break;
	case VIDEO:
		if (mCurrentOption == 0)
		{
			ResolutionSelector(mCurrentResolution + 1);
		}
		break;
	case AUDIO:
		break;
	case CONTROLS:
		break;
	default:
		break;
	}
}

void OptionsMenu::Right()
{
	switch (mCurrentMenu)
	{
	case GAMEPLAY:
		break;
	case VIDEO:
		if (mCurrentOption == 0)
		{
			ResolutionSelector(mCurrentResolution - 1);
		}
		break;
	case AUDIO:
		break;
	case CONTROLS:
		break;
	default:
		break;
	}
}

void OptionsMenu::Next()
{
	switch (mCurrentMenu)
	{
	case GAMEPLAY:
		SwitchMenu(VIDEO);
		break;
	case VIDEO:
		SwitchMenu(AUDIO);
		break;
	case AUDIO:
		SwitchMenu(CONTROLS);
		break;
	case CONTROLS:
		break;
	default:
		break;
	}
}

void OptionsMenu::Previous()
{
	switch (mCurrentMenu)
	{
	case GAMEPLAY:
		break;
	case VIDEO:
		SwitchMenu(GAMEPLAY);
		break;
	case AUDIO:
		SwitchMenu(VIDEO);
		break;
	case CONTROLS:
		SwitchMenu(AUDIO);
		break;
	default:
		break;
	}
}

void OptionsMenu::SwitchMenu(OptionsMenuButtons menu)
{
	mCurrentMenu = menu;

	switch (menu)
	{
	case GAMEPLAY:
		for (UIWidget* button : mGameplayButtons)
		{
			button->SetActive(true);
		}
		for (Component* component : mVideoComponents)
		{
			component->SetActive(false);
		}
		for (UIWidget* button : mVideoButtons)
		{
			button->SetActive(false);
		}
		for (UIWidget* button : mAudioButtons)
		{
			button->SetActive(false);
		}
		for (UIWidget* button : mControlsButtons)
		{
			button->SetActive(false);
		}
		break;
	case VIDEO:
		for (Component* component : mVideoComponents)
		{
			component->SetActive(true);
		}
		for (UIWidget* button : mVideoButtons)
		{
			button->SetActive(true);
		}
		for (UIWidget* button : mGameplayButtons)
		{
			button->SetActive(false);
		}
		for (UIWidget* button : mAudioButtons)
		{
			button->SetActive(false);
		}
		for (UIWidget* button : mControlsButtons)
		{
			button->SetActive(false);
		}
		break;
	case AUDIO:
		for (Component* component : mVideoComponents)
		{
			component->SetActive(false);
		}
		for (UIWidget* button : mVideoButtons)
		{
			button->SetActive(false);
		}
		for (UIWidget* button : mGameplayButtons)
		{
			button->SetActive(false);
		}
		for (UIWidget* button : mAudioButtons)
		{
			button->SetActive(true);
		}
		for (UIWidget* button : mControlsButtons)
		{
			button->SetActive(false);
		}
		break;
	case CONTROLS:
		for (UIWidget* button : mGameplayButtons)
		{
			button->SetActive(false);
		}
		for (Component* component : mVideoComponents)
		{
			component->SetActive(false);
		}
		for (UIWidget* button : mVideoButtons)
		{
			button->SetActive(false);
		}
		for (UIWidget* button : mAudioButtons)
		{
			button->SetActive(false);
		}
		for (UIWidget* button : mControlsButtons)
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

	for (int display_index = 0; display_index < display_count; display_index++)
	{
		int modes_count = SDL_GetNumDisplayModes(display_index);

		for (int mode_index = 0; mode_index <= modes_count; mode_index++)
		{
			SDL_DisplayMode mode = { SDL_PIXELFORMAT_UNKNOWN, 0, 0, 0, 0 };

			if (SDL_GetDisplayMode(display_index, mode_index, &mode) == 0)
			{
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
	if (resolutionIndex > (mResolutions.size() -1) && resolutionIndex != -1)
	{
		mCurrentResolution = 0;
	}
	else if (resolutionIndex < 0)
	{	
		mCurrentResolution = (mResolutions.size() -1);
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
