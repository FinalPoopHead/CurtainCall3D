#include "MenuSelector.h"
#include "Menu.h"
#include "MenuItem.h"
#include "RankViewer.h"
#include "../FloaterGameEngine/include/Input.h"
#include <iostream>

#include "../FloaterGameEngine/include/internal/GameEngine.h"

MenuSelector::MenuSelector(Menu* mainMenu, Menu* controllerMenu)
	: _title(nullptr)
	, _mainMenu(mainMenu)
	, _controllerSelectMenu(controllerMenu)
	, _rankViewer(nullptr)
	, _selectedItem(nullptr)
	, _ui(nullptr)
	, _lastLStickY{0.0f, 0.0f}
	, _mode(Mode::MainMenu)
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
	if (!_selectedItem->IsEnable())
	{
		return;
	}

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
	if (!_selectedItem->IsEnable())
	{
		return;
	}

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
	if (!_selectedItem->IsEnable())
	{
		return;
	}

	_selectedItem->Select(keyCode);
}

void MenuSelector::OnEnable()
{
	SetMainMenuMode();

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
	switch (_mode)
	{
		case MenuSelector::Mode::MainMenu:
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

			for (int i = 0; i < 2; ++i)
			{
				flt::GamePadState state;
				bool isGamePadConnected = flt::GetGamePadState(i, &state);
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

					if (fabsf(_lastLStickY[i]) < 0.5f)
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

					_lastLStickY[i] = state.lStickY;
				}
			}
			
		}
		break;
		case MenuSelector::Mode::RankView:
		{
			if (flt::GetKeyDown(flt::KeyCode::enter))
			{
				SetMainMenuMode();
			}

			for (int i = 0; i < 2; ++i)
			{
				flt::GamePadState state;
				bool isGamePadConnected = flt::GetGamePadState(i, &state);
				if (isGamePadConnected)
				{
					if (state.buttonsDown & flt::GamePadState::ButtonFlag::A)
					{
						SetMainMenuMode();
					}
				}
			}

		}
		break;
		case MenuSelector::Mode::ControllerSelect:
		{

		}
		break;
		default:
			break;
	}


	//if (flt::GetKeyDown(flt::KeyCode::lAlt))
	//{
	//	if (_mainMenu->IsEnable())
	//	{
	//		_mainMenu->Disable();
	//		_controllerSelectMenu->Enable();
	//	}
	//	else
	//	{
	//		_mainMenu->Enable();
	//		_controllerSelectMenu->Disable();
	//	}
	//}

	//if(flt::GetKeyDown(flt::KeyCode::lCtrl))
	//{
	//	flt::GameEngine::Instance()->SetWindowSize(1280, 720, flt::WindowMode::FULLSCREEN);
	//}

	//if(flt::GetKeyDown(flt::KeyCode::lShift))
	//{
	//	flt::GameEngine::Instance()->SetWindowSize(1280, 720, flt::WindowMode::WINDOWED);
	//}

	//if (flt::GetKeyDown(flt::KeyCode::capslock))
	//{
	//	flt::GameEngine::Instance()->SetWindowSize(0, 0, flt::WindowMode::BORDERLESS);
	//}


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

void MenuSelector::SetMainMenuMode()
{
	_title->Enable();
	_mainMenu->Enable();
	_rankViewer->Disable();
	_controllerSelectMenu->Disable();
	_mode = Mode::MainMenu;
}

void MenuSelector::SetViewRankMode()
{
	_title->Disable();
	_mainMenu->Disable();
	_controllerSelectMenu->Disable();
	_rankViewer->Enable();
	_mode = Mode::RankView;

}

void MenuSelector::SetControllerSelectMode()
{
	_title->Disable();
	_mainMenu->Disable();
	_rankViewer->Disable();
	_controllerSelectMenu->Enable();
	_mode = Mode::ControllerSelect;
}


