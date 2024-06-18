#include "NormalCube.h"
#include "CubeController.h"

NormalCube::NormalCube()
{
	std::wstring filePath = L"..\\Resources\\Models\\cube.fbx";
	flt::RendererComponent* renderer = AddComponent<flt::RendererComponent>(true);
	renderer->SetFilePath(filePath);
	renderer->SetMaterial(0, L"../Resources/Textures/NormalCube.png", flt::RawMaterial::TextureType::ALBEDO_OPACITY);

	const auto& cubeCtr = AddComponent<CubeController>(true);
	cubeCtr->SetCubeType(eCUBETYPE::NORMAL);
}

NormalCube::~NormalCube()
{

}
