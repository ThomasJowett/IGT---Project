#pragma once
#include "Messaging.h"
#include "Button.h"


class ButtonManager :
	public Observer<ButtonEvent, int>
{
public:
	ButtonManager();
	~ButtonManager();

	void OnNotify(ButtonEvent event, int ID)override;
};

