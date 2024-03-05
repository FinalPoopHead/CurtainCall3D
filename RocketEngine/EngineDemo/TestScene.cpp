#include "TestScene.h"
#include "..\\RocketEngine\\RocketAPI.h"
#include "DebugCameraMove.h"
#include "PlayerController.h"

TestScene::TestScene()
	: scene()
{ 

}

void TestScene::Initialize()
{
	scene = Rocket::CreateScene("TEST");
	assert(scene);

	auto camObj = scene->GetMainCamera()->gameObject;
	camObj->AddComponent<DebugCameraMove>();	
	scene->GetMainCamera()->GetCamera().SetAsMainCamera();

	auto player = scene->CreateObject("Sphere");
	auto meshRenderer = player->AddComponent<Rocket::MeshRenderer>();
	//meshRenderer->SetMesh(Rocket::eMeshType::SPHERE);
// 	meshRenderer->SetMesh("SK_TP_CH_Default.fbx");
// 	meshRenderer->SetTexture("T_TP_CH_Camo_006_003_D.png");
	meshRenderer->SetMesh("SM_Box_Cargo.fbx");
	player->AddComponent<PlayerController>();

	auto text = scene->CreateObject("text");
	text->AddComponent<Rocket::TextBox>();
	text->GetComponent<Rocket::TextBox>()->SetText("Hello World");
	text->transform.Translate(500.0f, 500.0f, 0.0f);

	auto sprite = scene->CreateObject("sprite");
	sprite->AddComponent<Rocket::SpriteRenderer>();
	sprite->transform.Translate(100.0f, 100.0f, 0.0f);
}
