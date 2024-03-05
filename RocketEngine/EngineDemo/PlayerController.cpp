#include "PlayerController.h"

PlayerController::PlayerController()
	: _moveSpeed()
{

}

PlayerController::~PlayerController()
{

}

void PlayerController::Update()
{
	//float deltaTime = rocket.GetDeltaTime();
	float deltaTime = Rocket::GetDeltaTime();
	Vector3 moveVec = Vector3::Zero;
	_moveSpeed = 5.0f;

	if (Rocket::GetKey(VK_SHIFT))
	{
		_moveSpeed *= 2.0f;
	}

	if (Rocket::GetKey(VK_UP))
	{
		moveVec += gameObject->transform.GetForward() * _moveSpeed * deltaTime;
	}

	if (Rocket::GetKey(VK_DOWN))
	{
		moveVec += gameObject->transform.GetForward() * -_moveSpeed * deltaTime;
	}

	if (Rocket::GetKey(VK_LEFT))
	{
		moveVec += gameObject->transform.GetRight() * -_moveSpeed * deltaTime;
	}

	if (Rocket::GetKey(VK_RIGHT))
	{
		moveVec += gameObject->transform.GetRight() * _moveSpeed * deltaTime;
	}

	gameObject->transform.Translate(moveVec);
}
