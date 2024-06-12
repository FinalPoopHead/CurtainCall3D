#include "CubeController.h"
#include "../FloaterGameEngine/include/Input.h"

#include "Board.h"

constexpr float PIVOTSIZE = 2.0f;		// 피봇까지의 y,z 거리. 모델 사이즈에 맞게 변경해야함.
constexpr float ROLLANGLE = 90.0f;		// 회전할 각도
constexpr float TARGETANGLE[4] = { 90.0f, 180.0f, 270.0f, 360.0f };	// 회전 목표 각도
constexpr float GRAVITY = 9.8f;
constexpr float STARTFALLSPEED = 8.0f;
constexpr float FALLHEIGHT = -50.0f;

CubeController::CubeController()
	: _board(nullptr)
	, _isRolling(false)
	, _isFalling(false)
	, _targetIndex(0)
	, _rotateSpeed(0.0f)
	, _currentAngle(0.0f)
	, _rotatePivot(0.0f, 0.0f, 0.0f)
	, _fallSpeed(0.0f)
{

}

CubeController::~CubeController()
{

}

void CubeController::PreUpdate(float deltaSecond)
{
	if (_isRolling)
	{
		Roll(deltaSecond);
		return;
	}

	if(_isFalling)
	{
		Fall(deltaSecond);
		if(IsFallEnough())
		{
			_board->BackToPool(_gameObject);

			_isFalling = false;
			_isRolling = false;
		}
		return;
	}

	if (!_isRolling && IsOutofBoard())
	{
		// TODO : 체력 감소 또는 게임 오버 처리 필요.
		StartFalling();
	}
}

void CubeController::OnDisable()
{
	_board->RemoveFromControllerList(this);
}

void CubeController::StartRolling(float rotateTime)
{
	if (_isRolling || _isFalling)
	{
		return;
	}

	_isRolling = true;

	flt::Vector4f pos = _gameObject->tr.GetWorldPosition();
	_rotatePivot = { pos.x, pos.y - PIVOTSIZE, pos.z - PIVOTSIZE };
	_rotateSpeed = 1 / rotateTime;

	int prevIndex = (_targetIndex + 3) % 4;
	_currentAngle = prevIndex == 3 ? 0.0f : TARGETANGLE[prevIndex];
}

void CubeController::StartFalling()
{
	if (_isFalling || _isRolling)
	{
		return;
	}

	_isFalling = true;
	_fallSpeed = STARTFALLSPEED;
	_board->RemoveFromControllerList(this);
	_board->ReduceHPbyCubeFalling();
}

void CubeController::Roll(float deltaSecond)
{
	flt::Vector4f pos = _gameObject->tr.GetWorldPosition();
	flt::Vector4f pivot = {_rotatePivot.x, _rotatePivot.y, _rotatePivot.z, 1.0f};
	flt::Vector4f dir = pivot - pos;

	_gameObject->tr.AddWorldPosition(dir);

	float deltaAngle = _rotateSpeed * deltaSecond * ROLLANGLE;
	if (_currentAngle + deltaAngle >= TARGETANGLE[_targetIndex])
	{
		deltaAngle = TARGETANGLE[_targetIndex] - _currentAngle;
		_isRolling = false;
	}
	_currentAngle += deltaAngle;

	_gameObject->tr.SetRotation({ 1.0f,0.0f,0.0f }, flt::DegToRad(-_currentAngle));

	flt::Transform tr;
	tr.SetRotation({ 1.0f,0.0f,0.0f }, flt::DegToRad(-deltaAngle));
	dir = dir * tr.GetWorldMatrix4f();

	_gameObject->tr.AddWorldPosition(-dir);

	if (!_isRolling)
	{
		FinishRolling();
	}
}

void CubeController::FinishRolling()
{
	_targetIndex++;
	_targetIndex %= 4;
	_board->OnEndRolling();
}

void CubeController::Fall(float deltaSecond)
{
	_fallSpeed += GRAVITY * deltaSecond * _board->GetFFValue();
	flt::Vector4f pos = _gameObject->tr.GetWorldPosition();
	_gameObject->tr.AddWorldPosition( 0.0f, -_fallSpeed * deltaSecond, 0.0f);
}

bool CubeController::IsOutofBoard()
{
	auto pos = _gameObject->tr.GetWorldPosition();
	auto state = _board->QueryTileState(pos.x,pos.z);

	if(state == (int)TileStateFlag::None)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool CubeController::IsFallEnough()
{
	if (_gameObject->tr.GetWorldPosition().y <= FALLHEIGHT)
	{
		return true;
	}
	else
	{
		return false;
	}
}
