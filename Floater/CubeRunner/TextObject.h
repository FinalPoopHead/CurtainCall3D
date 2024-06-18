#pragma once
#include "../FloaterGameEngine/include/EngineMinimal.h"

class TextObject : public flt::GameObject
{
public:
	TextObject();
	~TextObject();

	void SetText(const std::wstring& text);
	void SetFont(const std::wstring& fontPath);
	void SetFontSize(int fontSize);
	void SetTextColor(flt::Vector4f color);
	void SetTextColor(float r, float g, float b);

	void SetPosition(flt::Vector2f pos);
	void SetOffsetPosition(flt::Vector2f offsetPos);

	flt::Vector2f GetPosition();
	flt::Vector2f GetOffsetPosition();

private:
	flt::UIComponent* _uiRenderer;
};

