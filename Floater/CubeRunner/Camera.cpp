#include "Camera.h"
#include "Player.h"
#include "Board.h"
#include "../FloaterGameEngine/include/Input.h"


Camera::Camera(Player* player, Board* board) :
	_player(player),
	_board(board),
	_height(10.0f),
	_playHeight(30.0f),
	_lookZOffset(0.0f),
	_posZOffsest(-10.0f),
	_playerDistance(20.0f),
	_lookDegree(40.0f)
{
	AddComponent<flt::CameraComponent>(true);

	tr.SetPosition(0.0f, _height, 0.0f);
	tr.LookAt(0.0f, 0.0f, 0.0f);
}

void Camera::PostUpdate(float deltaSecond)
{
	flt::Vector4f playerPos = _player->tr.GetWorldPosition();
	//tr.SetPosition(pos.x, _height, pos.z);

	flt::Vector4f centerPos{};
	_board->GetCenterPosition(centerPos.x, centerPos.z);

	float cameraX = centerPos.x + (playerPos.x - centerPos.x) * 3.0f;
	flt::Vector3f linerPos = flt::Vector3f{ cameraX, playerPos.y, playerPos.z + _posZOffsest };
	flt::Vector3f positionDir = linerPos - (flt::Vector3f)_player->tr.GetWorldPosition();
	positionDir.Normalize();
	positionDir *= _playerDistance;
	positionDir += (flt::Vector3f)_player->tr.GetWorldPosition();
	tr.SetPosition(positionDir.x, _playHeight, positionDir.z);
	flt::Vector3f direction = flt::Vector3f{ playerPos.x, _playHeight, playerPos.z + _lookZOffset } - (flt::Vector3f)tr.GetWorldPosition();
	//flt::Vector3f direction = flt::Vector3f{centerPos.x, _playHeight, playerPos.z + _lookZOffset } - (flt::Vector3f)tr.GetWorldPosition();
	_targetRotation.Look(direction);
	
	tr.SetRotation(_targetRotation);
	tr.AddLocalRotation({ 1.0f, 0.0f, 0.0f }, flt::DegToRad(_lookDegree));
	//tr.LookAt(centerPos.x, 0.0f, playerPos.z + _lookZOffset);



	flt::KeyData keyData = GetKey(flt::KeyCode::lAlt);
	if (keyData)
	{
		keyData = GetKey(flt::KeyCode::mouseWheelUp);
		if (keyData)
		{
			_playerDistance += 1.0f * deltaSecond * keyData.x;
		}

		keyData = GetKey(flt::KeyCode::mouseWheelDown);
		if (keyData)
		{
			_playerDistance += 1.0f * deltaSecond * keyData.x;
		}
	}
	else if (GetKey(flt::KeyCode::lCtrl))
	{
		keyData = GetKey(flt::KeyCode::mouseWheelUp);
		if (keyData)
		{
			_lookDegree += 1.0f * deltaSecond * keyData.x;
		}

		keyData = GetKey(flt::KeyCode::mouseWheelDown);
		if (keyData)
		{
			_lookDegree += 1.0f * deltaSecond * keyData.x;
		}
	}
	else
	{
		keyData = GetKey(flt::KeyCode::mouseWheelUp);
		if (keyData)
		{
			_lookZOffset += 1.0f * deltaSecond * keyData.x;
		}

		keyData = GetKey(flt::KeyCode::mouseWheelDown);
		if (keyData)
		{
			_lookZOffset += 1.0f * deltaSecond * keyData.x;
		}
	}

}
