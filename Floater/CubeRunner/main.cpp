#include "../FloaterGameEngine/include/EngineMinimal.h"
#include "../FloaterGameEngine/include/internal/GameEngine.h"
#include "GameScene.h"
#include "MainMenuScene.h"


int main(int argc, char* argv[])
{
	flt::GameEngine* pGameEngine = flt::GameEngine::Instance();
	pGameEngine->SetWindowTitle(L"Cube Runner");
	pGameEngine->SetWindowSize(1280, 720, flt::WindowMode::FULLSCREEN);

	flt::CreateScene<MainMenuScene>();
	flt::CreateScene<GameScene>();
	flt::SetScene(L"class MainMenuScene");

	while (pGameEngine->Update())
	{
	}

	pGameEngine->Finalize();

	return 0;
}
