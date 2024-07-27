#include "MineCone.h"

MineCone::MineCone(GameObject* parentMine, const std::wstring& texturePath)
	: _moveSpeed(2.0f)
	, _acceleration(4.0f)
	, _coneHeight(8.0f)
{
	parentMine->AddChild(this);

	std::wstring conePath = L"..\\Resources\\Models\\Cone.fbx";

	auto coneRenderer = AddComponent<flt::RendererComponent>(true);
	coneRenderer->SetFilePath(conePath);
	coneRenderer->SetMaterial(0, texturePath, flt::RawMaterial::TextureType::ALBEDO_OPACITY);
	coneRenderer->SetCastShadow(false);


	tr.SetPosition(0.0f, _coneHeight, 0.0f);
}

void MineCone::Update(float deltaTime)
{
	float currentY = tr.GetWorldPosition().y;

	float accelDir = 1.0f;
	if (currentY > _coneHeight)
	{
		accelDir = -1.0f;
	}

	_moveSpeed += _acceleration * accelDir * deltaTime;

	tr.AddLocalPosition(0.0f, _moveSpeed * deltaTime, 0.0f);

}

void MineCone::OnEnable()
{

}
