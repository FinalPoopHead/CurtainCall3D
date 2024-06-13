#include "HPSlot.h"

HPSlot::HPSlot()
{
	std::wstring path = L"../Resources/Sprites/HPSlot.png";

	auto uiRenderer = AddComponent<flt::UIComponent>(true);
	uiRenderer->SetImage(path);
	uiRenderer->SetZOrder(0.2f);
	uiRenderer->SetOffsetPosition({ 0.0f, 0.0f });
}

HPSlot::~HPSlot()
{

}
