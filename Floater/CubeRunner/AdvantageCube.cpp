#include "AdvantageCube.h"
#include "CubeController.h"

AdvantageCube::AdvantageCube()
{
	std::wstring filePath = L"..\\Resources\\Models\\cube.fbx";
	flt::RendererComponent* renderer = AddComponent<flt::RendererComponent>(true);
	renderer->SetFilePath(filePath);
	renderer->SetMaterial(0, L"../Resources/Textures/AdvantageCube.png", flt::RawMaterial::TextureType::ALBEDO_OPACITY);

	const auto& cubeCtr = AddComponent<CubeController>(true);
	cubeCtr->SetCubeType(eCUBETYPE::ADVANTAGE);
}

AdvantageCube::~AdvantageCube()
{

}
