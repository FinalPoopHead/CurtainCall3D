﻿#include "TestGameObejct.h"
#include "../FloaterRendererCommon/include/ModelLoader.h"
#include "../FloaterGameEngine/include/EngineMinimal.h"
#include "../FloaterGameEngine/include/Input.h"


TestGameObejct::TestGameObejct()
	: flt::GameObject()
{
	flt::RendererComponent* renderer = AddComponent<flt::RendererComponent>(true);
	flt::BoxColliderComponent* boxCollider = AddComponent<flt::BoxColliderComponent>(true);
	boxCollider->UseKinematic(true);
	boxCollider->SetSize({ 100.0f, 100.0f, 100.0f });

	std::wstring filePath = L"..\\x64\\fbx\\Tile_Edge.fbx";
	//flt::ModelLoader loader;
	////std::wstring filePath = L"..\\x64\\fbx\\Ganondorf-3d-model-dl\\source\\Ganondorf (TotK) 3D Model\\Dying.fbx";
	//flt::RawScene rawScene;
	//loader.Load(filePath, &rawScene);
	//auto& node = rawScene.nodes[0];

	//renderer->SetRawNode(node);

	renderer->SetFilePath(filePath);
}

TestGameObejct::~TestGameObejct()
{

}

void TestGameObejct::Update(float deltaTime)
{
	flt::GamePadState outState;
	bool ret = GetGamePadState(0, &outState);

	_elapsedTime += deltaTime;
	static float enableTime = 2.0f;
	static float disableTime = 1.0f;

	if (_elapsedTime > enableTime)
	{
		flt::RendererComponent* renderer = GetComponent<flt::RendererComponent>();
		renderer->Enable();
		_elapsedTime = 0.0f;
	}
	else if (_elapsedTime > disableTime)
	{
		flt::RendererComponent* renderer = GetComponent<flt::RendererComponent>();
		renderer->Disable();
	}
}
