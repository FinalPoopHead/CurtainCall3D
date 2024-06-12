#include "Menu.h"

void Menu::AddItem(MenuItem* item)
{
	items.push_back(item);
}

MenuItem* Menu::NextItem(MenuItem* selectedItem)
{
	if (items.back() == selectedItem)
	{
		return items.front();
	}
	else
	{
		auto it = std::find(items.begin(), items.end(), selectedItem);
		return *(it + 1);
	}
}

MenuItem* Menu::PreviousItem(MenuItem* selectedItem)
{
	if (items.front() == selectedItem)
	{
		return items.back();
	}
	else
	{
		auto it = std::find(items.begin(), items.end(), selectedItem);
		return *(it - 1);
	}
}

MenuItem* Menu::FirstItem()
{
	if (items.empty())
	{
		return nullptr;
	}

	return items.front();
}
