#include "TestScene.h"
#include "..\\RocketEngine\\RocketAPI.h"
#include "DebugCameraMove.h"
#include "PlayerController.h"
#include "PBRController.h"
#include "DirLightRotate.h"

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
	scene->GetMainCamera()->SetAsMainCamera();
	camObj->transform.Rotate(45.0f, 45.0f, 0.0f);
	camObj->transform.SetPosition(-10.0f, 10.0f, -10.0f);

	/// 라이트
	auto lightObj = scene->CreateObject("light");
	auto lightComp = lightObj->AddComponent<Rocket::DirectionalLight>();
	lightObj->AddComponent<DirLightRotate>();
	lightObj->transform.Rotate(45.0f, 45.0f, 0.0f);
	lightObj->transform.Translate(-10.0f, 10.0f, -10.0f);

	/// Plane
	// 바닥
	{
		auto plane = scene->CreateObject("plane");
		plane->transform.SetPosition(0.0f, 0.0f, 0.0f);
		plane->transform.SetScale(50.0f, 0.01f, 50.0f);
		auto planeRenderer = plane->AddComponent<Rocket::MeshRenderer>();
		planeRenderer->SetMesh(Rocket::eMeshType::CUBE);
		planeRenderer->SetBaseColorTexture("T_WEP_Basic_008_D.png");
	}
	//오른쪽 벽
	{
		auto plane = scene->CreateObject("wall_right");
		plane->transform.SetPosition(25.0f, 0.0f, 0.0f);
		plane->transform.SetRotationEuler(0.0f, 0.0f, 90.0f);
		plane->transform.SetScale(50.0f, 0.01f, 50.0f);
		auto planeRenderer = plane->AddComponent<Rocket::MeshRenderer>();
		planeRenderer->SetMesh(Rocket::eMeshType::CUBE);
		planeRenderer->SetBaseColorTexture("T_WEP_Basic_008_D.png");
	}
	// 뒤쪽 벽
	{
		auto plane = scene->CreateObject("wall_left");
		plane->transform.SetPosition(0.0f, 0.0f, 25.0f);
		plane->transform.SetRotationEuler(90.0f, 0.0f, 0.0f);
		plane->transform.SetScale(50.0f, 0.01f, 50.0f);
		auto planeRenderer = plane->AddComponent<Rocket::MeshRenderer>();
		planeRenderer->SetMesh(Rocket::eMeshType::CUBE);
		planeRenderer->SetBaseColorTexture("T_WEP_Basic_008_D.png");
	}

	/// PBR 테스트
	// PBR 구
	auto PBRSphere = scene->CreateObject("PBRTest");
	PBRSphere->AddComponent<PBRController>();
	PBRSphere->transform.SetPosition(0.0f, 1.0f, -10.0f);
	auto SphereRenderer = PBRSphere->AddComponent<Rocket::MeshRenderer>();
	SphereRenderer->SetMesh(Rocket::eMeshType::SPHERE);
	SphereRenderer->SetBaseColorTexture("T_WEP_Basic_009_D.png");

	// PBR 구 25개 배치
	// 0.0 0.25 0.5 0.75 1.0
	int count = 7;
	float offSetX = 10.0f;
	float offSetY = 1.0f;
	float offSetZ = 10.0f;
	float lenInterval = 1.5f;
	float matInterval = 1.0f / static_cast<float>(count-1);
	for (int i = 0; i < count; i++)
	{
		for (int j = 0; j < count; j++)
		{
			auto sphere = scene->CreateObject("PBRTest"+std::to_string(i)+std::to_string(j));
			sphere->transform.SetPosition(offSetX + j * lenInterval, offSetY + i * lenInterval, offSetZ);
			auto sr = sphere->AddComponent<Rocket::MeshRenderer>();
			sr->SetMesh(Rocket::eMeshType::SPHERE);
			sr->SetBaseColorTexture("T_WEP_Basic_009_D.png");
			sr->SetMetallic(i * matInterval);
			sr->SetRoughness(j * matInterval);
		}
	}

	// PBR 모델
	auto PBRModel = scene->CreateModelObject("Cerberus_LP.fbx");
	PBRModel->transform.SetPosition(4.0f, 3.0f, -10.0f);
	PBRModel->transform.SetScale(0.05f, 0.05f, 0.05f);
	PBRModel->transform.Rotate(0.0f, 180.0f, 0.0f);

	auto PBRmr = PBRModel->GetComponentsFromAll<Rocket::MeshRenderer>();
	for (auto& m : PBRmr)
	{
		m->SetBaseColorTexture("Cerberus_A.tga");
		m->SetNormalTexture("Cerberus_N.tga");
		m->SetMetallicTexture("Cerberus_M.tga");
		m->SetRoughnessTexture("Cerberus_R.tga");
	}

	/// 스킨드 메쉬 테스트
	auto skinnedTest1 = scene->CreateModelObject("Rob02.fbx");
	skinnedTest1->transform.Translate(0.0f, 0.0f, 3.0f);

	auto dmr1 = skinnedTest1->GetComponentsFromAll<Rocket::DynamicModelRenderer>();
	for (auto& m : dmr1)
	{
		m->SetBaseColorTexture("Rob02Yellow_AlbedoTransparency.png");
		m->SetNormalTexture("Rob02_Normal.dds");
		m->SetMetallicTexture("Rob02White_MetallicSmoothness.dds");
		m->SetRoughnessTexture("Rob02White_Roughness.png");
	}

	auto skinnedTest2 = scene->CreateModelObject("Dying.fbx");
	skinnedTest2->transform.SetScale(0.005f, 0.005f, 0.005f);
	skinnedTest2->transform.Translate(-10.0f, 0.0f, 5.0f);
	
	auto skinnedTest3 = scene->CreateModelObject("A_TP_CH_Sprint_F.fbx");
	skinnedTest3->transform.SetScale(0.05f, 0.05f, 0.05f);
	skinnedTest3->transform.Translate(3.0f, 0.0f, 0.0f);

	auto dmr3 = skinnedTest3->GetComponentsFromAll<Rocket::DynamicModelRenderer>();
	for (auto& m : dmr3)
	{
		m->SetBaseColorTexture("T_TP_CH_Camo_001_006_D.png");
	}

	{
		auto skinnedTest3 = scene->CreateModelObject("A_TP_CH_Breathing.fbx");
		skinnedTest3->transform.SetScale(0.05f, 0.05f, 0.05f);
		skinnedTest3->transform.Translate(10.0f, 0.0f, -10.0f);

		auto dmr3 = skinnedTest3->GetComponentsFromAll<Rocket::DynamicModelRenderer>();
		for (auto& m : dmr3)
		{
			m->SetBaseColorTexture("T_TP_CH_Camo_001_006_D.png");
		}
	}

	/// 스태틱 메쉬 테스트
	auto staticTest = scene->CreateModelObject("SK_TP_CH_Default.fbx");
	staticTest->transform.SetPosition(-10.0f, 0.0f, -10.0f);
	staticTest->transform.SetScale(5.0f, 5.0f, 5.0f);

	{
		auto mr = staticTest->GetComponentsFromAll<Rocket::MeshRenderer>();
		for (auto& m : mr)
		{
			m->SetBaseColorTexture("T_TP_CH_Camo_006_003_D.png");
		}
	}

	/// 노드구조대로 게임오브젝트 생성한 것 테스트
	auto boneHierarchyTest = scene->CreateModelObject("SK_TP_CH_Default.fbx");
	//staticTest->transform.SetPosition(1.0f, 0.0f, 0.0f);

	auto mr = boneHierarchyTest->GetComponentsFromAll<Rocket::MeshRenderer>();
	for (auto& m : mr)
	{
		m->SetBaseColorTexture("T_TP_CH_Camo_006_003_D.png");
	}

	auto hand = scene->FindObjectByName("hand_r");
	boneHierarchyTest->transform.SetParent(hand,false);
	boneHierarchyTest->transform.SetScale(1.0f, 1.0f, 1.0f);


	/// 스태틱 메쉬 계층구조 테스트
	auto hierarchyTest = scene->CreateModelObject("SM_Box_Cargo.fbx");
	hierarchyTest->transform.SetPosition(-3.0f, 0.4f, 5.0f);
	hierarchyTest->transform.SetScale(2.0f, 2.0f, 2.0f);
	auto hierarchyRenderer = hierarchyTest->GetComponentsFromAll<Rocket::MeshRenderer>();
	for(auto& m : hierarchyRenderer)
	{
		m->SetBaseColorTexture("T_TP_CH_Props_D.png");
	}

	/// 텍스트 테스트
	auto text = scene->CreateObject("text");
	text->AddComponent<Rocket::TextBox>();
	text->GetComponent<Rocket::TextBox>()->SetText("Hello World\nBABO");
	text->transform.Translate(1400.0f, 0.0f, 0.0f);

	/// 스프라이트 테스트
	auto sprite = scene->CreateObject("sprite");
	sprite->AddComponent<Rocket::SpriteRenderer>();
	sprite->transform.Translate(1400.0f, 700.0f, 0.0f);

	/// 부하테스트
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			/// 스킨드 메쉬 부하 테스트
// 			auto skinnedTest3 = scene->CreateModelObject("A_TP_CH_Sprint_F.fbx");
// 			skinnedTest3->transform.SetScale(0.05f, 0.05f, 0.05f);
// 			skinnedTest3->transform.Translate(5 * i - 10.0f , 0.0f, 5 * j);
// 
// 			auto dmr3 = skinnedTest3->GetComponentsFromAll<Rocket::DynamicModelRenderer>();
// 			for (auto& m : dmr3)
// 			{
// 				m->SetBaseColorTexture("T_TP_CH_Camo_001_006_D.png");
// 			}

			/// 스태틱 메쉬 부하 테스트
// 			auto staticTest = scene->CreateModelObject("SK_TP_CH_Default.fbx");
// 			staticTest->transform.SetPosition(5 + i, 0.0f, 5 + j);
// 
// 			auto mr = staticTest->GetComponentsFromAll<Rocket::MeshRenderer>();
// 			for (auto& m : mr)
// 			{
// 				m->SetTexture("T_TP_CH_Camo_006_003_D.png");
// 			}
		}
	}
}
