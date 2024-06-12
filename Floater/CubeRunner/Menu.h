#pragma once
#include "../FloaterGameEngine/include/EngineMinimal.h"
#include <vector>


class MenuItem;

class Menu : public flt::GameObject
{
public:
	void AddItem(MenuItem* item);
	MenuItem* NextItem(MenuItem* selectedItem);
	MenuItem* PrevItem(MenuItem* selectedItem);
	MenuItem* FirstItem();

private:
	std::vector<MenuItem*> items;
};
