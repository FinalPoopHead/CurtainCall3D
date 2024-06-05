#include "NormalCube.h"
#include "CubeController.h"
#include "../FloaterRendererCommon/include/ModelLoader.h"

NormalCube::NormalCube()
{
	std::wstring filePath = L"..\\x64\\fbx\\cube.fbx";

	AddComponent<CubeController>(true);

	flt::RendererComponent* renderer = AddComponent<flt::RendererComponent>(true);
	renderer->SetFilePath(filePath);
}

NormalCube::~NormalCube()
{

}
