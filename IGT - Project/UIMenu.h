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

protected:
	std::vector<Button*> mButtons;
};