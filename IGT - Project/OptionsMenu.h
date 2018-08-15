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
	VOL_MASTER_LEFT,
	VOL_MASTER_RIGHT,
	VOL_MUSIC_LEFT,
	VOL_MUSIC_RIGHT,
	VOL_SOUND_LEFT,
	VOL_SOUND_RIGHT,

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

	void Up()override;
	void Down()override;
	void Left()override;
	void Right()override;

	void Next()override;
	void Previous()override;

private:
	void SwitchMenu(OptionsMenuButtons menu);
	void GetAvailableResolutions();
	void GetCurrentResolution();
	void ResolutionSelector(int resolutionIndex);

	std::vector<Component*> mGameplayComponents;
	std::vector<UIWidget*> mGameplayButtons;
	std::vector<Component*> mVideoComponents;
	std::vector<UIWidget*> mVideoButtons;
	std::vector<Component*> mAudioComponents;
	std::vector<UIWidget*> mAudioButtons;
	std::vector<Component*> mControlsComponents;
	std::vector<UIWidget*> mControlsButtons;

	std::vector<SDL_DisplayMode> mResolutions;

	TextRender* mResolutionsText;

	int mCurrentResolution;

	OptionsMenuButtons mCurrentMenu;
	int mCurrentOption = 0;
};

