#include "NormalCube.h"
#include "CubeController.h"

NormalCube::NormalCube()
{
	std::wstring filePath = L"..\\x64\\fbx\\cube.fbx";
	flt::RendererComponent* renderer = AddComponent<flt::RendererComponent>(true);
	renderer->SetFilePath(filePath);

	AddComponent<CubeController>(true);
}

NormalCube::~NormalCube()
{

}
