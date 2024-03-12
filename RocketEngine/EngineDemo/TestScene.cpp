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

	/// 카메라
	auto camObj = scene->GetMainCamera()->gameObject;
	camObj->AddComponent<DebugCameraMove>();	
	scene->GetMainCamera()->GetCamera().SetAsMainCamera();

	/// 스태틱 메쉬 테스트
	auto staticTest = scene->CreateObject("StaticTest");
	staticTest->transform.SetPosition(1.0f, 0.0f, 0.0f);
	auto staticTestRenderer = staticTest->AddComponent<Rocket::StaticModelRenderer>();
	staticTestRenderer->SetMesh("SK_TP_CH_Default.fbx");
	staticTestRenderer->SetTexture("T_TP_CH_Camo_006_003_D.png");
	staticTest->AddComponent<PlayerController>();

	/// 스태틱 메쉬 노드 하이어라키 테스트
// 	auto boxTest = scene->CreateObject("boxTest");
// 	boxTest->transform.SetPosition(-1.0f, 0.0f, 0.0f);
// 	auto boxTestRenderer = boxTest->AddComponent<Rocket::StaticModelRenderer>();
// 	boxTestRenderer->SetMesh("SM_Box_Cargo.fbx");
// 	boxTestRenderer->SetTexture("T_TP_CH_Camo_006_003_D.png");


	/// 스킨드 메쉬 테스트
	auto skinnedTest = scene->CreateObject("SkinnedTest");
	auto skinnedMeshRenderer = skinnedTest->AddComponent<Rocket::DynamicModelRenderer>();
	skinnedMeshRenderer->SetMesh("A_TP_CH_Sprint_F.fbx");
	skinnedMeshRenderer->SetTexture("T_TP_CH_Camo_001_006_D.png");

	/// 텍스트 테스트
// 	auto text = scene->CreateObject("text");
// 	text->AddComponent<Rocket::TextBox>();
// 	text->GetComponent<Rocket::TextBox>()->SetText("Hello World");
// 	text->transform.Translate(500.0f, 500.0f, 0.0f);

	/// 스프라이트 테스트
// 	auto sprite = scene->CreateObject("sprite");
// 	sprite->AddComponent<Rocket::SpriteRenderer>();
// 	sprite->transform.Translate(100.0f, 100.0f, 0.0f);
}
