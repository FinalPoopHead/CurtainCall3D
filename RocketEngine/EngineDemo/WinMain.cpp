// 애플리케이션에 대한 진입점을 정의합니다.
//
#include <windows.h>
#include <memory>
#include <cassert>
//#include "..\\Rocket\\IEngineProcess.h"
#include "..\\RocketEngine\\RocketAPI.h"
#include "Client.h"
#include "DebugCameraMove.h"

// 전역 변수:
HINSTANCE g_hInst;                                // 현재 인스턴스입니다.
//WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
//WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	/// RocketAPI를 갖고온다.
	//RocketAPILoader::Instance().Initialize();


	/// 엔진을 초기화 하고 루프시킨다.
	Rocket::EngineProcess* Rocket = Rocket::CreateLauncher();
	//Rocket::IEngineProcess* Rocket = RocketAPILoader::Instance().CreateLauncher();
	Rocket->Initialize(static_cast<void*>(hInstance), nCmdShow);

	/// 클라이언트를 초기화하고 준비시킨다.
// 	std::unique_ptr<Client> client(new Client());
// 	client->Initialize();

	/// 용훈이형 여기야
	{
		Rocket::Scene* scene = Rocket::CreateScene("TEST");
		assert(scene);

		//만약 여러 씬이 있다면 아래 함수로 씬을 로드해줘야해.
		//Rocket::LoadScene("TEST");

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

		/// 이 CreateModelObject를 하면
		/// 1. FBX파일을 이때 읽으면서 Raw Data들을 만들어내고
		/// 2. 그 Raw Data를 그래픽스한테도 넘겨줘서 리소스를 만들어두도록 하고
		/// 3. 적절한 Renderer 컴포넌트를 부착하고 그래픽스와 연결해.
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

		/// 이 CreateModelObject를 하면
		/// 1. FBX파일을 이때 읽으면서 Raw Data들을 만들어내고
		/// 2. 그 Raw Data를 그래픽스한테도 넘겨줘서 리소스를 만들어두도록 하고
		/// 3. 적절한 Renderer 컴포넌트를 부착하고 그래픽스와 연결해.
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


	Rocket->RunEngine();

	/// 게임을 정리한다.
	Rocket->Finalize();

	Rocket::ReleaseLauncher(Rocket);
}

// #include "..\\Rocket\\IRocketLauncher.h"
// 
// #define ENGINEDLL_PATH L"..\\x64\\Debug\\Rocket.dll"
// #else
// #define ENGINEDLL_PATH L"Engine\\Rocket.dll" 
// #endif // _DEBUG
// #define LAUNCHER_SIGNATURE Rocket::IRocketLauncher* (*) (void)
// #define LAUNCHER_NAME "CreateEngine"
// main
// {
// 	HMODULE hEngineModule = LoadLibrary(ENGINEDLL_PATH);
// 
// 	assert(hEngineModule);
// 
// 	std::unique_ptr<Rocket::IRocketLauncher> Rocket((reinterpret_cast<LAUNCHER_SIGNATURE>(GetProcAddress(hEngineModule, LAUNCHER_NAME)))());
// }
