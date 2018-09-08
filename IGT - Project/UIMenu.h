#pragma once
#include "GameObject.h"
#include "Messaging.h"
#include "UIWidget.h"
#include "Button.h"
#include "MenuManager.h"

class MenuManager;

class UIMenu :
	public GameObject, public Observer<WidgetEvent, WidgetEventData>
{
public:
	UIMenu(const char* name, Transform* transform);
	UIMenu(const char* name, Transform* transform, bool active);
	virtual ~UIMenu();

	virtual void CreateWidgets() = 0;
	virtual void OnNotify(WidgetEvent event, WidgetEventData data)override = 0;
	
	void Update(float deltaTime)override;
	void Render(Shader* shader)override;

	std::vector<UIWidget*> GetWidgets() { return mWidgets; }
	UIWidget* GetCurrentWidget() { return mWidgets[mCurrentWidget]; }
	void SetCurrentWidget(unsigned int widgetID);

	//Navigation of the menu;
	virtual void Up();
	virtual void Down();
	virtual void Left() {}
	virtual void Right() {}
	virtual void Back();
	virtual void Next() {}
	virtual void Previous() {}

protected:
	std::vector<UIWidget*> mWidgets;

	int mCurrentWidget;
};