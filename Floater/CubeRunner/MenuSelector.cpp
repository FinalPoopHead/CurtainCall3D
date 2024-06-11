#include "MenuSelector.h"
#include "Menu.h"
#include "MenuItem.h"
#include "../FloaterGameEngine/include/Input.h"


MenuSelector::MenuSelector(Menu* menu) :
	_menu(menu)
{
	_selectedItem = _menu->FirstItem();
}

void MenuSelector::SetMenu(Menu* menu)
{
	_menu = menu;
	_selectedItem = _menu->FirstItem();
}

void MenuSelector::Select(flt::KeyCode keyCode)
{
	_selectedItem->Select(keyCode);
}

void MenuSelector::Update(float deltaSecond)
{
	if(flt::GetKeyDown(flt::KeyCode::enter))
	{
		Select(flt::KeyCode::enter);
	}
}
