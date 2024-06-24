#include "AdvantageMine.h"

AdvantageMine::AdvantageMine() :
	_plane(flt::CreateGameObject<flt::GameObject>(true))
	, _cone(flt::CreateGameObject<flt::GameObject>(true))
{
	std::wstring planePath = L"..\\Resources\\Models\\cube.fbx";
	std::wstring conePath = L"..\\Resources\\Models\\Cone.fbx";
	std::wstring texturePath = L"..\\Resources\\Textures\\AdvantageMine.png";	// TODO : 텍스쳐는 임시임.

	float planeHeight = 2.05f;
	float coneHeight = 8.0f;

	_plane->tr.SetParent(&tr);
	_plane->tr.AddWorldPosition(0.0f, planeHeight, 0.0f);

	_cone->tr.SetParent(&tr);
	_cone->tr.AddWorldPosition(0.0f, coneHeight, 0.0f);

	auto planeRenderer = _plane->AddComponent<flt::RendererComponent>(true);
	planeRenderer->SetFilePath(planePath);
	planeRenderer->SetMaterial(0, texturePath, flt::RawMaterial::TextureType::ALBEDO_OPACITY);

	auto coneRenderer = _cone->AddComponent<flt::RendererComponent>(true);
	coneRenderer->SetFilePath(conePath);
	coneRenderer->SetMaterial(0, texturePath, flt::RawMaterial::TextureType::ALBEDO_OPACITY);
	coneRenderer->SetCastShadow(false);

	_plane->tr.SetScale(1.0f, 0.01f, 1.0f);
}

AdvantageMine::~AdvantageMine()
{

}

void AdvantageMine::OnEnable()
{
	_plane->Enable();
	_cone->Enable();
}

void AdvantageMine::OnDisable()
{
	_plane->Disable();
	_cone->Disable();
}
