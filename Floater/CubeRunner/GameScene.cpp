﻿#include "GameScene.h"
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

int g_player1Input = -1;
int g_player2Input = -1;

constexpr int STAGE = 1;
constexpr int WIDTH = 4;
constexpr int HEIGHT = 15;

void GameScene::Initialize()
{
	// TODO : 매개변수로 몇 스테이지인지(width 값) 받아야되려나?

	GameManager* gameManager = flt::CreateGameObject<GameManager>(true);

	DirectionalLight* light = flt::CreateGameObject<DirectionalLight>(true);

	float boardOffset = 512.0f;
	for (int playerIndex = 0; playerIndex < g_PlayerNum; playerIndex++)
	{
		gameManager->CreateUI(playerIndex);						// UI를 먼저 만들고 Player와 Board를 등록해야함.
		
		Board* board = flt::CreateGameObject<Board>(true, gameManager, playerIndex, WIDTH, HEIGHT);
		board->tr.SetPosition(boardOffset * playerIndex, 0.0f, 0.0f);

		Player* player = flt::CreateGameObject<Player>(true, gameManager, board);
		player->camera->SetCameraIndex(playerIndex);

		gameManager->SetBoardAndPlayer(playerIndex, board, player);	// UI를 먼저 만들고 Player와 Board를 등록해야함.
	}

	gameManager->ResetGame();
	if (g_PlayerNum < 2)
	{
		gameManager->SetStage(g_StageNum);
	}
	else
	{
		gameManager->SetBattleMode();
	}
}

void GameScene::Finalize()
{

}

