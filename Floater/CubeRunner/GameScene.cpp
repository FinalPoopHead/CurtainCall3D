#include "GameScene.h"
#include "DirectionalLight.h"
#include "Board.h"
#include "Player.h"
#include "TestCamera.h"
#include "NormalCube.h"
#include "GameManager.h"
#include "Camera.h"
#include "GameManager.h"
#include "SpriteObject.h"
#include "TextObject.h"

int g_PlayerNum;
int g_StageNum;

constexpr flt::Vector2f HPPANEL_OFFSETPOS = { 0.9f,0.95f };
constexpr flt::Vector2f TIMEPANEL_OFFSETPOS = { 0.8f,0.05f };
constexpr flt::Vector2f SCOREPANEL_OFFSETPOS = { 0.05f,0.05f };

constexpr float HPOFFSETSIZE = 80.0f;
constexpr flt::Vector2f HPUISIZE = { 64.0f,20.0f };

constexpr float HPSlotZorder = 0.2f;
constexpr float HPValueZorder = 0.3f;

constexpr flt::Vector4f TimeTextColor = { 1.0f,1.0f,1.0f,1.0f };
constexpr flt::Vector4f ScoreTextColor = { 1.0f,1.0f,1.0f,1.0f };

constexpr int STAGE = 1;
constexpr int WIDTH = 5;
constexpr int HEIGHT = 15;

void GameScene::OnEnable()
{
	// TODO : 매개변수로 몇 스테이지인지(width 값) 받아야되려나?

	GameManager* gameManager = flt::CreateGameObject<GameManager>(true);

	DirectionalLight* light = flt::CreateGameObject<DirectionalLight>(true);

	///	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	///	//////////////////////////////////////////		player 1	//////////////////////////////////////////////
	///	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	int playerIndex = 0;

	Board* board1 = flt::CreateGameObject<Board>(true, gameManager, playerIndex, WIDTH, HEIGHT);
	board1->tr.SetPosition(0.0f, 0.0f, 0.0f);

	Player* player1 = flt::CreateGameObject<Player>(true, board1);

	gameManager->CreateUI(playerIndex, WIDTH);						// UI를 먼저 만들고 Player와 Board를 등록해야함.
	gameManager->SetBoardAndPlayer(playerIndex, board1, player1);

	///	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	///	//////////////////////////////////////////		player 2	//////////////////////////////////////////////
	///	//////////////////////////////////////////////////////////////////////////////////////////////////////////

// 	playerIndex++;
// 
// 	Board* board2 = flt::CreateGameObject<Board>(true, gameManager, playerIndex, WIDTH, HEIGHT);
// 	board2->tr.SetPosition(40.0f, 0.0f, 0.0f);
// 
// 	Player* player2 = flt::CreateGameObject<Player>(true, board2);
// 
// 	gameManager->CreateUI(playerIndex, WIDTH);						// UI를 먼저 만들고 Player와 Board를 등록해야함.
// 	gameManager->SetBoardAndPlayer(playerIndex, board2, player2);
}

void GameScene::OnDisable()
{

}
