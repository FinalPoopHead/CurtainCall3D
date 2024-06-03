#include "GameScene.h"
#include "Board.h"
#include "TestCamera.h"

void GameScene::Initialize()
{
	Board* board = flt::CreateGameObject<Board>(true, 2, 2);
	board->tr.SetPosition(0.0f, 0.0f, 0.0f);

	TestCamera* camera = flt::CreateGameObject<TestCamera>(true);
	camera->tr.SetPosition(0.0f, 0.0f, -10.0f);
}

void GameScene::Finalize()
{

}
