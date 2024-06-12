#include "AdvantageMine.h"

AdvantageMine::AdvantageMine()
{
	// TODO : 임시로 아무 모델링파일 넣어둠
	std::wstring filePath = L"..\\Resources\\Models\\Cone.fbx";

	flt::RendererComponent* renderer = AddComponent<flt::RendererComponent>(true);
	renderer->SetFilePath(filePath);
	renderer->SetMaterial(0, L"../Resources/Textures/AdvantageCube.png", flt::RawMaterial::TextureType::ALBEDO_OPACITY);
}

AdvantageMine::~AdvantageMine()
{

}
