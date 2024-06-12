#pragma once
#include "../FloaterGameEngine/include/EngineMinimal.h"


class MenuItem : public flt::GameObject
{
public:
	MenuItem();
	~MenuItem();

	void SetSprite(const std::wstring& spriteName);
	void SetPosition(flt::Vector2f pos);
	flt::Vector2f GetPosition();
	void SetSize(flt::Vector2f size);
	flt::Vector2f GetSize();
	void Select(flt::KeyCode keyCode);

private:
	//flt::RendererComponent* _spriteRenderer;
	flt::UIComponent* _ui;
};
