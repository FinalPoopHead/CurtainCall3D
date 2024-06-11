#pragma once
#include "../FloaterGameEngine/include/EngineMinimal.h"


class MenuItem : public flt::GameObject
{
public:
	MenuItem();
	~MenuItem();

	void SetSprite(const std::wstring& spriteName);
	void Select(flt::KeyCode keyCode);

private:
	flt::RendererComponent* _spriteRenderer;
};
