#pragma once
#include "../FloaterGameEngine/include/EngineMinimal.h"

class Player;

class Camera : public flt::GameObject
{
public:
	Camera(Player* player);
	virtual void PostUpdate(float deltaSecond) override;

private:
	Player* _player;
	float _height;
};
