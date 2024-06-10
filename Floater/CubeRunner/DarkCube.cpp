#include "DarkCube.h"
#include "CubeController.h"

DarkCube::DarkCube()
{
	std::wstring filePath = L"..\\Resources\\Models\\cube.fbx";
	flt::RendererComponent* renderer = AddComponent<flt::RendererComponent>(true);
	renderer->SetFilePath(filePath);
	renderer->SetMaterial(0, L"../Resources/Textures/DarkCube.png", flt::RawMaterial::TextureType::ALBEDO_OPACITY);

	AddComponent<CubeController>(true);
}

DarkCube::~DarkCube()
{

}
