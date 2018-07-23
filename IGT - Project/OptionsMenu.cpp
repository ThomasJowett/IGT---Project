#include "OptionsMenu.h"

OptionsMenu::OptionsMenu(Transform * transform)
	:UIMenu("Options Menu", transform, false)
{
	CreateButtons();
}

OptionsMenu::~OptionsMenu()
{
}

void OptionsMenu::CreateButtons()
{
}

void OptionsMenu::OnNotify(ButtonEvent event, int ID)
{
}
