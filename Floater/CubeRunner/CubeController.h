#pragma once
#include "../FloaterGameEngine/include/EngineMinimal.h"

class CubeController : public flt::Component<CubeController>
{
public:
	CubeController();
	~CubeController();

	void Update(float deltaSecond) override;
	void StartRolling(float rotateTime);

private:
	void Roll(float deltaSecond);
	void FinishRolling();			// Roll이 끝나면 마무리 각도 깔끔하게 하기 위함

private:
	bool _isRolling;		// 회전 중인지 여부
	int _targetIndex;		// 회전 목표 각도 인덱스
	float _rotateSpeed;		// 회전 속도 -> 주어진 회전시간의 역수
	float _currentAngle;	// 현재 회전 각도
	flt::Vector3f _rotatePivot;	// 회전 중심
};

