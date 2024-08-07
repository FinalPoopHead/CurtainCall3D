﻿#include "CubeController.h"
#include "../FloaterGameEngine/include/Input.h"

#include "Board.h"

constexpr float PIVOTSIZE = 2.0f;		// 피봇까지의 y,z 거리. 모델 사이즈에 맞게 변경해야함.
constexpr float ROLLANGLE = 90.0f;		// 회전할 각도
constexpr float TARGETANGLE[4] = { 90.0f, 180.0f, 270.0f, 360.0f };	// 회전 목표 각도
constexpr float GRAVITY = 9.8f;
constexpr float STARTFALLSPEED = 20.0f;
constexpr float FALLHEIGHT = -50.0f;
constexpr float DISTANCE = 4.0f;
constexpr double OVERLAPSCALE = 0.975;
constexpr float DROPSTARTHEIGHT = 50.0f;
constexpr float DROPENDHEIGHT = 4.0f;
constexpr float STARTDROPSPEED = 50.0f;

CubeController::CubeController()
	: _board(nullptr)
	, _isRunning(false)
	, _status(eCUBESTATUS::NONE)
	, _targetIndex(0)
	, _rotateSpeed(0.0f)
	, _removeSpeed(0.0f)
	, _generateSpeed(0.0f)
	, _generateDelay(0.0f)
	, _dropDelay()
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
		Roll(deltaSecond * _board->GetGameSpeed());
		break;
	case eCUBESTATUS::FALLING:
		Fall(deltaSecond * _board->GetGameSpeed());
		break;
	case eCUBESTATUS::REMOVING:
		Removing(deltaSecond * _board->GetGameSpeed());
		break;
	case eCUBESTATUS::GENERATING:
		Generating(deltaSecond * _board->GetBattleSpeed());
		break;
	case eCUBESTATUS::DROPPING:
		Dropping(deltaSecond * _board->GetBattleSpeed());
		break;

	}
}

void CubeController::OnDisable()
{
	_isRunning = false;
	_status = eCUBESTATUS::NONE;
	// _board->RemoveFromControllerList(this);
}

bool CubeController::StartRoll(float rotateTime)
{
	if (_status != eCUBESTATUS::NONE)
	{
		return false;
	}

	if (!_isRunning)
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

void CubeController::StartFall(bool withDamage /*= true*/)
{
	if (_status != eCUBESTATUS::NONE)
	{
		return;
	}

	_status = eCUBESTATUS::FALLING;
	_fallSpeed = STARTFALLSPEED;
	_isRunning = false;

	if (withDamage)
	{
		if (_cubeType != eCUBETYPE::DARK)
		{
			_board->AddCubeFallCount();
		}
	}

	_board->RemoveFromControllerList(this);
}

void CubeController::StartRemove(float removeTime)
{
	// 치트키 사용시 바로 제거시키기 위해 rolling과 rising 카운트 제거해주는 것
	switch (_status)
	{
	case eCUBESTATUS::ROLLING:
		_board->OnEndCubeRoll();
		break;
	case eCUBESTATUS::GENERATING:
		_board->OnEndCubeGenerate();
		break;
	default:
		break;
	}

	_gameObject->tr.SetScale(OVERLAPSCALE + 0.005f, OVERLAPSCALE + 0.005f, 1.0);
	_removeSpeed = 1.0f / removeTime;

	_status = eCUBESTATUS::REMOVING;
	_board->RemoveFromControllerList(this);
	_isRunning = false;
}

void CubeController::StartGenerate(float riseTime, float delay)
{
	_generateSpeed = 1.0f / riseTime;
	_generateDelay = delay;
	_elapsedTime = 0.0f;

	_gameObject->tr.SetScale(1.0, OVERLAPSCALE, 1.0);
	_gameObject->tr.SetWorldRotation({ 0.0f,0.0f,0.0f,1.0f });

	_status = eCUBESTATUS::GENERATING;
	_isRunning = false;
}

void CubeController::StartDrop(float delay)
{
	if (_status != eCUBESTATUS::NONE)
	{
		return;
	}

	_dropDelay = delay;
	_elapsedTime = 0.0f;

	_status = eCUBESTATUS::DROPPING;
	_fallSpeed = STARTDROPSPEED;
	auto pos = _gameObject->tr.GetWorldPosition();
	_gameObject->tr.SetWorldPosition(pos.x, DROPSTARTHEIGHT, pos.z);
	_isRunning = false;
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
	_board->OnEndCubeRoll();

	if (IsOutofBoard())
	{
		StartFall();
	}
}

void CubeController::Fall(float deltaSecond)
{
	_fallSpeed += GRAVITY * deltaSecond;
	_gameObject->tr.AddWorldPosition(0.0f, -_fallSpeed * deltaSecond, 0.0f);

	if (IsFallEnough())
	{
		//_board->RemoveFromControllerList(this);
		_board->ReturnCubeToPool(_gameObject);
		_status = eCUBESTATUS::NONE;
	}
}

bool CubeController::IsOutofBoard()
{
	auto pos = _gameObject->tr.GetWorldPosition();
	auto state = _board->QueryTileState(pos.x, pos.z);

	if (state == (int)eTileStateFlag::NONE)
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

bool CubeController::IsDropEnough()
{
	if (_gameObject->tr.GetWorldPosition().y <= DROPENDHEIGHT)
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
		_board->ReturnCubeToPool(_gameObject);
		_status = eCUBESTATUS::NONE;
		_gameObject->tr.SetScale(1.0, 1.0, 1.0);
	}
}

void CubeController::Generating(float deltaSecond)
{
	if (_elapsedTime <= _generateDelay)
	{
		_elapsedTime += deltaSecond;
	}
	else
	{
		_gameObject->tr.AddWorldPosition(0.0f, _generateSpeed * DISTANCE * deltaSecond, 0.0f);
		flt::Vector4f pos = _gameObject->tr.GetWorldPosition();
		_board->AddTileState(pos.x, pos.z, eTileStateFlag::GENERATING);

		if (pos.y >= DISTANCE)	// 타일 높이보다 같거나 커지면 등장완료
		{
			_status = eCUBESTATUS::NONE;
			_board->OnEndCubeGenerate();
			_gameObject->tr.SetScale(1.0, 1.0, 1.0);
			_gameObject->tr.SetWorldPosition(pos.x, DISTANCE, pos.z);
		}
	}
}

void CubeController::Dropping(float deltaSecond)
{
	if (_elapsedTime <= _generateDelay)
	{
		_elapsedTime += deltaSecond;
	}
	else
	{
		_fallSpeed += GRAVITY * deltaSecond;
		_gameObject->tr.AddWorldPosition(0.0f, -_fallSpeed * deltaSecond, 0.0f);

		if (IsDropEnough())
		{
			//_board->RemoveFromControllerList(this);
			//_board->ReturnCubeToPool(_gameObject);
			_status = eCUBESTATUS::NONE;

			_board->OnEndCubeDrop(this);

			flt::Vector4f pos = _gameObject->tr.GetWorldPosition();
			_gameObject->tr.SetWorldPosition(pos.x, DROPENDHEIGHT, pos.z);
			_gameObject->tr.SetScale(1.0, 1.0, 1.0);
		}
	}
}
