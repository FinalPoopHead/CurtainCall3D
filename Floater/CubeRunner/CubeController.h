#pragma once
#include "../FloaterGameEngine/include/EngineMinimal.h"

class CubeController : public flt::Component<CubeController>
{
public:
	CubeController();
	~CubeController();

	void Update(float deltaSecond) override;

private:
	float _delayTime;		// 회전 사이의 딜레이 시간
	float _rotateTime;		// 회전에 걸리는 시간
	float _elapsedTime;		// 경과 시간
	float _startEuler;		// 회전 시작 각도
	float _targetEuler;		// 회전 목표 각도
	flt::Vector3f _rotatePivot;	// 회전 중심
};

