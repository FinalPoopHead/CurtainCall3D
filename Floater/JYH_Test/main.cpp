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
#include <windows.h>

#include <array>

//void Func(flt::info info);

template<typename T>
T& unmove(T&& t)
{
	return t;
}

int main(int argc, char* argv[])
{
	int ghag = 0;
	int* pghag = &ghag;

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
	{
		std::wstring str = L"Hello {}!";
		std::wstring str2 = L"hi";
		flt::Info(str, L"World");

		flt::Info(L"Debug Test {}, {:2}, {}", 156, 2.12345f, str2);
		//auto arr = std::make_wformat_args(unmove(1));
		//auto arr = std::make_wformat_args(unmove(2.1f));
		//auto arr = std::make_wformat_args(unmove(L"finished"));
	}

	/// 베지어 테스트
	/*{
		auto inout = flt::Bezier::EaseInOut();

		for (int i = 0; i < 100; ++i)
		{
			float t = i / 100.0f;
			auto result = inout.Evaluate(t);

			std::cout << result.x << ", " << result.y << std::endl;
		}
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


		{
			flt::FLTween tween = flt::tween::from(0)
				.to(100).during(100.0f).onStep(onStepFunc)
				.to(0).during(100.0f).easing(flt::ease::easeInOutBounce).onStep(onStepFunc);

			for (int i = 0; i < 200; ++i)
			{
				tween.step(1.0f);
			}
			
		}




		flt::Timer timer;
		timer.Start();

		auto onStartFunc = [&timer]() { std::cout << timer.GetTotalSeconds() << " s "; };
		auto onEndFunc = [&timer]() { std::cout << timer.GetTotalSeconds() << " e "; };
		flt::FLTween tween = flt::tween::from(0)
			.to(10).preDelay(2.0f).during(10.0f).postDelay(2.0f).easing(flt::Bezier::EaseInOut()).onStart(onStartFunc).onStep(onStepFunc).onEnd(onEndFunc)
			.to(0).preDelay(3.0f).during(10.0f).postDelay(3.0f).easing(flt::ease::easeInOut).onStart(onStartFunc).onStep(onStepFunc).onEnd(onEndFunc)
			.to(10).preDelay(4.0f).during(10.0f).postDelay(4.0f).easing(flt::ease::easeInOutExpo).onStart(onStartFunc).onStep(onStepFunc).onEnd(onEndFunc);


		flt::Timer timer2;
		timer2.Start();
		for (int i = 0; i < 50; ++i)
		{
			timer2.Update();
			float dt = timer2.GetDeltaSeconds();
			std::cout << timer2.GetTotalSeconds() << " : ";
			int value = tween.step(dt);
			std::cout << "\n";
			Sleep(1000);
		}

		std::cout << "------------------\n";

		flt::FLTween tweenPtr = flt::tween::from(&value)
			.from(0)
			.to(10).preDelay(2.0f).during(10.0f).postDelay(3.0f).easing(flt::Bezier::EaseInOut()).onStep(onStepFunc)
			.to(0).preDelay(2.0f).during(10.0f).postDelay(3.0f).easing(flt::Bezier::EaseIn()).onStep(onStepFunc);

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
	//pGameEngine->SetWindowSize(0, 0, flt::WindowMode::WINDOWED);

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
