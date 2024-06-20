#pragma once
#include "../FloaterGameEngine/include/EngineMinimal.h"

class Board;
class PlayerModel;

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
	PlayerModel* _model;
	Board* _board;
	bool _isGameOver;
	bool _isCrushed;
	int _padIndex;
	float _speed;
};
