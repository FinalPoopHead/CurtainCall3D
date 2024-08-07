﻿#include "MenuItem.h"
#include "GameScene.h"


MenuItem::MenuItem()
	: _defaultSpriteName()
	, _pointedSpriteName()
	, _ui(nullptr)
	, _selectFunc([]() {})
	, _tween(nullptr)
{
	_ui = AddComponent<flt::UIComponent>(true);
	_ui->SetZOrder(0.4f);
}

MenuItem::~MenuItem()
{

}

void MenuItem::SetSprite(const std::wstring& spriteName)
{
	_ui->SetImage(spriteName);
}

void MenuItem::SetPointedSpritePath(const std::wstring& spriteName)
{
	_pointedSpriteName = spriteName;
}

void MenuItem::SetDefaultSpritePath(const std::wstring& spriteName)
{
	_defaultSpriteName = spriteName;
	SetSprite(spriteName);
}

void MenuItem::SetPosition(flt::Vector2f pos)
{
	_ui->SetPosition(pos);
}

flt::Vector2f MenuItem::GetPosition()
{
	return _ui->GetPosition();
}

void MenuItem::SetOffsetPosition(flt::Vector2f offsetPos)
{
	_ui->SetOffsetPosition(offsetPos);
}

flt::Vector2f MenuItem::GetOffsetPosition()
{
	return _ui->GetOffsetPosition();
}

void MenuItem::SetZOrder(float zOrder)
{
	ASSERT( zOrder >= 0.2f && zOrder <= 0.8f, "ZOrder must be between 0.2f and 0.8f");
	_ui->SetZOrder(zOrder);
}

float MenuItem::GetZOrder()
{
	return _ui->GetZOrder();
}

void MenuItem::SetSize(flt::Vector2f size)
{
	_ui->SetSize(size);
}

flt::Vector2f MenuItem::GetSize()
{
	return _ui->GetSize();
}

void MenuItem::SetText(const std::wstring& text)
{
	_ui->SetText(text);
}

void MenuItem::SetFont(const std::wstring& fontPath)
{
	_ui->SetFont(fontPath);
}

void MenuItem::SetFontSize(int fontSize)
{
	_ui->SetFontSize(fontSize);
}

void MenuItem::SetTextColor(flt::Vector4f color)
{
	_ui->SetTextColor(color);
}

void MenuItem::SetTextColor(float r, float g, float b)
{
	_ui->SetTextColor(r, g, b);
}

void MenuItem::OnPointed()
{
	SetSprite(_pointedSpriteName);
}

void MenuItem::OnUnpointed()
{
	SetSprite(_defaultSpriteName);
}

void MenuItem::Select(flt::KeyCode keyCode)
{
	if (keyCode == flt::KeyCode::enter)
	{
		_selectFunc();
	}
}

void MenuItem::OnEnable()
{
	if (_tween.get())
	{
		flt::StartTween(_tween);
	}
}
