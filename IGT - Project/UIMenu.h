#pragma once
#include "GameObject.h"
#include "Messaging.h"
#include "Button.h"
#include "MenuManager.h"

class MenuManager;

class UIMenu :
	public GameObject, public Observer<ButtonEvent, int>
{
public:
	UIMenu(const char* name, Transform* transform);
	UIMenu(const char* name, Transform* transform, bool active);
	virtual ~UIMenu();

	virtual void CreateButtons() = 0;
	virtual void OnNotify(ButtonEvent event, int ID)override = 0;
	
	void Update(float deltaTime)override;
	void Render(Shader* shader)override;

	std::vector<Button*> GetButtons() { return mButtons; }
	Button* GetCurrentButton() { return mButtons[mCurrentButton]; }
	void SetCurrentButton(int button);

	//Navigation of the menu;
	virtual void Up();
	virtual void Down();
	virtual void Left() {}
	virtual void Right() {}
	virtual void Back();
	virtual void Next() {}
	virtual void Previous() {}

protected:
	std::vector<Button*> mButtons;

	int mCurrentButton;

	
};