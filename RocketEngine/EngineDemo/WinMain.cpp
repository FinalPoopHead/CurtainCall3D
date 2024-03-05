// ���ø����̼ǿ� ���� �������� �����մϴ�.
//
#include <windows.h>
#include <memory>
#include <cassert>
//#include "..\\Rocket\\IEngineProcess.h"
#include "..\\RocketEngine\\RocketAPI.h"
#include "Client.h"

// ���� ����:
HINSTANCE g_hInst;                                // ���� �ν��Ͻ��Դϴ�.
//WCHAR szTitle[MAX_LOADSTRING];                  // ���� ǥ���� �ؽ�Ʈ�Դϴ�.
//WCHAR szWindowClass[MAX_LOADSTRING];            // �⺻ â Ŭ���� �̸��Դϴ�.

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	/// RocketAPI�� ����´�.
	//RocketAPILoader::Instance().Initialize();


	/// ������ �ʱ�ȭ �ϰ� ������Ų��.
	Rocket::EngineProcess* Rocket = Rocket::CreateLauncher();
	//Rocket::IEngineProcess* Rocket = RocketAPILoader::Instance().CreateLauncher();
	Rocket->Initialize(static_cast<void*>(hInstance), nCmdShow);

	/// Ŭ���̾�Ʈ�� �ʱ�ȭ�ϰ� �غ��Ų��.
	std::unique_ptr<Client> client(new Client());
	client->Initialize();

	Rocket->RunEngine();

	/// ������ �����Ѵ�.
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
