#include "MenuSelector.h"
#include "Menu.h"
#include "MenuItem.h"
#include "../FloaterGameEngine/include/Input.h"


MenuSelector::MenuSelector(Menu* menu) :
	_menu(menu)
{


	_ui = AddComponent<flt::UIComponent>(true);
	_ui->SetImage(L"../Resources/Textures/abcd.jpg");
	_ui->SetSize({ 10.0f, 10.0f });
	
	_selectedItem = _menu->FirstItem();

	MoveSelectedItem();
}

void MenuSelector::SetMenu(Menu* menu)
{
	_menu = menu;
	_selectedItem = _menu->FirstItem();
}

void MenuSelector::next()
{
	_selectedItem = _menu->NextItem(_selectedItem);

	MoveSelectedItem();
}

void MenuSelector::Select(flt::KeyCode keyCode)
{
	_selectedItem->Select(keyCode);
}

void MenuSelector::Update(float deltaSecond)
{
	if (flt::GetKeyDown(flt::KeyCode::right))
	{
		next();
	}
	if(flt::GetKeyDown(flt::KeyCode::enter))
	{
		Select(flt::KeyCode::enter);
	}

}

void MenuSelector::MoveSelectedItem()
{
	flt::Vector2f pos = _selectedItem->GetPosition();
	flt::Vector2f size = _selectedItem->GetSize();

	pos.x -= size.x / 2;

	const float offset = 10.0f;
	pos.x -= offset;

	_ui->SetPosition(pos);
}
