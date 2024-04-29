#include "TestGameObejct.h"
#include "../FloaterRendererCommon/include/ModelLoader.h"


TestGameObejct::TestGameObejct()
	: flt::GameObject()
{
	//flt::CameraComponent* camera = new flt::CameraComponent(this);
	//AddComponent(camera);
	//AddComponent<flt::CameraComponent>();

	//flt::RendererComponent* renderer = new flt::RendererComponent(this);
	//AddComponent(renderer);
	flt::RendererComponent* renderer = AddComponent<flt::RendererComponent>();

	flt::ModelLoader loader;
	std::wstring filePath = L"..\\x64\\fbx\\Ganondorf-3d-model-dl\\source\\Ganondorf (TotK) 3D Model\\Dying.fbx";
	flt::RawScene rawScene;
	loader.Load(filePath, &rawScene);
	auto& node = rawScene.nodes[0];

	renderer->SetRawNode(node);
}

TestGameObejct::~TestGameObejct()
{

}
