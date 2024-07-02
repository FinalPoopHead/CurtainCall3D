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

int g_PlayerNum = 1;
int g_StageNum = 1;

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

void GameScene::Initialize()
{
	// TODO : 매개변수로 몇 스테이지인지(width 값) 받아야되려나?

	GameManager* gameManager = flt::CreateGameObject<GameManager>(true);

	DirectionalLight* light = flt::CreateGameObject<DirectionalLight>(true);

	float boardOffset = 100.0f;
	for (int playerIndex = 0; playerIndex < g_PlayerNum; playerIndex++)
	{
		Board* board = flt::CreateGameObject<Board>(true, gameManager, playerIndex, WIDTH, HEIGHT);
		board->tr.SetPosition(boardOffset * playerIndex, 0.0f, 0.0f);

		Player* player = flt::CreateGameObject<Player>(true, board);
		player->camera->GetComponent<flt::CameraComponent>()->SetIndex(playerIndex);

		gameManager->CreateUI(playerIndex, WIDTH);						// UI를 먼저 만들고 Player와 Board를 등록해야함.
		gameManager->SetBoardAndPlayer(playerIndex, board, player);
	}

	gameManager->SetStage(g_StageNum);		// TEST : 임시
}

void GameScene::Finalize()
{

}

