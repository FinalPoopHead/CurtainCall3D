#pragma once
#include "../FloaterGameEngine/include/EngineMinimal.h"

class Board;

class Player : public flt::GameObject
{
public:
	Player(Board* board);
	~Player() {}

protected:
	virtual void OnEnable() override;
	virtual void Update(float deltaSecond) override;

public:
	void SetPadIndex(int padIndex) { _padIndex = padIndex; }
	void SetGameOver();

private:
	Board* _board;
	bool _isGameOver;
	int _padIndex;
	float _speed;
};
