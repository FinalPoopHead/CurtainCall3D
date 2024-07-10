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
#include "../FloaterUtil/include/OwningPtr.h"
#include "../FloaterUtil/include/NonOwningPtr.h"
#include "../FloaterUtil/FLTween.h"
#include "../FloaterUtil/include/SparseSet.h"

//void Func(flt::info info);

int main(int argc, char* argv[])
{
	setlocale(LC_ALL, ".UTF8");
	std::cout << std::boolalpha;

	//flt::Info(L"Hello {}!", L"World");

	//{
	//	flt::OwningPtr<int> pInt = new int(10);
	//	flt::NonOwningPtr<int> pInt2{ pInt };
	//	flt::OwningPtr<int> pInt3 = std::move(pInt);

	//	std::cout << *pInt3 << std::endl;

	//	const flt::OwningPtr<int> cpInt = new int(10);
	//	const flt::NonOwningPtr<int> cpInt2{ cpInt };
	//}

	//{
	//	flt::SparseSet<float> sparseSet;
	//	sparseSet.Insert(1.0f);
	//	sparseSet.Insert(2.0f);
	//	sparseSet.Insert(3.0f);
	//	sparseSet.Insert(4.0f);
	//	sparseSet.Insert(5.0f);
	//	sparseSet.Insert(6.0f);
	//	sparseSet.Insert(7.0f);

	//	for (auto& value : sparseSet)
	//	{
	//		std::cout << value << " ";
	//	}
	//	std::cout << std::endl;

	//	sparseSet.Reserve(5);

	//	for (auto& value : sparseSet)
	//	{
	//		std::cout << value << " ";
	//	}
	//	std::cout << std::endl;

	//	sparseSet.Remove(3);

	//	for (auto& value : sparseSet)
	//	{
	//		std::cout << value << " ";
	//	}
	//	std::cout << std::endl;

	//	auto iters = sparseSet.Find(5.0f);
	//}

	auto tween = flt::tween::from(0).to(100).during(100.0f);

	for(int i = 0; i < 100; ++i)
	{
		std::cout << tween.step(1.0f) << std::endl;
	}



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
