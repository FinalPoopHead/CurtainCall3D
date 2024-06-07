#include "Player.h"
#include "../FloaterGameEngine/include/Input.h"
#include "Camera.h"


Player::Player() : _padIndex(-1), _speed(10.0f)
{
	//AddComponent<flt::CameraComponent>(true);
	Camera* camera = flt::CreateGameObject<Camera>(true);
	camera->tr.SetParent(&this->tr);
	camera->tr.SetPosition(0.0f, 5.0f, 0.0f);
	camera->tr.LookAt(0.0f, 0.0f, 0.0f);
}

void Player::Update(float deltaSecond)
{
	flt::KeyData keyData = flt::GetKey(flt::KeyCode::w);
	if (keyData)
	{
		transform.AddWorldPosition(transform.Forward() * 10.0f * deltaSecond);
	}

	keyData = flt::GetKey(flt::KeyCode::s);
	if (keyData)
	{
		transform.AddWorldPosition(-transform.Forward() * 10.0f * deltaSecond);
	}

	keyData = flt::GetKey(flt::KeyCode::a);
	if (keyData)
	{
		transform.AddWorldPosition(-transform.Right() * 10.0f * deltaSecond);
	}

	keyData = flt::GetKey(flt::KeyCode::d);
	if (keyData)
	{
		transform.AddWorldPosition(transform.Right() * 10.0f * deltaSecond);
	}
}

