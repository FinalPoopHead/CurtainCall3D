#pragma once
#include "../FloaterGameEngine/include/EngineMinimal.h"

enum class eCUBETYPE
{
	NORMAL,
	ADVANTAGE,
	DARK
};

enum class eCUBESTATUS
{
	NONE
	, ROLLING
	, FALLING
	, REMOVING
	, GENERATING		// 등장 연출
	, DROPPING
};

class Board;

class CubeController : public flt::Component<CubeController>
{
public:
	CubeController();
	~CubeController();

	void SetBoard(Board* board) { _board = board; }
	void SetCubeType(eCUBETYPE type) { this->_cubeType = type; }

	virtual void PreUpdate(float deltaSecond) override;
	virtual void OnDisable() override;
	bool StartRoll(float rotateTime);
	void StartFall(bool withDamage = true);
	void StartRemove(float removeTime);
	void StartGenerate(float riseTime, float delay);
	void StartDrop(float delay);
	bool IsRolling() { return _status == eCUBESTATUS::ROLLING; }
	flt::Vector4f GetPosition() { return _gameObject->tr.GetWorldPosition(); }
	eCUBETYPE GetCubeType() { return _cubeType; }
	flt::GameObject* GetGameObject() { return _gameObject; }
	void SetIsRunning(bool isRunning) { _isRunning = isRunning; }
	bool IsRunning() { return _isRunning; }

private:
	void Roll(float deltaSecond);
	void FinishRolling();			// Roll이 끝나면 마무리 각도 깔끔하게 하기 위함
	void Fall(float deltaSecond);
	bool IsOutofBoard();			// 보드를 벗어나는지 체크
	bool IsFallEnough();			// 충분히 떨어졌는지 체크
	bool IsDropEnough();
	void Removing(float deltaSecond);
	void Generating(float deltaSecond);
	void Dropping(float deltaSecond);

private:
	Board* _board;			// 게임의 바닥 보드.

	bool _isRunning;		// 큐브가 동작중인지 체크(웨이브 대기중인지)
	eCUBESTATUS _status;	// 큐브의 상태
	int _targetIndex;		// 회전 목표 각도 인덱스
	float _rotateSpeed;		// 회전 속도 -> 주어진 회전시간의 역수
	float _removeSpeed;		// 제거 속도 -> 주어진 제거시간의 역수
	float _generateSpeed;		// 상승 속도 -> 주어진 상승시간의 역수
	float _generateDelay;		// 상승 딜레이
	float _dropDelay;		// 떨어지기 딜레이
	float _elapsedTime;
	float _currentAngle;	// 현재 회전 각도
	flt::Vector3f _rotatePivot;	// 회전 중심
	float _fallSpeed;		// 떨어지는 속도
	eCUBETYPE _cubeType;
};

