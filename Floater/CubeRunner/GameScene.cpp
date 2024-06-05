#include "GameScene.h"
#include "Board.h"
#include "TestCamera.h"
#include "NormalCube.h"
#include "ManagerObject.h"

void GameScene::Initialize()
{
	ManagerObject* managerObj = flt::CreateGameObject<ManagerObject>(true);

	Board* board = flt::CreateGameObject<Board>(true, 5, 10);
	board->tr.SetPosition(0.0f, 0.0f, 0.0f);

	TestCamera* camera = flt::CreateGameObject<TestCamera>(true, board);
	camera->tr.SetPosition(0.0f, 5.0f, -10.0f);

	NormalCube* cube = flt::CreateGameObject<NormalCube>(true);
	cube->tr.SetPosition(0.0f, 4.0f, 36.4f);
}

void GameScene::Finalize()
{

}
