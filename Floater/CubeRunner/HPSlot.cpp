#include "HPSlot.h"

HPSlot::HPSlot()
{
	std::wstring path = L"../Resources/Sprites/HPSlot.png";

	auto uiRenderer = AddComponent<flt::UIComponent>(true);
	uiRenderer->SetImage(path);
	uiRenderer->SetZOrder(0.2f);
}

HPSlot::~HPSlot()
{

}
