#pragma once
#include "../FloaterGameEngine/include/EngineMinimal.h"

class Board;

class CubeController : public flt::Component<CubeController>
{
public:
	CubeController();
	~CubeController();

	void SetBoard(Board* board) { _board = board; }

	virtual void Update(float deltaSecond) override;
	virtual void OnDisable() override;
	void StartRolling(float rotateTime);
	void StartFalling();

private:
	void Roll(float deltaSecond);
	void FinishRolling();			// Roll이 끝나면 마무리 각도 깔끔하게 하기 위함
	void Fall(float deltaSecond);
	bool IsOutofBoard();			// 보드를 벗어나는지 체크
	bool IsFallEnough();			// 충분히 떨어졌는지 체크

private:
	Board* _board;			// 게임의 바닥 보드. 사실 상 게임매니저.

	bool _isRolling;		// 회전 중인지 여부
	bool _isFalling;		// 떨어지는 중인지 여부
	int _targetIndex;		// 회전 목표 각도 인덱스
	float _rotateSpeed;		// 회전 속도 -> 주어진 회전시간의 역수
	float _currentAngle;	// 현재 회전 각도
	flt::Vector3f _rotatePivot;	// 회전 중심
	float _fallSpeed;		// 떨어지는 속도
};

