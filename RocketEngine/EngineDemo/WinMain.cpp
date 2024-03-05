// 애플리케이션에 대한 진입점을 정의합니다.
//
#include <windows.h>
#include <memory>
#include <cassert>
//#include "..\\Rocket\\IEngineProcess.h"
#include "..\\RocketEngine\\RocketAPI.h"
#include "Client.h"

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
	std::unique_ptr<Client> client(new Client());
	client->Initialize();

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
