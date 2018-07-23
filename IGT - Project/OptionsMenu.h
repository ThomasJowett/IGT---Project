#pragma once
#include "UIMenu.h"
class OptionsMenu :
	public UIMenu
{
public:
	OptionsMenu(Transform* transform);
	virtual ~OptionsMenu();

	void CreateButtons()override;
	void OnNotify(ButtonEvent event, int ID)override;
};

