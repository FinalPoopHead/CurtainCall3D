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

	///					//////////////////////////////////////////////////////////////////////////////////////////
	///		player 1	//////////////////////////////////////////////////////////////////////////////////////////
	///					//////////////////////////////////////////////////////////////////////////////////////////

	Board* board1 = flt::CreateGameObject<Board>(true, gameManager, playerIndex++, 5, 15);
	board1->tr.SetPosition(0.0f, 0.0f, 0.0f);

	Player* player1 = flt::CreateGameObject<Player>(true, board1);

	SpriteObject* p1HPPanel = flt::CreateGameObject<SpriteObject>(true);
	p1HPPanel->SetOffsetPosition({ 0.9f,0.9f });

	constexpr float offSetSize = 64.0f;
	int count = 0;

	SpriteObject* p1HPSlot1 = flt::CreateGameObject<SpriteObject>(true);
	p1HPSlot1->tr.SetParent(&p1HPPanel->tr);
	p1HPSlot1->SetSprite(L"../Resources/Sprites/HPSlot.png");
	p1HPSlot1->SetSize({ 64.0f,64.0f });
	p1HPSlot1->SetZOrder(0.2f);
	p1HPSlot1->SetPosition({ -offSetSize * count++,0.0f });

	SpriteObject* p1HPSlot2 = flt::CreateGameObject<SpriteObject>(true);
	p1HPSlot2->tr.SetParent(&p1HPPanel->tr);
	p1HPSlot2->SetSprite(L"../Resources/Sprites/HPSlot.png");
	p1HPSlot2->SetSize({ 64.0f,64.0f });
	p1HPSlot2->SetZOrder(0.2f);
	p1HPSlot2->SetPosition({ -offSetSize * count++,0.0f });

	SpriteObject* p1HPSlot3 = flt::CreateGameObject<SpriteObject>(true);
	p1HPSlot3->tr.SetParent(&p1HPPanel->tr);
	p1HPSlot3->SetSprite(L"../Resources/Sprites/HPSlot.png");
	p1HPSlot3->SetSize({ 64.0f,64.0f });
	p1HPSlot3->SetZOrder(0.2f);
	p1HPSlot3->SetPosition({ -offSetSize * count++,0.0f });

	count = 0;

	SpriteObject* p1HPValue1 = flt::CreateGameObject<SpriteObject>(true);
	p1HPValue1->tr.SetParent(&p1HPPanel->tr);
	p1HPValue1->SetSprite(L"../Resources/Sprites/HPValue.png");
	p1HPValue1->SetSize({ 64.0f,64.0f });
	p1HPValue1->SetZOrder(0.3f);
	p1HPValue1->SetPosition({ -offSetSize * count++,0.0f });

	SpriteObject* p1HPValue2 = flt::CreateGameObject<SpriteObject>(true);
	p1HPValue2->tr.SetParent(&p1HPPanel->tr);
	p1HPValue2->SetSprite(L"../Resources/Sprites/HPValue.png");
	p1HPValue2->SetSize({ 64.0f,64.0f });
	p1HPValue2->SetZOrder(0.3f);
	p1HPValue2->SetPosition({ -offSetSize * count++,0.0f });

	SpriteObject* p1HPValue3 = flt::CreateGameObject<SpriteObject>(true);
	p1HPValue3->tr.SetParent(&p1HPPanel->tr);
	p1HPValue3->SetSprite(L"../Resources/Sprites/HPValue.png");
	p1HPValue3->SetSize({ 64.0f,64.0f });
	p1HPValue3->SetZOrder(0.3f);
	p1HPValue3->SetPosition({ -offSetSize * count++,0.0f });

	// TODO : 여기다가 상위 객체 하나 두고 그 녀석의 OffsetTransform을 부여하고
	//			나머지 녀석들은 그녀석 아래로 두고 절대적인 위치로 들고있어야 될거같은데?

	gameManager->AddPlayerHPPanel(0, p1HPPanel);
	gameManager->AddPlayerHPSlot(0, p1HPSlot1);
 	gameManager->AddPlayerHPSlot(0, p1HPSlot2);
 	gameManager->AddPlayerHPSlot(0, p1HPSlot3);
	gameManager->AddPlayerHPValue(0, p1HPValue1);
	gameManager->AddPlayerHPValue(0, p1HPValue2);
	gameManager->AddPlayerHPValue(0, p1HPValue3);
	gameManager->SetBoard(0, board1);
	gameManager->SetPlayer(0, player1);

	///					//////////////////////////////////////////////////////////////////////////////////////////
	///		player 2	//////////////////////////////////////////////////////////////////////////////////////////
	///					//////////////////////////////////////////////////////////////////////////////////////////

	Board* board2 = flt::CreateGameObject<Board>(true, gameManager, playerIndex++, 5, 15);
	board2->tr.SetPosition(25.0f, 0.0f, 0.0f);

	Player* player2 = flt::CreateGameObject<Player>(true, board2);

	SpriteObject* p2HPPanel = flt::CreateGameObject<SpriteObject>(true);
	p2HPPanel->SetOffsetPosition({ 0.9f,0.9f });

	count = 0;

	SpriteObject* p2HPSlot1 = flt::CreateGameObject<SpriteObject>(true);
	p2HPSlot1->tr.SetParent(&p2HPPanel->tr);
	p2HPSlot1->SetSprite(L"../Resources/Sprites/HPSlot.png");
	p2HPSlot1->SetSize({ 64.0f,64.0f });
	p2HPSlot1->SetZOrder(0.2f);
	p2HPSlot1->SetPosition({ -offSetSize * count++,0.0f });

	SpriteObject* p2HPSlot2 = flt::CreateGameObject<SpriteObject>(true);
	p2HPSlot2->tr.SetParent(&p2HPPanel->tr);
	p2HPSlot2->SetSprite(L"../Resources/Sprites/HPSlot.png");
	p2HPSlot2->SetSize({ 64.0f,64.0f });
	p2HPSlot2->SetZOrder(0.2f);
	p2HPSlot2->SetPosition({ -offSetSize * count++,0.0f });

	SpriteObject* p2HPSlot3 = flt::CreateGameObject<SpriteObject>(true);
	p2HPSlot3->tr.SetParent(&p2HPPanel->tr);
	p2HPSlot3->SetSprite(L"../Resources/Sprites/HPSlot.png");
	p2HPSlot3->SetSize({ 64.0f,64.0f });
	p2HPSlot3->SetZOrder(0.2f);
	p2HPSlot3->SetPosition({ -offSetSize * count++,0.0f });

	count = 0;

	SpriteObject* p2HPValue1 = flt::CreateGameObject<SpriteObject>(true);
	p2HPValue1->tr.SetParent(&p2HPPanel->tr);
	p2HPValue1->SetSprite(L"../Resources/Sprites/HPValue.png");
	p2HPValue1->SetSize({ 64.0f,64.0f });
	p2HPValue1->SetZOrder(0.3f);
	p2HPValue1->SetPosition({ -offSetSize * count++,0.0f });

	SpriteObject* p2HPValue2 = flt::CreateGameObject<SpriteObject>(true);
	p2HPValue2->tr.SetParent(&p2HPPanel->tr);
	p2HPValue2->SetSprite(L"../Resources/Sprites/HPValue.png");
	p2HPValue2->SetSize({ 64.0f,64.0f });
	p2HPValue2->SetZOrder(0.3f);
	p2HPValue2->SetPosition({ -offSetSize * count++,0.0f });

	SpriteObject* p2HPValue3 = flt::CreateGameObject<SpriteObject>(true);
	p2HPValue3->tr.SetParent(&p2HPPanel->tr);
	p2HPValue3->SetSprite(L"../Resources/Sprites/HPValue.png");
	p2HPValue3->SetSize({ 64.0f,64.0f });
	p2HPValue3->SetZOrder(0.3f);
	p2HPValue3->SetPosition({ -offSetSize * count++,0.0f });

	gameManager->AddPlayerHPPanel(1, p2HPPanel);
	gameManager->AddPlayerHPSlot(1, p2HPSlot1);
	gameManager->AddPlayerHPSlot(1, p2HPSlot2);
	gameManager->AddPlayerHPSlot(1, p2HPSlot3);
	gameManager->AddPlayerHPValue(1, p2HPValue1);
	gameManager->AddPlayerHPValue(1, p2HPValue2);
	gameManager->AddPlayerHPValue(1, p2HPValue3);
	gameManager->SetBoard(1, board2);
	gameManager->SetPlayer(1, player2);
}

void GameScene::Finalize()
{

}
