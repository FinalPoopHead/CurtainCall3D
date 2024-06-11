#pragma once
#include "../FloaterGameEngine/include/EngineMinimal.h"


class Player;
class Board;

class Camera : public flt::GameObject
{
public:
	Camera(Player* player, Board* board);
	virtual void PostUpdate(float deltaSecond) override;

private:
	Player* _player;
	Board* _board;
	float _height;
	float _playHeight;
	float _lookZOffset;
	float _posZOffsest;
	float _playerDistance;
	float _lookDegree;

	flt::Quaternion _targetRotation;
};
