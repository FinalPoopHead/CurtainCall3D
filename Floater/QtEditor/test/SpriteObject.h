#pragma once
#include "../FloaterGameEngine/include/EngineMinimal.h"

class SpriteObject : public flt::GameObject
{
public:
	SpriteObject();
	~SpriteObject();

	void SetSprite(const std::wstring& spriteName);
	void SetPosition(flt::Vector2f pos);
	void SetOffsetPosition(flt::Vector2f offsetPos);
	void SetSize(flt::Vector2f size);
	void SetZOrder(float zOrder);
	void SetColor(flt::Vector4f color);

	flt::Vector2f GetPosition();
	flt::Vector2f GetOffsetPosition();
	flt::Vector2f GetSize();
	float GetZOrder();


private:
	flt::UIComponent* _uiRenderer;
};

