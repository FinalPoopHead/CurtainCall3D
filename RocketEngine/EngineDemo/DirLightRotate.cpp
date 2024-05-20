#include "DirLightRotate.h"



DirLightRotate::DirLightRotate()
	: rotateSpeed(10.0f)
{

}

void DirLightRotate::Start()
{

}

void DirLightRotate::Update()
{
	float deltaTime = Rocket::GetDeltaTime();
	Vector3 original = gameObject->transform.GetEuler();
	original.x = DirectX::XMConvertToDegrees(original.x);
	original.y = DirectX::XMConvertToDegrees(original.y);
	original.z = DirectX::XMConvertToDegrees(original.z);


	if (Rocket::GetKey('O'))
	{
		gameObject->transform.SetRotationEuler(original.x, original.y + rotateSpeed * deltaTime, original.z);
	}

	if (Rocket::GetKey('P'))
	{
		gameObject->transform.SetRotationEuler(original.x, original.y - rotateSpeed * deltaTime, original.z);

	}
}
