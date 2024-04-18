#include "PBRController.h"

PBRController::PBRController()
	: _delta(0.5f), _meshRenderer(), _metallic(0.0f), _roughness(0.5f)
{

}

PBRController::~PBRController()
{

}

void PBRController::Start()
{
	_meshRenderer = gameObject->GetComponent<Rocket::MeshRenderer>();
}

void PBRController::Update()
{
	//float deltaTime = rocket.GetDeltaTime();
	float deltaTime = Rocket::GetDeltaTime();

	if (Rocket::GetKey('7'))
	{
		_metallic -= _delta * deltaTime;
		if (_metallic < 0.0f)
		{
			_metallic = 0.0f;
		}
		_meshRenderer->SetMetallic(_metallic);
	}

	if (Rocket::GetKey('8'))
	{
		_metallic += _delta * deltaTime;
		if (_metallic > 1.0f)
		{
			_metallic = 1.0f;
		}
		_meshRenderer->SetMetallic(_metallic);
	}

	if (Rocket::GetKey('9'))
	{
		_roughness -= _delta * deltaTime;
		if (_roughness < 0.0f)
		{
			_roughness = 0.0f;
		}
		_meshRenderer->SetRoughness(_roughness);
	}

	if (Rocket::GetKey('0'))
	{
		_roughness += _delta * deltaTime;
		if (_roughness > 1.0f)
		{
			_roughness = 1.0f;
		}
		_meshRenderer->SetRoughness(_roughness);
	}
}
