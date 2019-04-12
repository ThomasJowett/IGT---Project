#include "HUD.h"
#include "Image.h"
#include "ProgressBar.h"
#include "Texture2D.h"

HUD::HUD(GameObject* subject)
	:UIMenu("HUD", new Transform())
{
	mSubject = subject;
}

Player1HUD::Player1HUD(GameObject * subject)
	:HUD(subject)
{
	CreateWidgets();
}

void Player1HUD::CreateWidgets()
{
	ProgressBar* progressBar = new HealthBar("Health Bar", { 0, 0 }, { 64,-7 }, "Images/HealthBar.png", { 52,6 }, 1.0f, mSubject);
	mWidgets.emplace_back(progressBar);
	AddChild(progressBar);

	progressBar = new ProgressBar("Stamina Bar", { 0, 0 }, { 64,-17 }, "Images/StaminaBar.png", { 52,6 }, 1.0f);
	mWidgets.emplace_back(progressBar);
	AddChild(progressBar);

	progressBar = new ProgressBar("Stamina Bar", { 0, 0 }, { 64,-27 }, "Images/MagickaBar.png", { 52,6 }, 1.0f);
	mWidgets.emplace_back(progressBar);
	AddChild(progressBar);

	Image* portrait = new Image("Player1 Portrait", { 0,0 }, { 18, -17 }, Texture2D::GetTexture2D("Images/Portrait 1.png"), { 32,32 });
	AddChild(portrait);

	Image* background = new Image("Player1 Background", { 0,0 }, { 52, -17 }, Texture2D::GetTexture2D("Images/HUD_BackGround.png"), { 100,32 });
	AddChild(background);

	background = new Image("Player1 Hotbar", { 0,1 }, { 48, 20 }, Texture2D::GetTexture2D("Images/Hotbar_Background.png"), { 90,24 });
	AddChild(background);
}

Player2HUD::Player2HUD(GameObject * subject)
	:HUD(subject)
{
	CreateWidgets();
}

void Player2HUD::CreateWidgets()
{
	ProgressBar* progressBar = new HealthBar("Health Bar", { 1, 0 }, { -40,-7 }, "Images/HealthBar.png", { 52,6 }, 1.0f, mSubject);
	mWidgets.emplace_back(progressBar);
	AddChild(progressBar);

	progressBar = new ProgressBar("Stamina Bar", { 1, 0 }, { -40,-17 }, "Images/StaminaBar.png", { 52,6 }, 1.0f);
	mWidgets.emplace_back(progressBar);
	AddChild(progressBar);

	progressBar = new ProgressBar("Stamina Bar", { 1, 0 }, { -40,-27 }, "Images/MagickaBar.png", { 52,6 }, 1.0f);
	mWidgets.emplace_back(progressBar);
	AddChild(progressBar);

	Image* portrait = new Image("Player2 Portrait", { 1,0 }, { -86, -17 }, Texture2D::GetTexture2D("Images/Portrait 2.png"), { 32,32 });
	AddChild(portrait);

	Image* background = new Image("Player2 Background", { 1,0 }, { -52, -17 }, Texture2D::GetTexture2D("Images/HUD_BackGround.png"), { 100,32 });
	AddChild(background);

	background = new Image("Player2 Hotbar", { 1,1 }, { -48, 20 }, Texture2D::GetTexture2D("Images/Hotbar_Background.png"), { 90,24 });
	AddChild(background);

	
}
