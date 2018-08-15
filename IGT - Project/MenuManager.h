#pragma once
#include "iInput.h"
#include "Messaging.h"
#include "UIMenu.h"
#include <vector>

class UIMenu;

class MenuManager
{
public:
	~MenuManager();

	static MenuManager * GetInstance();

	void ChangeToMenu(int menu);
	void ChangeToPreviousMenu();

	int AddMenu(UIMenu* menu);
	void RemoveMenu(int ID);
	void RemoveAllMenus();

	void ShowCurrentMenu(bool isActive);

	UIMenu* GetCurrentMenu();

	UIWidget* GetCurrentWidget();

private:
	MenuManager();

	std::vector<UIMenu*>mMenus;
	
	int mCurrentMenuID;
	int mPreviousMenuID;
};
