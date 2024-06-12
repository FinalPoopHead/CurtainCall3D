#pragma once
#include "../FloaterGameEngine/include/EngineMinimal.h"

class Board;

class TestCamera : public flt::GameObject
{
public:
	TestCamera(Board* board);
	virtual ~TestCamera() {};

protected:
	virtual void Update(float deltaSecond) override;

private:
	Board* _board;
	float _speed;
};

