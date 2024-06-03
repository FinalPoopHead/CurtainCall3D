#include "../FloaterGameEngine/include/internal/GameEngine.h"
#include "GameScene.h"


int main(int argc, char* argv[])
{
	flt::GameEngine* pGameEngine = flt::GameEngine::Instance();
	
	GameScene gameScene;
	pGameEngine->SetScene(&gameScene);

	while (pGameEngine->Update())
	{
	}

	pGameEngine->Finalize();

	return 0;
}
