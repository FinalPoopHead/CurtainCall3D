#include "GameScene.h"
#include "Board.h"
#include "Player.h"
#include "TestCamera.h"
#include "NormalCube.h"
#include "GameManager.h"
#include "Camera.h"
#include "GameManager.h"
#include "SpriteObject.h"

void GameScene::Initialize()
{
	GameManager* gameManager = flt::CreateGameObject<GameManager>(true);
	int playerIndex = 0;

	/// player 1

	Board* board1 = flt::CreateGameObject<Board>(true, gameManager, playerIndex++, 5, 15);
	board1->tr.SetPosition(0.0f, 0.0f, 0.0f);

	Player* player1 = flt::CreateGameObject<Player>(true, board1);

	SpriteObject* hpSlot = flt::CreateGameObject<SpriteObject>(true);
	hpSlot->SetSprite(L"../Resources/Sprites/HPSlot.png");
	hpSlot->SetOffsetPosition({ 0.9f, 0.9f });
	hpSlot->SetSize({ 64.0f,64.0f });
	hpSlot->SetZOrder(0.2f);

	SpriteObject* hpValue = flt::CreateGameObject<SpriteObject>(true);
	hpValue->SetSprite(L"../Resources/Sprites/HPValue.png");
	hpValue->SetOffsetPosition({ 0.9f, 0.9f });
	hpValue->SetSize({ 64.0f,64.0f });
	hpValue->SetZOrder(0.3f);

	gameManager->SetBoard(0, board1);
	gameManager->SetPlayer(0, player1);
	gameManager->AddPlayerHPSlot(0, hpSlot);
	gameManager->AddPlayerHPValue(0, hpValue);

	/// player 2

// 	Board* board2 = flt::CreateGameObject<Board>(true, gameManager, playerIndex++, 5, 15);
// 	board2->tr.SetPosition(25.0f, 0.0f, 0.0f);
// 
// 	Player* player2 = flt::CreateGameObject<Player>(true, board2);
// 
// 	gameManager->SetBoard(1, board2);
// 	gameManager->SetPlayer(1, player2);

}

void GameScene::Finalize()
{

}
