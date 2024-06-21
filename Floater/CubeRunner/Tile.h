#pragma once
#include "../FloaterGameEngine/include/EngineMinimal.h"

class Board;

class Tile : public flt::GameObject
{
public:
	Tile(Board* board);
	virtual ~Tile();

protected:
	virtual void OnCreate() override;
	virtual void OnEnable() override;
	virtual void OnDisable() override;
	virtual void OnDestroy() override;
	virtual void Update(float deltaSecond) override;

public:
	void EnableMine();
	void DisableMine();

	void EnableDetonated();
	void DisableDetonated();

	void EnableAdvantageMine();
	void DisableAdvantageMine();

	void StartAddMoving(float movingTime, flt::Vector3f targetPos);

public:
	Board* _board;
	flt::GameObject* _cube;				// 현재 타일에 있는 큐브
	flt::GameObject* _mine;				// 지뢰
	flt::GameObject* _advantageMine;	// 어드밴 지뢰
	flt::GameObject* _detonated;		// 폭파예정 지뢰
	bool _isMoving;						// 이동중인지
	float _movingTime;					// 이동시간
	float _elapsedTime;
	flt::Vector3f _startPos;			// 시작 위치
	flt::Vector3f _targetPos;			// 이동할 위치
};
