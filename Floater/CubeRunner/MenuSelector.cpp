#include "MenuSelector.h"
#include "Menu.h"
#include "MenuItem.h"
#include "../FloaterGameEngine/include/Input.h"
#include <iostream>


MenuSelector::MenuSelector(Menu* mainMenu, Menu* controllerMenu)
	: _mainMenu(mainMenu)
	, _controllerSelectMenu(controllerMenu)
	, _selectedItem(nullptr)
	, _ui(nullptr)
	, _lastLStickY(0.0f)
{
	//_ui = AddComponent<flt::UIComponent>(true);
	//_ui->SetImage(L"../Resources/Sprites/abcd.jpg");
	//_ui->SetSize({ 50.0f, 50.0f });
	//_ui->SetZOrder(0.8f);
}

void MenuSelector::SetMenu(Menu* menu)
{
	_mainMenu = menu;

	if (_selectedItem)
	{
		_selectedItem->OnUnpointed();
	}
	_selectedItem = _mainMenu->FirstItem();
	_selectedItem->OnPointed();
}

void MenuSelector::next()
{
	if (_selectedItem)
	{
		_selectedItem->OnUnpointed();
	}

	_selectedItem = _mainMenu->NextItem(_selectedItem);
	_selectedItem->OnPointed();
	MoveSelectedItem();
}

void MenuSelector::prev()
{
	if (_selectedItem)
	{
		_selectedItem->OnUnpointed();
	}

	_selectedItem = _mainMenu->PrevItem(_selectedItem);
	_selectedItem->OnPointed();
	MoveSelectedItem();
}

void MenuSelector::Select(flt::KeyCode keyCode)
{
	_selectedItem->Select(keyCode);
}

void MenuSelector::OnEnable()
{
	_selectedItem = _mainMenu->FirstItem();
	_selectedItem->OnPointed();

	MoveSelectedItem();
}

void MenuSelector::OnDisable()
{
	if (_selectedItem)
	{
		_selectedItem->OnUnpointed();
	}
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
	if (flt::GetKeyDown(flt::KeyCode::enter))
	{
		Select(flt::KeyCode::enter);
	}

	if (flt::GetKeyDown(flt::KeyCode::lAlt))
	{
		if (_mainMenu->IsEnable())
		{
			_mainMenu->Disable();
			_controllerSelectMenu->Enable();
		}
		else
		{
			_mainMenu->Enable();
			_controllerSelectMenu->Disable();
		}
	}

	flt::GamePadState state;
	bool isGamePadConnected = flt::GetGamePadState(0, &state);
	if (isGamePadConnected)
	{
		if (state.buttonsDown & flt::GamePadState::ButtonFlag::UP)
		{
			prev();
		}
		if (state.buttonsDown & flt::GamePadState::ButtonFlag::DOWN)
		{
			next();
		}
		if (state.buttonsDown & flt::GamePadState::ButtonFlag::A)
		{
			Select(flt::KeyCode::enter);
		}

		if (fabsf(_lastLStickY) < 0.5f)
		{
			if (state.lStickY > 0.5f)
			{
				prev();
			}
			else if (state.lStickY < -0.5f)
			{
				next();
			}
		}

		_lastLStickY = state.lStickY;
	}
}

void MenuSelector::MoveSelectedItem()
{
	//flt::Vector2f pos = _selectedItem->GetPosition();
	//flt::Vector2f size = _selectedItem->GetSize();

	//pos.x -= size.x / 2;

	//const float offset = 30.0f;
	//pos.x -= offset;

	//_ui->SetPosition(pos);
}
