#include "NormalCube.h"
#include "CubeController.h"
#include "../FloaterRendererCommon/include/ModelLoader.h"

NormalCube::NormalCube()
{
	std::wstring filePath = L"..\\x64\\fbx\\SM_Box_Cargo.fbx";

	AddComponent<CubeController>(true);

	flt::RendererComponent* renderer = AddComponent<flt::RendererComponent>(true);

	flt::ModelLoader loader;
	flt::RawScene rawScene;
	loader.Load(filePath, &rawScene);
	flt::RawNode* node = rawScene.nodes[0];

	renderer->SetRawNode(node);
}

NormalCube::~NormalCube()
{

}
