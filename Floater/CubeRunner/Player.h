﻿#pragma once
#include "../FloaterGameEngine/include/EngineMinimal.h"

class Board;

class Player : public flt::GameObject
{
public:
	Player(Board* board);
	~Player() {}

	virtual void Update(float deltaSecond) override;

	void SetPadIndex(int padIndex) { _padIndex = padIndex; }

private:
	Board* _board;
	int _padIndex;
	float _speed;
};