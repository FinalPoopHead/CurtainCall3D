#include "MenuSelector.h"
#include "Menu.h"
#include "MenuItem.h"
#include "../FloaterGameEngine/include/Input.h"
#include <iostream>


MenuSelector::MenuSelector(Menu* menu) :
	_menu(menu)
{


	_ui = AddComponent<flt::UIComponent>(true);
	_ui->SetImage(L"../Resources/Sprites/abcd.jpg");
	_ui->SetSize({ 50.0f, 50.0f });
	_ui->SetZOrder(0.8f);
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

void MenuSelector::prev()
{
	_selectedItem = _menu->PrevItem(_selectedItem);
	MoveSelectedItem();
}

void MenuSelector::Select(flt::KeyCode keyCode)
{
	_selectedItem->Select(keyCode);
}

void MenuSelector::OnEnable()
{
	_selectedItem = _menu->FirstItem();
	MoveSelectedItem();
}

void MenuSelector::Update(float deltaSecond)
{
	if (flt::GetKeyDown(flt::KeyCode::right) || flt::GetKeyDown(flt::KeyCode::down))
	{
		next();
	}
	if (flt::GetKeyDown(flt::KeyCode::left) || flt::GetKeyDown(flt::KeyCode::up))
	{
		prev();
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

	const float offset = 30.0f;
	pos.x -= offset;

	_ui->SetPosition(pos);
}
