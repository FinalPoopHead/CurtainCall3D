#include "TestGameObejct.h"
#include "../FloaterRendererCommon/include/ModelLoader.h"
#include "../FloaterGameEngine/include/BuiltinComponent.h"


TestGameObejct::TestGameObejct()
	: flt::GameObject()
{
	flt::RendererComponent* renderer = AddComponent<flt::RendererComponent>();
	flt::BoxColliderComponent* boxCollider = AddComponent<flt::BoxColliderComponent>();
	boxCollider->UseKinematic(false);

	flt::ModelLoader loader;
	//std::wstring filePath = L"..\\x64\\fbx\\Ganondorf-3d-model-dl\\source\\Ganondorf (TotK) 3D Model\\Dying.fbx";
	std::wstring filePath = L"..\\x64\\fbx\\Tile_Center.fbx";
	flt::RawScene rawScene;
	loader.Load(filePath, &rawScene);
	auto& node = rawScene.nodes[0];

	renderer->SetRawNode(node);
}

TestGameObejct::~TestGameObejct()
{

}
