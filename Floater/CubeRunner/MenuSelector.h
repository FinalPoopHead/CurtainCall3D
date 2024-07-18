#pragma once
#include "../FloaterGameEngine/include/EngineMinimal.h"


class Menu;
class MenuItem;

class MenuSelector : public flt::GameObject
{
public:
	MenuSelector(Menu* mainMenu, Menu* controllerMenu);
	void SetMenu(Menu* mainMenu);
	void next();
	void prev();
	void Select(flt::KeyCode keyCode);

protected:
	void OnEnable() override;
	void OnDisable() override;
	void Update(float deltaSecond) override;

private:
	void MoveSelectedItem();

private:
	Menu* _mainMenu;
	Menu* _controllerSelectMenu;
	MenuItem* _selectedItem;
	flt::UIComponent* _ui;
	float _lastLStickY;
};

