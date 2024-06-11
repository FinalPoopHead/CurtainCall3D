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
	_playerDistance(25.0f),
	_lookDegree(40.0f),
	_movSpeed(1.5f),
	_rotSpeed(15.0f)
{
	AddComponent<flt::CameraComponent>(true);

	_currPosition = CalcTargetPosition();
	tr.SetPosition(_currPosition);

	_currRotation = CalcTargetRotation();
	tr.SetRotation(_currRotation);
}

void Camera::PostUpdate(float deltaSecond)
{
	//flt::Vector3f targetPosition = CalcTargetPosition();
	//tr.SetPosition(targetPosition);
	//_currPosition = targetPosition;

	_currPosition = flt::Vector3f::Lerp(_currPosition, CalcTargetPosition(), std::clamp(deltaSecond * _movSpeed, 0.0f, 1.0f) );
	tr.SetPosition(_currPosition);

	_currRotation = flt::Quaternion::Slerp(_currRotation, CalcTargetRotation(), std::clamp(deltaSecond * _rotSpeed, 0.0f, 1.0f));
	
	tr.SetRotation(_currRotation);
	tr.AddLocalRotation({ 1.0f, 0.0f, 0.0f }, flt::DegToRad(_lookDegree));



	/*flt::KeyData keyData = GetKey(flt::KeyCode::lAlt);
	if (keyData)
	{
		keyData = GetKey(flt::KeyCode::mouseWheelUp);
		if (keyData)
		{
			_rotSpeed += 1.0f * deltaSecond * keyData.x;
			std::cout << "rotSpeed : " << _rotSpeed << std::endl;
		}

		keyData = GetKey(flt::KeyCode::mouseWheelDown);
		if (keyData)
		{
			_rotSpeed += 1.0f * deltaSecond * keyData.x;
			std::cout << "rotSpeed : " << _rotSpeed << std::endl;
		}
	}
	else if (GetKey(flt::KeyCode::lCtrl))
	{
		keyData = GetKey(flt::KeyCode::mouseWheelUp);
		if (keyData)
		{
			_movSpeed += 1.0f * deltaSecond * keyData.x;
			std::cout << "movSpeed : " << _movSpeed << std::endl;
		}

		keyData = GetKey(flt::KeyCode::mouseWheelDown);
		if (keyData)
		{
			_movSpeed += 1.0f * deltaSecond * keyData.x;
			std::cout << "movSpeed : " << _movSpeed << std::endl;
		}
	}
	else
	{

	}*/

	/*flt::KeyData keyData = GetKey(flt::KeyCode::lAlt);
	if (keyData)
	{
		keyData = GetKey(flt::KeyCode::mouseWheelUp);
		if (keyData)
		{
			_playHeight += 0.5f;
			std::cout << "_playHeight : " << _playHeight << std::endl;
		}

		keyData = GetKey(flt::KeyCode::mouseWheelDown);
		if (keyData)
		{
			_playHeight -= 0.5f;
			std::cout << "_playHeight : " << _playHeight << std::endl;
		}
	}
	else if (GetKey(flt::KeyCode::lCtrl))
	{
		keyData = GetKey(flt::KeyCode::mouseWheelUp);
		if (keyData)
		{
			_lookDegree += 1.0f;
			std::cout << "_lookDegree : " << _lookDegree << std::endl;
		}

		keyData = GetKey(flt::KeyCode::mouseWheelDown);
		if (keyData)
		{
			_lookDegree -= 1.0f;
			std::cout << "_lookDegree : " << _lookDegree << std::endl;
		}
	}
	else if (GetKey(flt::KeyCode::spacebar))
	{
		keyData = GetKey(flt::KeyCode::mouseWheelUp);
		if (keyData)
		{
			_playerDistance += 1.0f;
			std::cout << "_playerDistance : " << _playerDistance << std::endl;
		}

		keyData = GetKey(flt::KeyCode::mouseWheelDown);
		if (keyData)
		{
			_playerDistance -= 1.0f;
			std::cout << "_playerDistance : " << _playerDistance << std::endl;
		}
	}*/
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
