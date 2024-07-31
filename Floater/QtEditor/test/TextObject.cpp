#include "TextObject.h"

TextObject::TextObject()
{
	_uiRenderer = AddComponent<flt::UIComponent>(true);
}

TextObject::~TextObject()
{

}

void TextObject::SetText(const std::wstring& text)
{
	_uiRenderer->SetText(text);
}

void TextObject::SetFont(const std::wstring& fontPath)
{
	_uiRenderer->SetFont(fontPath);
}

void TextObject::SetFontSize(int fontSize)
{
	_uiRenderer->SetFontSize(fontSize);
}

void TextObject::SetTextColor(flt::Vector4f color)
{
	_uiRenderer->SetTextColor(color);
}

void TextObject::SetTextColor(float r, float g, float b)
{
	_uiRenderer->SetTextColor(r, g, b);
}

void TextObject::SetTextAlignment(eTextAlignment alignment)
{
	_uiRenderer->SetTextAlignment((flt::eTextAlignment)alignment);
}

void TextObject::SetPosition(flt::Vector2f pos)
{
	_uiRenderer->SetPosition(pos);
}

void TextObject::SetOffsetPosition(flt::Vector2f offsetPos)
{
	_uiRenderer->SetOffsetPosition(offsetPos);
}

flt::Vector2f TextObject::GetPosition()
{
	return _uiRenderer->GetPosition();
}

flt::Vector2f TextObject::GetOffsetPosition()
{
	return _uiRenderer->GetOffsetPosition();
}
