#include "CubeController.h"
#include "../FloaterGameEngine/include/Input.h"

constexpr float ROLLANGLE = 90.0f;		// 회전할 각도
constexpr float TARGETANGLE[4] = { 90.0f, 180.0f, 270.0f, 360.0f };	// 회전 목표 각도

CubeController::CubeController()
	: _isRolling(false)
	, _targetIndex(0)
	, _rotateSpeed(0.0f)
	, _currentAngle(0.0f)
	, _rotatePivot(0.0f, 0.0f, 0.0f)
{

}

CubeController::~CubeController()
{

}

void CubeController::Update(float deltaSecond)
{
	if (flt::GetKey(flt::KeyCode::spacebar))
	{
		StartRolling(1.0f);
	}

	if (!_isRolling)
	{
		return;
	}

	Roll(deltaSecond);
}

void CubeController::StartRolling(float rotateTime)
{
	_isRolling = true;

	flt::Vector4f pos = _gameObject->tr.GetWorldPosition();
	_rotatePivot = { pos.x, pos.y - 1.0f,pos.z + 1.0f };
	_rotateSpeed = 1 / rotateTime;

	int prevIndex = (_targetIndex + 3) % 4;
	_currentAngle = prevIndex == 3 ? 0.0f : TARGETANGLE[prevIndex];
}

void CubeController::Roll(float deltaSecond)
{
	float deltaAngle = _rotateSpeed * deltaSecond * ROLLANGLE;
	if (_currentAngle + deltaAngle >= TARGETANGLE[_targetIndex])
	{
		deltaAngle = TARGETANGLE[_targetIndex] - _currentAngle;
		_isRolling = false;
	}
	_currentAngle += deltaAngle;

	_gameObject->tr.SetRotation({ 1.0f,0.0f,0.0f }, flt::DegToRad(_currentAngle));

	if (!_isRolling)
	{
		FinishRolling();
	}
}


void CubeController::FinishRolling()
{
	_targetIndex++;
	_targetIndex %= 4;

	// 	// 정밀한 위치 조정
	// 	Vector3 finalPosition = transform.position;
	// 	finalPosition.x = Mathf.Round(finalPosition.x);
	// 	finalPosition.y = Mathf.Round(finalPosition.y);
	// 	finalPosition.z = Mathf.Round(finalPosition.z);
	// 	transform.position = finalPosition;
	// 
	// 	// 정밀한 회전 조정
	// 	Vector3 finalEulerAngles = transform.eulerAngles;
	// 	finalEulerAngles.x = Mathf.Round(finalEulerAngles.x / 90f) * 90f;
	// 	finalEulerAngles.y = Mathf.Round(finalEulerAngles.y / 90f) * 90f;
	// 	finalEulerAngles.z = Mathf.Round(finalEulerAngles.z / 90f) * 90f;
	// 	transform.eulerAngles = finalEulerAngles;
}
