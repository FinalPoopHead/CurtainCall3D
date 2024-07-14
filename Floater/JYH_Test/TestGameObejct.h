#pragma once
#include "../FloaterGameEngine/include/internal/GameObject.h"

class TestGameObejct : public flt::GameObject
{
public:
	TestGameObejct();
	virtual ~TestGameObejct();

	void OnEnable() override;
	void Update(float deltaTime) override;

private:
	float _elapsedTime = 0.0f;
};

