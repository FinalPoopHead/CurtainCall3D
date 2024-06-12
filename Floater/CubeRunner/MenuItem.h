#pragma once
#include "../FloaterGameEngine/include/EngineMinimal.h"
#include <functional>


class MenuItem : public flt::GameObject
{
public:
	MenuItem();
	~MenuItem();

	void SetSprite(const std::wstring& spriteName);
	void SetPosition(flt::Vector2f pos);
	flt::Vector2f GetPosition();
	void SetZOrder(float zOrder);
	float GetZOrder();
	void SetSize(flt::Vector2f size);
	flt::Vector2f GetSize();
	void Select(flt::KeyCode keyCode);

	void SetSelectFunc(const std::function<void()>& func) { _selectFunc = func; }

private:
	//flt::RendererComponent* _spriteRenderer;
	flt::UIComponent* _ui;
	std::function<void()> _selectFunc;
};
