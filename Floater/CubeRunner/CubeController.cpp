#include "CubeController.h"
#include "../FloaterGameEngine/include/Input.h"

#include "Board.h"

constexpr float PIVOTSIZE = 2.0f;		// 피봇까지의 y,z 거리. 모델 사이즈에 맞게 변경해야함.
constexpr float ROLLANGLE = 90.0f;		// 회전할 각도
constexpr float TARGETANGLE[4] = { 90.0f, 180.0f, 270.0f, 360.0f };	// 회전 목표 각도
constexpr float GRAVITY = 9.8f;
constexpr float STARTFALLSPEED = 20.0f;
constexpr float FALLHEIGHT = -50.0f;
constexpr float DISTANCE = 4.0f;
constexpr double OVERLAPSCALE = 0.98;

CubeController::CubeController()
	: _board(nullptr)
	, _status(eCUBESTATUS::NONE)
	, _targetIndex(0)
	, _rotateSpeed(0.0f)
	, _removeSpeed(0.0f)
	, _riseSpeed(0.0f)
	, _riseDelay(0.0f)
	, _elapsedTime(0.0f)
	, _currentAngle(0.0f)
	, _rotatePivot(0.0f, 0.0f, 0.0f)
	, _fallSpeed(0.0f)
	, _cubeType(eCUBETYPE::NORMAL)
{

}

CubeController::~CubeController()
{

}

void CubeController::PreUpdate(float deltaSecond)
{
	switch (_status)
	{
	case eCUBESTATUS::ROLLING:
		Roll(deltaSecond * _board->GetFFValue());
		break;
	case eCUBESTATUS::FALLING:
		Fall(deltaSecond * _board->GetFFValue());
		break;
	case eCUBESTATUS::REMOVING:
		Removing(deltaSecond * _board->GetFFValue());
		break;
	case eCUBESTATUS::RISING:
		Rising(deltaSecond * _board->GetFFValue());
		break;
	}
}

void CubeController::OnDisable()
{
	_board->RemoveFromControllerList(this);
}

bool CubeController::StartRolling(float rotateTime)
{
	if (_status != eCUBESTATUS::NONE)
	{
		return false;
	}

	_status = eCUBESTATUS::ROLLING;

	flt::Vector4f pos = _gameObject->tr.GetWorldPosition();
	_rotatePivot = { pos.x, pos.y - PIVOTSIZE, pos.z - PIVOTSIZE };
	_rotateSpeed = 1 / rotateTime;

	int prevIndex = (_targetIndex + 3) % 4;
	_currentAngle = prevIndex == 3 ? 0.0f : TARGETANGLE[prevIndex];

	return true;
}

void CubeController::StartFalling(bool withDamage /*= true*/)
{
	if (_status != eCUBESTATUS::NONE)
	{
		return;
	}

	_status = eCUBESTATUS::FALLING;
	_fallSpeed = STARTFALLSPEED;

	if (withDamage)
	{
		if (_cubeType != eCUBETYPE::DARK)
		{
			_board->AddCubeFallCount();
		}
	}
}

void CubeController::StartRemoving(float removeTime)
{
	// 치트키 사용시 바로 제거시키기 위해 rolling과 rising 카운트 제거해주는 것
	switch (_status)
	{
	case eCUBESTATUS::ROLLING:
		_board->OnEndRolling();
		break;
	case eCUBESTATUS::RISING:
		_board->OnEndRising();
		break;
	default:
		break;
	}

	_gameObject->tr.SetScale(OVERLAPSCALE, OVERLAPSCALE, OVERLAPSCALE);
	_removeSpeed = 1.0f / removeTime;

	_status = eCUBESTATUS::REMOVING;
	// _board->RemoveFromControllerList(this);
}

void CubeController::StartRising(float riseTime, float delay)
{
	_riseSpeed = 1.0f / riseTime;
	_riseDelay = delay;
	_elapsedTime = 0.0f;

	_gameObject->tr.SetScale(OVERLAPSCALE, OVERLAPSCALE, OVERLAPSCALE);
	_gameObject->tr.SetWorldRotation({ 0.0f,0.0f,0.0f,1.0f });

	_status = eCUBESTATUS::RISING;
}

void CubeController::Roll(float deltaSecond)
{
	flt::Vector4f pos = _gameObject->tr.GetWorldPosition();
	flt::Vector4f pivot = { _rotatePivot.x, _rotatePivot.y, _rotatePivot.z, 1.0f };
	flt::Vector4f dir = pivot - pos;

	_gameObject->tr.AddWorldPosition(dir);

	bool isRollEnd = false;
	float deltaAngle = _rotateSpeed * deltaSecond * ROLLANGLE;
	if (_currentAngle + deltaAngle >= TARGETANGLE[_targetIndex])
	{
		deltaAngle = TARGETANGLE[_targetIndex] - _currentAngle;
		isRollEnd = true;
	}
	_currentAngle += deltaAngle;

	_gameObject->tr.SetRotation({ 1.0f,0.0f,0.0f }, flt::DegToRad(-_currentAngle));

	flt::Transform tr;
	tr.SetRotation({ 1.0f,0.0f,0.0f }, flt::DegToRad(-deltaAngle));
	dir = dir * tr.GetWorldMatrix4f();

	_gameObject->tr.AddWorldPosition(-dir);

	if (isRollEnd)
	{
		FinishRolling();
	}
}

void CubeController::FinishRolling()
{
	_status = eCUBESTATUS::NONE;
	_targetIndex++;
	_targetIndex %= 4;
	_board->OnEndRolling();

	if (IsOutofBoard())
	{
		StartFalling();
	}
}

void CubeController::Fall(float deltaSecond)
{
	_fallSpeed += GRAVITY * deltaSecond;
	_gameObject->tr.AddWorldPosition(0.0f, -_fallSpeed * deltaSecond, 0.0f);

	if (IsFallEnough())
	{
		//_board->RemoveFromControllerList(this);
		_board->BackToPool(_gameObject);
		_status = eCUBESTATUS::NONE;
	}
}

bool CubeController::IsOutofBoard()
{
	auto pos = _gameObject->tr.GetWorldPosition();
	auto state = _board->QueryTileState(pos.x, pos.z);

	if (state == (int)TileStateFlag::NONE)
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

void CubeController::Removing(float deltaSecond)
{
	_gameObject->tr.AddWorldPosition(0.0f, -_removeSpeed * DISTANCE * deltaSecond, 0.0f);

	flt::Vector4f pos = _gameObject->tr.GetWorldPosition();

	if (pos.y <= 0.0f)	// 타일 높이보다 같거나 작아지면 제거
	{
		//_board->RemoveFromControllerList(this);
		_board->BackToPool(_gameObject);
		_status = eCUBESTATUS::NONE;
	}
}

void CubeController::Rising(float deltaSecond)
{
	if (_elapsedTime <= _riseDelay)
	{
		_elapsedTime += deltaSecond;
	}
	else
	{
		_gameObject->tr.AddWorldPosition(0.0f, _riseSpeed * DISTANCE * deltaSecond, 0.0f);
		flt::Vector4f pos = _gameObject->tr.GetWorldPosition();

		if (pos.y >= DISTANCE)	// 타일 높이보다 같거나 커지면 등장완료
		{
			_status = eCUBESTATUS::NONE;
			_board->OnEndRising();
			_gameObject->tr.SetScale(1.0, 1.0, 1.0);
			_gameObject->tr.SetWorldPosition(pos.x, DISTANCE, pos.z);
		}
	}
}
