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

	virtual void OnEnable() override;
	virtual void OnDisable() override;

private:
	std::vector<MenuItem*> items;
};
