#include "GameScene.h"
#include "Board.h"
#include "Player.h"
#include "TestCamera.h"
#include "NormalCube.h"
#include "GameManager.h"
#include "Camera.h"
#include "GameManager.h"

void GameScene::Initialize()
{
	GameManager* gameManager = flt::CreateGameObject<GameManager>(true);
	int playerIndex = 0;

	Board* board1 = flt::CreateGameObject<Board>(true, gameManager, playerIndex++, 5, 10);
	board1->tr.SetPosition(0.0f, 0.0f, 0.0f);

	Player* player1 = flt::CreateGameObject<Player>(true, board1);
	
 	TestCamera* camera = flt::CreateGameObject<TestCamera>(true, board1);
 	camera->tr.SetPosition(0.0f, 5.0f, -10.0f);

// 	Camera* camera = flt::CreateGameObject<Camera>(true);
// 	camera->tr.SetPosition(0.0f, 5.0f, -10.0f);

// 	NormalCube* cube = flt::CreateGameObject<NormalCube>(true);
// 	cube->tr.SetPosition(0.0f, 4.0f, 36.0f);
}

void GameScene::Finalize()
{

}
