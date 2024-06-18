#define FLT_WINDOWS
#define _CRT_SECURE_NO_WARNINGS

#include "../FloaterGameEngine/include\EngineMinimal.h"
#include "../FloaterGameEngine/include/internal/GameEngine.h"
#include "TestScene.h"
#include "UnityLoadScene.h"
#include <iostream>
#include <filesystem>

#include "../FloaterMath/include/Bezier.h"
#include "../FloaterMath/include/floaterMath.h"
#include "../FloaterUtil/include/Log.h"


int main(int argc, char* argv[])
{
	setlocale(LC_ALL, ".UTF8");
	std::cout << std::boolalpha;

	flt::info(L"Hello World!");

	int i = 0;

	//std::wstring kString = L"한글 테스트";
	//auto format = std::format(kStringView, i);
	//std::wcout << format << "\n";
	constexpr std::wstring_view kStringView = L"한글 테스트";
	flt::info(kStringView);

	std::filesystem::path path = std::filesystem::current_path();

	////cpu 캐시 크기 가져오기
	////std::cout << std::hardware_destructive_interference_size << std::endl;
	  
	flt::GameEngine* pGameEngine = flt::GameEngine::Instance();
	
	//UnityLoadScene unityLoadScene(L"..\\x64\\data\\SoaringMap.json");
	//pGameEngine->SetScene(&unityLoadScene);

	TestScene* testScene = flt::CreateScene<TestScene>();
	pGameEngine->SetScene(testScene);

	//int loopCount = 0;
	while (pGameEngine->Update())
	{
	}

	pGameEngine->Finalize();

	return 0;
}
