#include "AdvantageCube.h"
#include "CubeController.h"

AdvantageCube::AdvantageCube()
{
	std::wstring filePath = L"..\\Resources\\Models\\BrickBlock.fbx";
	flt::RendererComponent* renderer = AddComponent<flt::RendererComponent>(true);
	renderer->SetFilePath(filePath);
	renderer->SetMaterial(0, L"../Resources/Textures/T_32_Diffuse.png", flt::RawMaterial::TextureType::ALBEDO_OPACITY);
	renderer->SetMaterial(0, L"../Resources/Textures/T_32_Normal.png", flt::RawMaterial::TextureType::NORMAL);
	renderer->SetMaterial(0, L"../Resources/Textures/T_32_Specular.png", flt::RawMaterial::TextureType::METALLIC);

	const auto& cubeCtr = AddComponent<CubeController>(true);
	cubeCtr->SetCubeType(eCUBETYPE::ADVANTAGE);
}

AdvantageCube::~AdvantageCube()
{

}
