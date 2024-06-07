#include "NormalCube.h"
#include "CubeController.h"

NormalCube::NormalCube()
{
	std::wstring filePath = L"..\\x64\\fbx\\cube.fbx";
	flt::RendererComponent* renderer = AddComponent<flt::RendererComponent>(true);
	renderer->SetFilePath(filePath);
	renderer->SetMaterial(0, L"NormalCube.png", flt::RawMaterial::TextureType::ALBEDO_OPACITY);

	AddComponent<CubeController>(true);
}

NormalCube::~NormalCube()
{

}
