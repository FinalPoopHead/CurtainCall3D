#include "Menu.h"
#include "MenuItem.h"
#include "../FloaterGameEngine/include/MakeTween.h"


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

MenuItem* Menu::PrevItem(MenuItem* selectedItem)
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

void Menu::OnEnable()
{
	for (auto& item : items)
	{
		item->Enable();
		flt::Vector2f pos = item->GetOffsetPosition();
		auto tween = flt::MakeTween(flt::Vector2f{ 1.0f, pos.y });
		tween->to({ pos.x, pos.y }).during(1.0f).easing(flt::ease::easeOutElastic).onStep([item](const flt::Vector2f& pos) {item->SetOffsetPosition(pos); });
		flt::StartTween(tween);
		item->Enable();
	}
}

void Menu::OnDisable()
{
	for (auto& item : items)
	{
		item->Disable();
	}
}
