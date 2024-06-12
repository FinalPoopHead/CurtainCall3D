#pragma once
#include "../FloaterGameEngine/include/EngineMinimal.h"


class Menu;
class MenuItem;

class MenuSelector : public flt::GameObject
{
public:
	MenuSelector(Menu* menu);
	void SetMenu(Menu* menu);
	void next();
	void Select(flt::KeyCode keyCode);

protected:
	void Update(float deltaSecond) override;

private:
	void MoveSelectedItem();

private:
	Menu* _menu;
	MenuItem* _selectedItem;
	flt::UIComponent* _ui;
};

