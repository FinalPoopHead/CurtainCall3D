#include "../FloaterGameEngine/include/EngineMinimal.h"
#include "../FloaterGameEngine/include/internal/GameEngine.h"
#include "GameScene.h"
#include "MainMenuScene.h"


int main(int argc, char* argv[])
{
	flt::GameEngine* pGameEngine = flt::GameEngine::Instance();
	pGameEngine->SetWindowTitle(L"Cube Runner");
	//pGameEngine->SetWindowFullScreen(false);
	pGameEngine->SetWindowSize();
	//GameScene* gameScene = flt::CreateScene<GameScene>();
	//MainMenuScene* mainMenuScene = flt::CreateScene<MainMenuScene>();
	//SetScene(gameScene);
	//flt::SetScene(mainMenuScene);

	flt::CreateScene<MainMenuScene>();
	flt::CreateScene<GameScene>();
	flt::SetScene(L"class MainMenuScene");

	while (pGameEngine->Update())
	{
	}

	pGameEngine->Finalize();

	return 0;
}
