#include "AdvantageCube.h"
#include "CubeController.h"

AdvantageCube::AdvantageCube()
{
	std::wstring filePath = L"..\\x64\\fbx\\cube.fbx";
	flt::RendererComponent* renderer = AddComponent<flt::RendererComponent>(true);
	renderer->SetFilePath(filePath);
	renderer->SetMaterial(0, L"AdvantageCube.png", flt::RawMaterial::TextureType::ALBEDO_OPACITY);

	AddComponent<CubeController>(true);
}

AdvantageCube::~AdvantageCube()
{

}
