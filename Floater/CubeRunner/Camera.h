#pragma once
#include "../FloaterGameEngine/include/EngineMinimal.h"


class Player;
class Board;

class Camera : public flt::GameObject
{
public:
	Camera(Player* player, Board* board);

	void TracePlayer();
	//void 

protected:
	virtual void PostUpdate(float deltaSecond) override;

private:
	flt::Quaternion CalcTargetRotation();
	flt::Vector3f CalcTargetPosition();

	bool CheckCameraMove();
	void StartCameraMove();
	void UpdateCameraMove(float deltaSecond);

private:
	Player* _player;
	Board* _board;
	float _height;
	float _playHeight;
	float _lookZOffset;
	float _posZOffsest;
	float _playerDistance;
	float _lookDegree;
	float _movSpeed;
	float _rotSpeed;

	bool _isMoving;
	bool _isPlayerLook;

	flt::Quaternion _currRotation;

	flt::Vector3f _currPosition;

};
