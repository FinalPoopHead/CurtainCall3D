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
	/*
	auto staticTest = scene->CreateObject("StaticTest");
	staticTest->transform.SetPosition(1.0f, 0.0f, 0.0f);
	auto staticTestRenderer = staticTest->AddComponent<Rocket::MeshRenderer>();
	staticTestRenderer->SetMesh("SK_TP_CH_Default.fbx");
	staticTestRenderer->SetTexture("T_TP_CH_Camo_006_003_D.png");
	staticTest->AddComponent<PlayerController>();
	*/

	auto staticTest = scene->CreateModelObject("SK_TP_CH_Default.fbx");
	staticTest->transform.SetPosition(1.0f, 0.0f, 0.0f);

	/// 스태틱 메쉬 노드 하이어라키 테스트
// 	auto boxTest = scene->CreateObject("boxTest");
// 	boxTest->transform.SetPosition(-1.0f, 0.0f, 0.0f);
// 	auto boxTestRenderer = boxTest->AddComponent<Rocket::StaticModelRenderer>();
// 	boxTestRenderer->SetMesh("SM_Box_Cargo.fbx");
// 	boxTestRenderer->SetTexture("T_TP_CH_Camo_006_003_D.png");


	/// 스킨드 메쉬 테스트
	/*
	auto skinnedTest1 = scene->CreateObject("SkinnedTest1");
	auto skinnedMeshRenderer1 = skinnedTest1->AddComponent<Rocket::DynamicModelRenderer>();
	skinnedTest1->transform.SetScale(0.05f, 0.05f, 0.05f);
	skinnedTest1->transform.Translate(3.0f, 0.0f, 0.0f);
	skinnedMeshRenderer1->LoadModel("A_TP_CH_Sprint_F.fbx");
	skinnedMeshRenderer1->SetTexture("T_TP_CH_Camo_001_006_D.png");

	auto skinnedTest2 = scene->CreateObject("SkinnedTest2");
	auto skinnedMeshRenderer2 = skinnedTest2->AddComponent<Rocket::DynamicModelRenderer>();
	skinnedTest2->transform.SetScale(0.005f, 0.005f, 0.005f);
	skinnedTest2->transform.Translate(-3.0f, 0.0f, 0.0f);
	skinnedMeshRenderer2->LoadModel("Dying.fbx" );
	skinnedMeshRenderer2->SetTexture("T_TP_CH_Camo_001_006_D.png");
	*/

	auto skinnedTest1 = scene->CreateModelObject("Rob02.fbx");
	skinnedTest1->transform.Translate(0.0f, 0.0f, 3.0f);
	
	auto skinnedTest2 = scene->CreateModelObject("A_TP_CH_Sprint_F.fbx");
	skinnedTest2->transform.SetScale(0.05f, 0.05f, 0.05f);
	skinnedTest2->transform.Translate(3.0f, 0.0f, 0.0f);

 	auto skinnedTest3 = scene->CreateModelObject("Dying.fbx");
 	skinnedTest3->transform.SetScale(0.005f, 0.005f, 0.005f);
 	skinnedTest3->transform.Translate(-3.0f, 0.0f, 0.0f);

	/*
	auto skinnedTest3 = scene->CreateObject("SkinnedTest");
	auto skinnedMeshRenderer3 = skinnedTest3->AddComponent<Rocket::DynamicModelRenderer>();
	//skinnedMeshRenderer->SetMesh("A_TP_CH_Sprint_F.fbx");
	skinnedTest3->transform.SetScale(0.01f, 0.01f, 0.01f);
	//skinnedMeshRenderer->SetMesh("Dying.fbx");
	skinnedMeshRenderer3->SetMesh("Monster2.fbx");
	skinnedMeshRenderer3->SetTexture("T_TP_CH_Camo_001_006_D.png");
	*/

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
