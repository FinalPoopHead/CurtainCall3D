#pragma once
#include "../FloaterGameEngine/include/EngineMinimal.h"


class TestCamera : public flt::GameObject
{
public:
	TestCamera();
	virtual ~TestCamera() {};

	virtual void Update(float deltaSecond) override;

private:
	float _speed;
};

