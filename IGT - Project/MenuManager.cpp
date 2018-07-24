#include "MenuManager.h"

static MenuManager* instance = 0;

MenuManager::~MenuManager()
{
}

MenuManager * MenuManager::GetInstance()
{
	if (instance == 0)
	{
		instance = new MenuManager();
	}
	return instance;
}

void MenuManager::ChangeToMenu(int menu)
{
	if (menu != mCurrentMenuID)
	{
		mPreviousMenuID = mCurrentMenuID;
		mMenus[mCurrentMenuID]->SetActive(false);
		mCurrentMenuID = menu;
		mMenus[mCurrentMenuID]->SetActive(true);
	}
}

int MenuManager::AddMenu(UIMenu * menu)
{
	mMenus.emplace_back(menu);
	return mMenus.size();
}

UIMenu * MenuManager::GetCurrentMenu()
{
	if (mMenus.size())
		return mMenus[mCurrentMenuID];
	else
		return nullptr;
}

MenuManager::MenuManager()
{
	instance = this;
}