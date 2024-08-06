#include "../FloaterGameEngine/include/EngineMinimal.h"
#include "../FloaterGameEngine/include/internal/GameEngine.h"
#include "GameScene.h"
#include "MainMenuScene.h"


int main(int argc, char* argv[])
{
	flt::GameEngine* pGameEngine = flt::GameEngine::Instance();
	pGameEngine->Initialize();
	pGameEngine->SetWindowTitle(L"Cube Runner");

	pGameEngine->SetWindowSize(0, 0, flt::WindowMode::BORDERLESS);

	flt::CreateScene<MainMenuScene>();
	flt::CreateScene<GameScene>();
	flt::SetScene(L"class MainMenuScene");

	while (pGameEngine->Update())
	{
	}

	pGameEngine->Finalize();

	return 0;
}
