#include "HUD.h"
#include "Image.h"
#include "ProgressBar.h"
#include "Texture2D.h"

HUD::HUD()
	:UIMenu("HUD", new Transform())
{
	CreateWidgets();
}

void HUD::CreateWidgets()
{
	ProgressBar* progressBar = new ProgressBar("Health Bar", { 0, 0 }, { 64,-7 }, "Images/HealthBar.png", { 52,6 }, 1.0f);
	mWidgets.emplace_back(progressBar);
	AddChild(progressBar);

	progressBar = new ProgressBar("Stamina Bar", { 0, 0 }, { 64,-17 }, "Images/StaminaBar.png", { 52,6 }, 1.0f);
	mWidgets.emplace_back(progressBar);
	AddChild(progressBar);

	progressBar = new ProgressBar("Stamina Bar", { 0, 0 }, { 64,-27 }, "Images/MagickaBar.png", { 52,6 }, 1.0f);
	mWidgets.emplace_back(progressBar);
	AddChild(progressBar);

	Image* background = new Image("Player1 Background", { 0,0 }, { 52, -17 }, Texture2D::GetTexture2D("Images/HUD_BackGround.png"), { 100,32 });
	AddChild(background);

	background = new Image("Player1 Hotbar", { 0,1 }, { 48, 20 }, Texture2D::GetTexture2D("Images/Hotbar_Background.png"), { 90,24 });
	AddChild(background);
}