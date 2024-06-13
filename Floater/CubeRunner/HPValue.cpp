#include "HPValue.h"

HPValue::HPValue()
{
	std::wstring path = L"../Resources/Sprites/HPValue.png";

	auto uiRenderer = AddComponent<flt::UIComponent>(true);
	uiRenderer->SetImage(path);
	uiRenderer->SetZOrder(0.3f);
	uiRenderer->SetOffsetPosition({ 0.0f, 0.0f });
}

HPValue::~HPValue()
{

}
