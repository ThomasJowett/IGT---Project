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

	BACK
};

class OptionsMenu :
	public UIMenu
{
public:
	OptionsMenu(Transform* transform);
	virtual ~OptionsMenu();

	void CreateButtons()override;
	void OnNotify(ButtonEvent event, int ID)override;

	void Up()override;
	void Down()override;
	void Left()override;
	void Right()override;

private:
	void SwitchMenu(OptionsMenuButtons menu);
	void GetAvailableResolutions();
	void GetCurrentResolution();
	void ResolutionSelector(int resolutionIndex);

	std::vector<Component*> mGameplayComponents;
	std::vector<Button*> mGameplayButtons;
	std::vector<Component*> mVideoComponents;
	std::vector<Button*> mVideoButtons;
	std::vector<Component*> mAudioComponents;
	std::vector<Button*> mAudioButtons;
	std::vector<Component*> mControlsComponents;
	std::vector<Button*> mControlsButtons;

	std::vector<SDL_DisplayMode> mResolutions;

	TextRender* mResolutionsText;

	int mCurrentResolution;
};

