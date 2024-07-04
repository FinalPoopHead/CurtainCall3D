#include <algorithm>
#include "Camera.h"
#include "Player.h"
#include "Board.h"
#include "../FloaterGameEngine/include/Input.h"


Camera::Camera(Player* player, Board* board) 
	: _player(player)
	, _board(board)
	, _height(10.0f)
	, _playHeight(40.0f)
	, _lookZOffset(0.0f)
	, _posZOffsest(-10.0f)
	, _playerDistance(35.0f)
	, _lookDegree(40.0f)
	, _movSpeed(1.5f)
	, _rotSpeed(15.0f)
	, _isMoving(true)
	, _isPlayerLook(true)
	, _isReadyToPlayerLook(true)
{
	AddComponent<flt::CameraComponent>(true);

	_currPosition = CalcTargetPosition();
	tr.SetPosition(_currPosition);

	_currRotation = CalcTargetRotation();
	tr.SetRotation(_currRotation);
}

void Camera::TracePlayer()
{
	_isPlayerLook = true;
	_isReadyToPlayerLook = true;
	_isTweenMove = false;
	_isTweenRotate = false;

	_startPosition = (flt::Vector3f)tr.GetLocalPosition();
	_startRotation = tr.GetLocalRotation();
}

void Camera::TweenMove(flt::Vector3f targetPos, float time, std::function<float(float)> ease)
{
	_isPlayerLook = false;
	_isReadyToPlayerLook = false;
	_isTweenMove = true;
	_startPosition = (flt::Vector3f)tr.GetLocalPosition();
	_targetPosition = targetPos;
	_moveTime = time;
	_moveTimeElapsed = 0.0f;
	_moveEase = ease;
}

void Camera::TweenRotate(flt::Quaternion targetRot, float time, std::function<float(float)> ease)
{
	_isPlayerLook = false;
	_isReadyToPlayerLook = false;
	_isTweenRotate = true;
	_startRotation = tr.GetLocalRotation();
	_targetRotation = targetRot;
	_rotateTime = time;
	_rotateTimeElapsed = 0.0f;
	_rotateEase = ease;
}

void Camera::PostUpdate(float deltaSecond)
{
	//flt::Vector3f targetPosition = CalcTargetPosition();
	//tr.SetPosition(targetPosition);
	//_currPosition = targetPosition;
	if(flt::GetKeyDown(flt::KeyCode::mouseLButton))
	{
		if (_isPlayerLook)
		{
			TweenMove({ 0.0f, 0.0f, 0.0f }, 1.0f);
			TweenRotate({ 0.0f, 0.0f, 0.0f, 1.0f }, 1.0f);
		}
		else
		{
			TracePlayer();
		}
	}

	UpdateCameraMove(deltaSecond);
}

flt::Quaternion Camera::CalcTargetRotation()
{
	flt::Vector4f playerPos = _player->tr.GetWorldPosition();

	flt::Vector3f direction = flt::Vector3f{ playerPos.x, _playHeight, playerPos.z + _lookZOffset } - (flt::Vector3f)tr.GetWorldPosition();

	flt::Quaternion targetRotation{}; 
	targetRotation.Look(direction);

	return targetRotation;
}

flt::Vector3f Camera::CalcTargetPosition()
{
	flt::Vector4f centerPos{};
	_board->GetCenterPosition(centerPos.x, centerPos.z);
	flt::Vector4f playerPos = _player->tr.GetWorldPosition();
	float cameraX = centerPos.x + (playerPos.x - centerPos.x) * 3.0f;

	flt::Vector3f linerPos = flt::Vector3f{ cameraX, playerPos.y, playerPos.z + _posZOffsest };
	flt::Vector3f positionDir = linerPos - (flt::Vector3f)_player->tr.GetWorldPosition();
	positionDir.Normalize();
	positionDir *= _playerDistance;
	positionDir += (flt::Vector3f)_player->tr.GetWorldPosition();

	positionDir.y = _playHeight;

	return positionDir;
}

//bool Camera::CheckCameraMove()
//{
//	return true;
//}
//
//void Camera::StartCameraMove()
//{
//	_isMoving = true;
//}

void Camera::UpdateCameraMove(float deltaSecond)
{
	if (_isReadyToPlayerLook)
	{
		_currPosition = flt::Vector3f::Lerp(_currPosition, CalcTargetPosition(), std::clamp(deltaSecond * _movSpeed, 0.0f, 1.0f));
		tr.SetPosition(_currPosition);

		_currRotation = flt::Quaternion::Slerp(_currRotation, CalcTargetRotation(), std::clamp(deltaSecond * _rotSpeed, 0.0f, 1.0f));

		tr.SetRotation(_currRotation);
		tr.AddLocalRotation({ 1.0f, 0.0f, 0.0f }, flt::DegToRad(_lookDegree));
	}
	/*else if (_isPlayerLook)
	{
		_moveTimeElapsed += deltaSecond;
		float moveT = (_moveTimeElapsed / 2.0f);
		if (moveT >= 1.0f)
		{
			moveT = 1.0f;
		}
		flt::Vector3f targetPos = CalcTargetPosition();
		flt::Vector3f pos = flt::Vector3f::Lerp(_startPosition, targetPos, moveT);
		tr.SetPosition(pos);

		_rotateTimeElapsed += deltaSecond;
		float rotT = (_rotateTimeElapsed / 2.0f);
		if (rotT >= 1.0f)
		{
			rotT = 1.0f;
		}
		flt::Quaternion targetRot = CalcTargetRotation();
		targetRot = flt::Quaternion{ { 1.0f, 0.0f, 0.0f }, flt::DegToRad(_lookDegree) } * targetRot;
		flt::Quaternion rot = flt::Quaternion::Slerp(_startRotation, targetRot, rotT);
		tr.SetRotation(rot);

		if (moveT >= 1.0f && rotT >= 1.0f)
		{
			_isReadyToPlayerLook = true;
		}
	}*/
	else
	{
		if (_isTweenMove)
		{
			_moveTimeElapsed += deltaSecond;
			float t = _moveEase(_moveTimeElapsed / _moveTime);
			if (t >= 1.0f)
			{
				t = 1.0f;
				_isTweenMove = false;
				_moveTimeElapsed = 0.0f;
			}
			flt::Vector3f pos = flt::Vector3f::Lerp(_startPosition, _targetPosition, t);
			tr.SetPosition(pos);
		}
		if (_isTweenRotate)
		{
			_rotateTimeElapsed += deltaSecond;
			float t = _rotateEase(_rotateTimeElapsed / _rotateTime);
			if (t >= 1.0f)
			{
				t = 1.0f;
				_isTweenRotate = false;
				_rotateTimeElapsed = 0.0f;
			}
			flt::Quaternion rot = flt::Quaternion::Slerp(_startRotation, _targetRotation, t);
			tr.SetRotation(rot);
		}
	}
}
