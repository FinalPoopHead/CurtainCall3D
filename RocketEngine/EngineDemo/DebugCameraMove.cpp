#include "DebugCameraMove.h"

DebugCameraMove::DebugCameraMove()
	: moveSpeed(2.0f),
	accelRatio(5.0f),
	thisFrameSpeed()
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
	thisFrameSpeed = moveSpeed;

	if (Rocket::GetKey(VK_SHIFT))
	{
		thisFrameSpeed *= moveSpeed * accelRatio;
	}

	if (Rocket::GetKey('W'))
	{
		_camera->Walk(thisFrameSpeed * deltaTime);
	}

	if (Rocket::GetKey('S'))
	{
		_camera->Walk(-thisFrameSpeed * deltaTime);
	}

	if (Rocket::GetKey('A'))
	{
		_camera->Strafe(-thisFrameSpeed * deltaTime);
	}

	if (Rocket::GetKey('D'))
	{
		_camera->Strafe(thisFrameSpeed * deltaTime);
	}

	if (Rocket::GetKey('Q'))
	{
		_camera->WorldUpDown(-thisFrameSpeed * deltaTime);
	}

	if (Rocket::GetKey('E'))
	{
		_camera->WorldUpDown(thisFrameSpeed * deltaTime);
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
	_camera->Pitch(mouseDelta.y * 40 * Rocket::GetDeltaTime());
	_camera->RotateY(mouseDelta.x * 1 * Rocket::GetDeltaTime());
}
