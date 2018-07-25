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

void MenuManager::ChangeToPreviousMenu()
{
	if (mCurrentMenuID != mPreviousMenuID)
		ChangeToMenu(mPreviousMenuID);
}

int MenuManager::AddMenu(UIMenu * menu)
{
	mMenus.emplace_back(menu);
	return mMenus.size();
}

void MenuManager::RemoveMenu(int ID)
{
	mMenus.erase(mMenus.begin()+ID);
}

void MenuManager::RemoveAllMenus()
{
	mMenus.clear();
}

void MenuManager::ShowCurrentMenu(bool isActive)
{
	mMenus[mCurrentMenuID]->SetActive(isActive);
}

UIMenu * MenuManager::GetCurrentMenu()
{
	if (mMenus.size())
		return mMenus[mCurrentMenuID];
	else
		return nullptr;
}

Button* MenuManager::GetCurrentButton()
{

	return mMenus[mCurrentMenuID]->GetCurrentButton();
}

MenuManager::MenuManager()
{
	instance = this;
}