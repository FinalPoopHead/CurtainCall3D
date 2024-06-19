#include "DarkCube.h"
#include "CubeController.h"

DarkCube::DarkCube()
{
	std::wstring filePath = L"..\\Resources\\Models\\BrickBlock.fbx";
	flt::RendererComponent* renderer = AddComponent<flt::RendererComponent>(true);
	renderer->SetFilePath(filePath);
	renderer->SetMaterial(0, L"../Resources/Textures/Floor_7_Albedo.png", flt::RawMaterial::TextureType::ALBEDO_OPACITY);
	renderer->SetMaterial(0, L"../Resources/Textures/Floor_7_Metallic.png", flt::RawMaterial::TextureType::METALLIC);
	renderer->SetMaterial(0, L"../Resources/Textures/Floor_7_Normal.png", flt::RawMaterial::TextureType::NORMAL);
	renderer->SetMaterial(0, L"../Resources/Textures/Floor_7_Occlusion.png", flt::RawMaterial::TextureType::AO);

	const auto& cubeCtr = AddComponent<CubeController>(true);
	cubeCtr->SetCubeType(eCUBETYPE::DARK);
}

DarkCube::~DarkCube()
{

}
