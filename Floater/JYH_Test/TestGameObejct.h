#pragma once
#include "../FloaterGameEngine/include/internal/GameObject.h"

class TestGameObejct : public flt::GameObject
{
public:
	TestGameObejct();
	virtual ~TestGameObejct();

	void Update(float deltaTime) override;
};

