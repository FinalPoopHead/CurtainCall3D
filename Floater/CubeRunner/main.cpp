#include "../FloaterGameEngine/include/EngineMinimal.h"
#include "../FloaterGameEngine/include/internal/GameEngine.h"
#include "GameScene.h"


int main(int argc, char* argv[])
{
	flt::GameEngine* pGameEngine = flt::GameEngine::Instance();
	
	GameScene* gameScene = flt::CreateScene<GameScene>();
	SetScene(gameScene);

	while (pGameEngine->Update())
	{
	}

	pGameEngine->Finalize();

	return 0;
}
