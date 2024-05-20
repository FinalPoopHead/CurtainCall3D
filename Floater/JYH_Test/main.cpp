#define FLT_WINDOWS
#define _CRT_SECURE_NO_WARNINGS

#include "../FloaterGameEngine/include/GameEngine.h"
#include "../FloaterPlatform/include/Platform.h"
#include "TestScene.h"
#include <iostream>

int main(int argc, char* argv[])
{
	setlocale(LC_ALL, ".UTF8");
	std::cout << std::boolalpha;

	////cpu 캐시 크기 가져오기
	////std::cout << std::hardware_destructive_interference_size << std::endl;
	  
	flt::GameEngine* pGameEngine = flt::GameEngine::Instance();
	TestScene testScene;
	pGameEngine->SetScene(&testScene);

	//int loopCount = 0;
	while (true)
	{
		pGameEngine->Update();
	}

	pGameEngine->Finalize();

	return 0;
}
