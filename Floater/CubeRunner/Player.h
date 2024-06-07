#pragma once
#include "../FloaterGameEngine/include/EngineMinimal.h"

class Player : public flt::GameObject
{
public:
	Player();
	~Player() {}

	virtual void Update(float deltaSecond) override;

	void SetPadIndex(int padIndex) { _padIndex = padIndex; }

private:
	int _padIndex;
	float _speed;
};
