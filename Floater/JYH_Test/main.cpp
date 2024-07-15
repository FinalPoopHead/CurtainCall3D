#define FLT_WINDOWS
#define _CRT_SECURE_NO_WARNINGS
#include "../FloaterUtil/include/Type.h"
#include "../FloaterGameEngine/include\EngineMinimal.h"
#include "../FloaterGameEngine/include/internal/GameEngine.h"
#include "TestScene.h"
#include "UnityLoadScene.h"
#include <iostream>
#include <filesystem>


#include "../FloaterMath/include/floaterMath.h"
#include "../FloaterUtil/include/Log.h"
#include "../FloaterUtil/include/OwningPtr.h"
#include "../FloaterUtil/include/NonOwningPtr.h"

/// 자료구조 테스트용 헤더
#include "../FloaterUtil/include/SparseSet.h"
#include "../FloaterUtil/include/StaticArray.h"

/// 트윈 테스트용 헤더
#include "../FloaterUtil/include/FLTween.h"
#include "../FloaterMath/include/Ease.h"
#include "../FloaterMath/include/Bezier.h"

#include <array>

//void Func(flt::info info);

int main(int argc, char* argv[])
{
	setlocale(LC_ALL, ".UTF8");
	std::cout << std::boolalpha;

	///자료구조 테스트
	/*{
		using namespace flt;

		StaticArray arr = { 1, 2, 3 };
		//StaticArray arr2(2, 3, 4);

		//StaticArray<int, 10> arr3{ 1, 2, };

		//int test = arr[0];
		//test = arr.At(1);
		//test = arr[2];
		//test = arr.At(3);

		//test = arr2[0];
		//test = arr2.At(1);
		//test = arr2[2];
		//test = arr2.At(3);

		////std::array stdArr = { 1, 2, 3 };
		////std::array staArr2(2, 3, 4);

		//int i = 0;
	}*/

	/// 로그 테스트
	/*{
		flt::Info(L"Hello {}!", L"World");
	}*/

	/// 트윈 테스트
	{
		int value = 100;
		int valueRef = 100;

		auto ret = flt::defaultLerp(1, 3, 0.5f);
		int i = 2;
		int j = 6;
		ret = flt::defaultLerp(i, j, 0.5f);
		int xx = 0;

		auto onStepFunc = [](const int& value)
			{ 
				for (int j = 0; j < value; ++j)
				{
					std::cout << " ";
				}
				std::cout << "*\n";
			};

		flt::FLTween tween = flt::tween::from(0)
			.to(100).during(100.0f).easing(flt::ease::easeInOut).onStart([]() { std::cout << "Start\n"; }).onStep(onStepFunc).onEnd([]() { std::cout << "End\n"; })
			.to(0).during(100.0f).easing(flt::ease::bezierEaseInOut).onStep(onStepFunc);

		for (int i = 0; i < 210; ++i)
		{
			int value = tween.step(1.0f);
		}

		std::cout << "------------------\n";

		flt::FLTween tweenPtr = flt::tween::from(&value)
			.from(0)
			.to(100).preDelay(10.0f).during(10.0f).postDelay(3.0f).easing(flt::Bezier::EaseInOut()).onStep(onStepFunc)
			.to(0).preDelay(2.0f).during(100.0f).easing(flt::Bezier::EaseIn()).onStep(onStepFunc);

		for (int i = 0; i < 110; ++i)
		{
			tweenPtr.step(1.0f);
		}

		std::cout << "------------------\n";

		flt::FLTween tweenRef = flt::tween::from<int&>(valueRef)
			.from(50)
			.to(100).preDelay(100.0f).during(10.0f).easing(flt::Bezier::EaseInOut())
			.to(0).during(10.0f).easing(flt::Bezier::EaseIn());

		for (int i = 0; i < 310; ++i)
		{
			tweenRef.step(1.0f);
			for (int j = 0; j < valueRef; ++j)
			{
				std::cout << " ";
			}
			std::cout << "*\n";
		}
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
