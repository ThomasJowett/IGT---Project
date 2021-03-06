#pragma once
#include "UIMenu.h"
#include "MainMenu.h"

class MainMenu;

enum OptionsMenuButtons
{
	GAMEPLAY=0,
	VIDEO,
	AUDIO,
	CONTROLS,
	RES_ARROW_RIGHT,
	RES_ARROW_LEFT,
	FULLSCREEN,
	VSYNC,
	VOL_MASTER,
	VOL_MUSIC,
	VOL_SOUND,

	BACK
};

class OptionsMenu :
	public UIMenu
{
public:
	OptionsMenu(Transform* transform);
	virtual ~OptionsMenu();

	void CreateWidgets()override;
	void OnNotify(WidgetEvent event, WidgetEventData data)override;

	//void Up()override;
	//void Down()override;
	void Left()override;
	void Right()override;

	void Next()override;
	void Previous()override;

private:
	void SwitchMenu(OptionsMenuButtons menu);
	void GetAvailableResolutions();
	void GetCurrentResolution();
	void ResolutionSelector(int resolutionIndex);

	
	std::vector<UIWidget*> mGameplayButtons;
	std::vector<Component*> mVideoComponents;
	std::vector<UIWidget*> mVideoButtons;
	std::vector<UIWidget*> mAudioButtons;
	std::vector<UIWidget*> mControlsButtons;

	std::vector<SDL_DisplayMode> mResolutions;

	TextRender* mResolutionsText;

	int mCurrentResolution;

	OptionsMenuButtons mCurrentMenu;
	int mCurrentOption = 0;
};

