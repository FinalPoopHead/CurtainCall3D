#include "DebugCameraMove.h"

DebugCameraMove::DebugCameraMove()
	: moveSpeed(2.0f)
{
}

void DebugCameraMove::Start()
{
	_camera = gameObject->GetComponent<Rocket::Camera>();
}

void DebugCameraMove::Update()
{
	//float deltaTime = rocket.GetDeltaTime();
	float deltaTime = Rocket::GetDeltaTime();
	moveSpeed = 5.0f;

	if (Rocket::GetKey(VK_SHIFT))
	{
		moveSpeed *= 2.0f;
	}

	if (Rocket::GetKey('W'))
	{
		_camera->Walk(moveSpeed * deltaTime);
	}

	if (Rocket::GetKey('S'))
	{
		_camera->Walk(-moveSpeed * deltaTime);
	}

	if (Rocket::GetKey('A'))
	{
		_camera->Strafe(-moveSpeed * deltaTime);
	}

	if (Rocket::GetKey('D'))
	{
		_camera->Strafe(moveSpeed * deltaTime);
	}

	if (Rocket::GetKey('Q'))
	{
		_camera->WorldUpDown(-moveSpeed * deltaTime);
	}

	if (Rocket::GetKey('E'))
	{
		_camera->WorldUpDown(moveSpeed * deltaTime);
	}

	OnMouseMove();
}

void DebugCameraMove::OnMouseMove()
{
	if (!Rocket::GetKey(VK_RBUTTON))
	{
		return;
	}

	Vector2 mouseDelta = Rocket::GetMouseDelta();
	mouseDelta = mouseDelta * Rocket::GetDeltaTime();
	_camera->Pitch(mouseDelta.y * 2);
	_camera->RotateY(mouseDelta.x * 2);
}