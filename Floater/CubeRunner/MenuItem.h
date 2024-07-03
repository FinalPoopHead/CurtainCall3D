#pragma once
#include "../FloaterGameEngine/include/EngineMinimal.h"
#include <functional>


class MenuItem : public flt::GameObject
{
public:
	MenuItem();
	~MenuItem();

	void SetSprite(const std::wstring& spriteName);
	void SetPointedSpritePath(const std::wstring& spriteName);
	void SetDefaultSpritePath(const std::wstring& spriteName);
	void SetPosition(flt::Vector2f pos);
	flt::Vector2f GetPosition();
	void SetOffsetPosition(flt::Vector2f offsetPos);
	flt::Vector2f GetOffsetPosition();
	void SetZOrder(float zOrder);
	float GetZOrder();
	void SetSize(flt::Vector2f size);
	flt::Vector2f GetSize();

	void SetText(const std::wstring& text);
	void SetFont(const std::wstring& fontPath);
	void SetFontSize(int fontSize);
	void SetTextColor(flt::Vector4f color);
	void SetTextColor(float r, float g, float b);

	void OnPointed();
	void OnUnpointed();
	void Select(flt::KeyCode keyCode);

	void SetSelectFunc(const std::function<void()>& func) { _selectFunc = func; }

private:
	//flt::RendererComponent* _spriteRenderer;
	std::wstring _defaultSpriteName;
	std::wstring _pointedSpriteName;
	flt::UIComponent* _ui;
	std::function<void()> _selectFunc;
};
