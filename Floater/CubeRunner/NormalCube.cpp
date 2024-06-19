#include "NormalCube.h"
#include "CubeController.h"

NormalCube::NormalCube()
{
	std::wstring filePath = L"..\\Resources\\Models\\BrickBlock.fbx";
	flt::RendererComponent* renderer = AddComponent<flt::RendererComponent>(true);
	renderer->SetFilePath(filePath);
	renderer->SetMaterial(0, L"../Resources/Textures/Concrete 1-diffuse.png", flt::RawMaterial::TextureType::ALBEDO_OPACITY);
	renderer->SetMaterial(0, L"../Resources/Textures/Concrete 1-normal.png", flt::RawMaterial::TextureType::NORMAL);
	renderer->SetMaterial(0, L"../Resources/Textures/Concrete 1-specular.png", flt::RawMaterial::TextureType::METALLIC);

	const auto& cubeCtr = AddComponent<CubeController>(true);
	cubeCtr->SetCubeType(eCUBETYPE::NORMAL);
}

NormalCube::~NormalCube()
{

}
