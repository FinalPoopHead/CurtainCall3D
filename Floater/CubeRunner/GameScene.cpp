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

constexpr flt::Vector2f HPPANEL_OFFSETPOS = { 0.9f,0.95f };
constexpr flt::Vector2f TIMEPANEL_OFFSETPOS = { 0.8f,0.05f };
constexpr flt::Vector2f SCOREPANEL_OFFSETPOS = { 0.05f,0.05f };

constexpr float HPOFFSETSIZE = 128.0f;
constexpr flt::Vector2f HPUISIZE = { HPOFFSETSIZE,HPOFFSETSIZE };

constexpr float HPSlotZorder = 0.2f;
constexpr float HPValueZorder = 0.3f;

constexpr flt::Vector4f TimeTextColor = { 1.0f,1.0f,1.0f,1.0f };
constexpr flt::Vector4f ScoreTextColor = { 1.0f,1.0f,1.0f,1.0f };

void GameScene::Initialize()
{
	std::wstring hpSlotPath = L"../Resources/Sprites/HPSlot.png";
	std::wstring hpValuePath = L"../Resources/Sprites/HPValue.png";
	std::wstring fontPath = L"../Resources/Fonts/LineSeedSansKR_KoreanCompatible_40.spritefont";

	GameManager* gameManager = flt::CreateGameObject<GameManager>(true);
	int playerIndex = 0;

	DirectionalLight* light = flt::CreateGameObject<DirectionalLight>(true);

	///	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	///	//////////////////////////////////////////		player 1	//////////////////////////////////////////////
	///	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	Board* board1 = flt::CreateGameObject<Board>(true, gameManager, playerIndex++, 5, 15);
	board1->tr.SetPosition(0.0f, 0.0f, 0.0f);

	Player* player1 = flt::CreateGameObject<Player>(true, board1);

	SpriteObject* p1HPPanel = flt::CreateGameObject<SpriteObject>(true);
	p1HPPanel->SetOffsetPosition(HPPANEL_OFFSETPOS);

	int hpCount = 0;

	SpriteObject* p1HPSlot1 = flt::CreateGameObject<SpriteObject>(true);
	p1HPSlot1->tr.SetParent(&p1HPPanel->tr);
	p1HPSlot1->SetSprite(hpSlotPath);
	p1HPSlot1->SetSize(HPUISIZE);
	p1HPSlot1->SetZOrder(HPSlotZorder);
	p1HPSlot1->SetPosition({ -HPOFFSETSIZE * hpCount++,0.0f });

	SpriteObject* p1HPSlot2 = flt::CreateGameObject<SpriteObject>(true);
	p1HPSlot2->tr.SetParent(&p1HPPanel->tr);
	p1HPSlot2->SetSprite(hpSlotPath);
	p1HPSlot2->SetSize(HPUISIZE);
	p1HPSlot2->SetZOrder(HPSlotZorder);
	p1HPSlot2->SetPosition({ -HPOFFSETSIZE * hpCount++,0.0f });

	SpriteObject* p1HPSlot3 = flt::CreateGameObject<SpriteObject>(true);
	p1HPSlot3->tr.SetParent(&p1HPPanel->tr);
	p1HPSlot3->SetSprite(hpSlotPath);
	p1HPSlot3->SetSize(HPUISIZE);
	p1HPSlot3->SetZOrder(HPSlotZorder);
	p1HPSlot3->SetPosition({ -HPOFFSETSIZE * hpCount++,0.0f });

	hpCount = 0;

	SpriteObject* p1HPValue1 = flt::CreateGameObject<SpriteObject>(true);
	p1HPValue1->tr.SetParent(&p1HPPanel->tr);
	p1HPValue1->SetSprite(hpValuePath);
	p1HPValue1->SetSize(HPUISIZE);
	p1HPValue1->SetZOrder(HPValueZorder);
	p1HPValue1->SetPosition({ -HPOFFSETSIZE * hpCount++,0.0f });

	SpriteObject* p1HPValue2 = flt::CreateGameObject<SpriteObject>(true);
	p1HPValue2->tr.SetParent(&p1HPPanel->tr);
	p1HPValue2->SetSprite(hpValuePath);
	p1HPValue2->SetSize(HPUISIZE);
	p1HPValue2->SetZOrder(HPValueZorder);
	p1HPValue2->SetPosition({ -HPOFFSETSIZE * hpCount++,0.0f });

	SpriteObject* p1HPValue3 = flt::CreateGameObject<SpriteObject>(true);
	p1HPValue3->tr.SetParent(&p1HPPanel->tr);
	p1HPValue3->SetSprite(hpValuePath);
	p1HPValue3->SetSize(HPUISIZE);
	p1HPValue3->SetZOrder(HPValueZorder);
	p1HPValue3->SetPosition({ -HPOFFSETSIZE * hpCount++,0.0f });

	TextObject* p1Time = flt::CreateGameObject<TextObject>(true);
	p1Time->SetOffsetPosition(TIMEPANEL_OFFSETPOS);
	p1Time->SetText(L"00:00");
	p1Time->SetFont(fontPath);
	p1Time->SetTextColor(TimeTextColor);

	TextObject* p1Score = flt::CreateGameObject<TextObject>(true);
	p1Score->SetOffsetPosition(SCOREPANEL_OFFSETPOS);
	p1Score->SetText(L"0");
	p1Score->SetFont(fontPath);
	p1Score->SetTextColor(ScoreTextColor);

	gameManager->AddPlayerHPPanel(0, p1HPPanel);
	gameManager->AddPlayerHPSlot(0, p1HPSlot1);
	gameManager->AddPlayerHPSlot(0, p1HPSlot2);
	gameManager->AddPlayerHPSlot(0, p1HPSlot3);
	gameManager->AddPlayerHPValue(0, p1HPValue1);
	gameManager->AddPlayerHPValue(0, p1HPValue2);
	gameManager->AddPlayerHPValue(0, p1HPValue3);
	gameManager->AddPlayTimeText(0, p1Time);
	gameManager->AddPlayerScoreText(0, p1Score);
	gameManager->SetBoard(0, board1);
	gameManager->SetPlayer(0, player1);

	///	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	///	//////////////////////////////////////////		player 2	//////////////////////////////////////////////
	///	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	Board* board2 = flt::CreateGameObject<Board>(true, gameManager, playerIndex++, 5, 15);
	board2->tr.SetPosition(40.0f, 0.0f, 0.0f);

	Player* player2 = flt::CreateGameObject<Player>(true, board2);

	SpriteObject* p2HPPanel = flt::CreateGameObject<SpriteObject>(true);
	p2HPPanel->SetOffsetPosition(HPPANEL_OFFSETPOS);

	hpCount = 0;

	SpriteObject* p2HPSlot1 = flt::CreateGameObject<SpriteObject>(true);
	p2HPSlot1->tr.SetParent(&p2HPPanel->tr);
	p2HPSlot1->SetSprite(hpSlotPath);
	p2HPSlot1->SetSize(HPUISIZE);
	p2HPSlot1->SetZOrder(HPSlotZorder);
	p2HPSlot1->SetPosition({ -HPOFFSETSIZE * hpCount++,0.0f });

	SpriteObject* p2HPSlot2 = flt::CreateGameObject<SpriteObject>(true);
	p2HPSlot2->tr.SetParent(&p2HPPanel->tr);
	p2HPSlot2->SetSprite(hpSlotPath);
	p2HPSlot2->SetSize(HPUISIZE);
	p2HPSlot2->SetZOrder(HPSlotZorder);
	p2HPSlot2->SetPosition({ -HPOFFSETSIZE * hpCount++,0.0f });

	SpriteObject* p2HPSlot3 = flt::CreateGameObject<SpriteObject>(true);
	p2HPSlot3->tr.SetParent(&p2HPPanel->tr);
	p2HPSlot3->SetSprite(hpSlotPath);
	p2HPSlot3->SetSize(HPUISIZE);
	p2HPSlot3->SetZOrder(HPSlotZorder);
	p2HPSlot3->SetPosition({ -HPOFFSETSIZE * hpCount++,0.0f });

	hpCount = 0;

	SpriteObject* p2HPValue1 = flt::CreateGameObject<SpriteObject>(true);
	p2HPValue1->tr.SetParent(&p2HPPanel->tr);
	p2HPValue1->SetSprite(hpValuePath);
	p2HPValue1->SetSize(HPUISIZE);
	p2HPValue1->SetZOrder(HPValueZorder);
	p2HPValue1->SetPosition({ -HPOFFSETSIZE * hpCount++,0.0f });

	SpriteObject* p2HPValue2 = flt::CreateGameObject<SpriteObject>(true);
	p2HPValue2->tr.SetParent(&p2HPPanel->tr);
	p2HPValue2->SetSprite(hpValuePath);
	p2HPValue2->SetSize(HPUISIZE);
	p2HPValue2->SetZOrder(HPValueZorder);
	p2HPValue2->SetPosition({ -HPOFFSETSIZE * hpCount++,0.0f });

	SpriteObject* p2HPValue3 = flt::CreateGameObject<SpriteObject>(true);
	p2HPValue3->tr.SetParent(&p2HPPanel->tr);
	p2HPValue3->SetSprite(hpValuePath);
	p2HPValue3->SetSize(HPUISIZE);
	p2HPValue3->SetZOrder(HPValueZorder);
	p2HPValue3->SetPosition({ -HPOFFSETSIZE * hpCount++,0.0f });

	TextObject* p2Time = flt::CreateGameObject<TextObject>(true);
	p2Time->SetOffsetPosition(TIMEPANEL_OFFSETPOS);
	p2Time->SetText(L"00:00");
	p2Time->SetFont(fontPath);
	p2Time->SetTextColor(TimeTextColor);

	TextObject* p2Score = flt::CreateGameObject<TextObject>(true);
	p2Score->SetOffsetPosition(SCOREPANEL_OFFSETPOS);
	p2Score->SetText(L"0");
	p2Score->SetFont(fontPath);
	p2Score->SetTextColor(ScoreTextColor);

	gameManager->AddPlayerHPPanel(1, p2HPPanel);
	gameManager->AddPlayerHPSlot(1, p2HPSlot1);
	gameManager->AddPlayerHPSlot(1, p2HPSlot2);
	gameManager->AddPlayerHPSlot(1, p2HPSlot3);
	gameManager->AddPlayerHPValue(1, p2HPValue1);
	gameManager->AddPlayerHPValue(1, p2HPValue2);
	gameManager->AddPlayerHPValue(1, p2HPValue3);
	gameManager->AddPlayTimeText(1, p2Time);
	gameManager->AddPlayerScoreText(1, p2Score);
	gameManager->SetBoard(1, board2);
	gameManager->SetPlayer(1, player2);

	player1->SetAlbedoPath(L"..\\Resources\\Textures\\Rob02Red_AlbedoTransparency.png");
	player2->SetAlbedoPath(L"..\\Resources\\Textures\\Rob02Blue_AlbedoTransparency.png");
}

void GameScene::Finalize()
{

}
