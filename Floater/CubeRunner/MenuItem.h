#pragma once
#include "../FloaterGameEngine/include/EngineMinimal.h"


class MenuItem : public flt::GameObject
{
public:
	MenuItem();
	~MenuItem();

	void SetSprite(const std::wstring& spriteName);
	void SetPosition(float pixelX, float pixelY);
	void SetSize(float pixelWidth, float pixelHeight);
	void GetSize(float& pixelWidth, float& pixelHeight);
	void Select(flt::KeyCode keyCode);

private:
	//flt::RendererComponent* _spriteRenderer;
	flt::UIComponent* _ui;
};
