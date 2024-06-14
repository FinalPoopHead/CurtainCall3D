#include "SpriteObject.h"

SpriteObject::SpriteObject()
{
	_uiRenderer = AddComponent<flt::UIComponent>(true);
}

SpriteObject::~SpriteObject()
{

}

void SpriteObject::SetSprite(const std::wstring& spriteName)
{
	_uiRenderer->SetImage(spriteName);
}

void SpriteObject::SetPosition(flt::Vector2f pos)
{
	_uiRenderer->SetPosition(pos);
}

void SpriteObject::SetOffsetPosition(flt::Vector2f offsetPos)
{
	_uiRenderer->SetOffsetPosition(offsetPos);
}

void SpriteObject::SetZOrder(float zOrder)
{
	_uiRenderer->SetZOrder(zOrder);
}

void SpriteObject::SetSize(flt::Vector2f size)
{
	_uiRenderer->SetSize(size);
}

flt::Vector2f SpriteObject::GetPosition()
{
	return _uiRenderer->GetPosition();
}

flt::Vector2f SpriteObject::GetOffsetPosition()
{
	return _uiRenderer->GetOffsetPosition();
}

flt::Vector2f SpriteObject::GetSize()
{
	return _uiRenderer->GetSize();
}

float SpriteObject::GetZOrder()
{
	return _uiRenderer->GetZOrder();
}
