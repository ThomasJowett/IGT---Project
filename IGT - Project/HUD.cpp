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
	ProgressBar* progressBar = new ProgressBar("Health Bar", { 0, 1 }, { 64,28 }, "Images/HealthBar.png", { 51,4 }, 1.0f);
	mWidgets.emplace_back(progressBar);
	AddChild(progressBar);

	progressBar = new ProgressBar("Stamina Bar", { 0, 1 }, { 64,28 }, "Images/StaminaBar.png", { 51,4 }, 1.0f);
	mWidgets.emplace_back(progressBar);
	AddChild(progressBar);

	Image* background = new Image("Player1 BackBround", { 0,1 }, { 56, 20 }, Texture2D::GetTexture2D("Images/HUD_BackGround.png"), { 100,32 });

	AddChild(background);	
}