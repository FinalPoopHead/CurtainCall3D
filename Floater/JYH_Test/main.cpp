#define FLT_WINDOWS
#define _CRT_SECURE_NO_WARNINGS
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>

#include "../FloaterGameEngine/include\EngineMinimal.h"
#include "../FloaterGameEngine/include/internal/GameEngine.h"
#include "TestScene.h"
#include "UnityLoadScene.h"
#include <iostream>
#include <filesystem>

#include "../FloaterMath/include/Bezier.h"
#include "../FloaterMath/include/floaterMath.h"
#include "../FloaterUtil/include/Log.h"
#include "../FloaterUtil/include/OwningPtr.h"
#include "../FloaterUtil/include/NonOwningPtr.h"
#include "../FloaterUtil/FLTween.h"
#include "../FloaterUtil/include/SparseSet.h"

#ifdef _DEBUG

//#define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )   

#define malloc(s) _malloc_dbg(s, _NORMAL_BLOCK, __FILE__, __LINE__)

#endif
//void Func(flt::info info);

int main(int argc, char* argv[])
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);


	setlocale(LC_ALL, ".UTF8");
	std::cout << std::boolalpha;

	////cpu 캐시 크기 가져오기
	////std::cout << std::hardware_destructive_interference_size << std::endl;

	//flt::GameEngine* pGameEngine = flt::GameEngine::Instance();
	//
	////UnityLoadScene unityLoadScene(L"..\\x64\\data\\SoaringMap.json");
	////pGameEngine->SetScene(&unityLoadScene);

	//TestScene* testScene = flt::CreateScene<TestScene>();
	//pGameEngine->SetScene(testScene);

	////int loopCount = 0;
	//while (pGameEngine->Update())
	//{
	//}

	//pGameEngine->Finalize();

	//_CrtDumpMemoryLeaks();
	int* pi = new(std::nothrow) int(10);
	std::cout << *pi << std::endl;

	return 0;
}
