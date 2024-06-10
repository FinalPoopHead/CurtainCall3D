#include "Camera.h"
#include "Player.h"


Camera::Camera(Player* player) :
	_player(player),
	_height(50.0f)
{
	AddComponent<flt::CameraComponent>(true);

	tr.SetPosition(0.0f, _height, 0.0f);
	tr.LookAt(0.0f, 0.0f, 0.0f);
}

void Camera::PostUpdate(float deltaSecond)
{
	flt::Vector4f pos = _player->tr.GetWorldPosition();
	tr.SetPosition(pos.x, _height, pos.z);
}
