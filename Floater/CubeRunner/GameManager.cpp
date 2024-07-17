#include <fstream>
#include <sstream>
#include <random>
#include "GameManager.h"
#include "../FloaterGameEngine/include/Input.h"
#include "../FloaterGameEngine/include/MakeTween.h"

#include "Player.h"
#include "Camera.h"
#include "Board.h"
#include "SpriteObject.h"
#include "TextObject.h"

constexpr int MAXPLAYERCOUNT = 2;
constexpr int MAXSTAGECOUNT = 9;
constexpr int CUBESCORE = 100;
constexpr int COMBOTEXTPOOLCOUNT = 20;
constexpr int MISSILEPOOLCOUNT = 32;
constexpr int MAXFALLCOUNT = 6;			// 최대 fallcount 개수
constexpr float COMBOTEXTSPEED = 0.2f;
constexpr flt::Vector2f COMBOTEXTPOSITION = { 0.05f,0.85f };

// UI 관련
constexpr flt::Vector2f HPPANEL_OFFSETPOS = { 0.9f,0.95f };
constexpr flt::Vector2f GARBAGEPANEL_OFFSETPOS = { 0.8f,0.1f };
constexpr flt::Vector2f STAGEINFOPANEL_OFFSETPOS = { 0.05f,0.1f };
constexpr flt::Vector2f GAMEOVERPANEL_OFFSETPOS = { 0.5f,0.5f };

constexpr float STAGESLOTZORDER = 0.2f;

constexpr float LEVELSLOTZORDER = 0.2f;
constexpr float LEVELVALUEZORDER = 0.3f;

constexpr float FALLCOUNTSLOTZORDER = 0.2f;
constexpr float FALLCOUNTREDZORDER = 0.3f;
constexpr float FALLCOUNTOFFSET = 50.0f;

constexpr flt::Vector4f whiteColor = { 1.0f,1.0f,1.0f,1.0f };

std::wstring counterSlotPath = L"../Resources/Sprites/FallCounterSlot.png";
std::wstring counterRedPath = L"../Resources/Sprites/FallCounterRed.png";

GameManager::GameManager() :
	_soundComponent()
	, _soundIndex()
	, _players()
	, _boards()
	, _stageInfoPanel()
	, _levelCountSlot()
	, _levelCountBlue()
	, _stageCountText()
	, _playerScoreText()
	, _fallCountPanel()
	, _fallCountSlot()
	, _fallCountRed()
	, _heightCountText()
	, _garbageLineText()
	, _gameoverTextPanel()
	, _missilePool()
	, _liveComboTexts()
	, _comboTextPool()
	, _isGameOver(std::vector<bool>(MAXPLAYERCOUNT))
	, _fallCount(std::vector<int>(MAXPLAYERCOUNT))
	, _fallCountMax(std::vector<int>(MAXPLAYERCOUNT))
	, _playTime(std::vector<float>(MAXPLAYERCOUNT))
	, _playerScore(std::vector<int>(MAXPLAYERCOUNT))
	, _comboTextPos(std::vector<flt::Vector2f>(MAXPLAYERCOUNT))
	, _stageData()
	, _currentStage(std::vector<int>(MAXPLAYERCOUNT))
	, _currentLevel(std::vector<int>(MAXPLAYERCOUNT))
	, _garbageLineCount(std::vector<int>(MAXPLAYERCOUNT))
{
	for (int i = 0; i < MAXPLAYERCOUNT; i++)
	{
		_isGameOver[i] = false;
		_fallCount[i] = 0;
		_playTime[i] = 0.0f;
		_playerScore[i] = 0;
		_comboTextPos[i] = COMBOTEXTPOSITION;
	}

	std::wstring path = L"../Resources/Sound/";
	_soundComponent = AddComponent<flt::SoundComponent>(true);
	_soundComponent->AddSound(path + L"BGM01hero.wav");
	_soundComponent->AddSound(path + L"BGM07battle.wav");
	_soundComponent->AddSound(path + L"BGM14chase.wav");
	_soundComponent->AddSound(path + L"BGM17battle2.wav");

	int index = 0;
	_soundIndex = std::unordered_map<std::string, int>();
	_soundIndex["BGM1"] = index++;
	_soundIndex["BGM2"] = index++;
	_soundIndex["BGM3"] = index++;
	_soundIndex["BGM4"] = index++;

	std::wstring fontPath = L"../Resources/Fonts/LineSeedSansKR_KoreanCompatible_40.spritefont";
	std::wstring smallFontPath = L"../Resources/Fonts/LineSeedSansKR_KoreanCompatible_25.spritefont";
	flt::Vector4f fontColor = { 1.0f,1.0f,1.0f,1.0f };

	// Create comboTextPool
	for (int i = 0; i < COMBOTEXTPOOLCOUNT; ++i)
	{
		TextObject* comboText = flt::CreateGameObject<TextObject>(false);
		comboText->SetFont(smallFontPath);
		comboText->SetText(L"default text");
		comboText->SetTextColor(fontColor);
		_comboTextPool.push_back(comboText);
	}


	std::wstring missilePath = L"../Resources/Sprites/Missile.png";

	for (int i = 0; i < COMBOTEXTPOOLCOUNT; ++i)
	{
		SpriteObject* missileObj = flt::CreateGameObject<SpriteObject>(false);
		missileObj->SetSprite(missilePath);
		missileObj->SetZOrder(0.8f);
		_missilePool.push_back(missileObj);
	}

	ReadStageFile();
}

GameManager::~GameManager()
{

}

void GameManager::Update(float deltaSecond)
{
	for (auto& comboText : _liveComboTexts)
	{
		auto originOffset = comboText->GetOffsetPosition();
		comboText->SetOffsetPosition({ originOffset.x, originOffset.y - COMBOTEXTSPEED * deltaSecond });
	}

	if (_players.size() < MAXPLAYERCOUNT)
	{
		flt::KeyData keyData = flt::GetKeyDown(flt::KeyCode::key1);
		if (keyData)
		{
			ResetGame();
			SetStage(1);
		}

		keyData = flt::GetKeyDown(flt::KeyCode::key2);
		if (keyData)
		{
			ResetGame();
			SetStage(2);
		}

		keyData = flt::GetKeyDown(flt::KeyCode::key3);
		if (keyData)
		{
			ResetGame();
			SetStage(3);
		}

		keyData = flt::GetKeyDown(flt::KeyCode::key4);
		if (keyData)
		{
			ResetGame();
			SetStage(4);
		}

		keyData = flt::GetKeyDown(flt::KeyCode::key5);
		if (keyData)
		{
			ResetGame();
			SetStage(5);
		}

		keyData = flt::GetKeyDown(flt::KeyCode::key6);
		if (keyData)
		{
			ResetGame();
			SetStage(6);
		}

		keyData = flt::GetKeyDown(flt::KeyCode::key7);
		if (keyData)
		{
			ResetGame();
			SetStage(7);
		}

		keyData = flt::GetKeyDown(flt::KeyCode::key8);
		if (keyData)
		{
			ResetGame();
			SetStage(8);
		}

		keyData = flt::GetKeyDown(flt::KeyCode::key9);
		if (keyData)
		{
			ResetGame();
			SetStage(9);
		}
	}
}

void GameManager::PostUpdate(float deltaSecond)
{
	for (auto& liveText : _liveComboTexts)
	{
		auto offsetPos = liveText->GetOffsetPosition();

		if (offsetPos.y <= 0.85f && !liveText->IsEnable())
		{
			liveText->Enable();		// 특정 높이가 되면 Enable
		}
	}

	if (!_liveComboTexts.empty())
	{
		auto& comboText = _liveComboTexts.front();
		auto offsetPos = comboText->GetOffsetPosition();

		if (offsetPos.y <= 0.15f)
		{
			comboText->Disable();		// 특정 높이가 되면 Disable 및 Pool로 반환
			_comboTextPool.push_back(comboText);
			_liveComboTexts.pop_front();
		}
	}

	for (int i = 0; i < _players.size(); i++)
	{
		if (_isGameOver[i])
		{
			continue;
		}

		AddPlayTime(i, deltaSecond);
	}
}

void GameManager::CreateUI(int index)
{
	_fallCount[index] = 0;

	std::wstring stageCounterSlotPath = L"../Resources/Sprites/StageCounterSlot.png";
	std::wstring levelCounterSlotPath = L"../Resources/Sprites/LevelCounterSlot.png";
	std::wstring levelCounterBluePath = L"../Resources/Sprites/LevelCounterBlue.png";
	std::wstring bigFontPath = L"../Resources/Fonts/LineSeedSansKR_KoreanCompatible_55.spritefont";
	std::wstring fontPath = L"../Resources/Fonts/LineSeedSansKR_KoreanCompatible_40.spritefont";
	std::wstring smallFontPath = L"../Resources/Fonts/LineSeedSansKR_KoreanCompatible_25.spritefont";

	TextObject* garbageLineText = flt::CreateGameObject<TextObject>(true);
	garbageLineText->SetOffsetPosition(GARBAGEPANEL_OFFSETPOS);
	garbageLineText->SetText(L"0");
	garbageLineText->SetFont(bigFontPath);
	garbageLineText->SetTextColor(whiteColor);
	garbageLineText->SetTextAlignment(eTextAlignment::LEFT);
	_garbageLineText.push_back(garbageLineText);

	SpriteObject* stageInfoPanel = flt::CreateGameObject<SpriteObject>(true);
	stageInfoPanel->SetOffsetPosition(STAGEINFOPANEL_OFFSETPOS);
	_stageInfoPanel.push_back(stageInfoPanel);

	SpriteObject* stageCounterSlot = flt::CreateGameObject<SpriteObject>(true);
	stageCounterSlot->tr.SetParent(&stageInfoPanel->tr);
	stageCounterSlot->SetSprite(stageCounterSlotPath);
	stageCounterSlot->SetZOrder(STAGESLOTZORDER);
	stageCounterSlot->SetPosition({ 0.0f,0.0f });

	TextObject* stageCounterText = flt::CreateGameObject<TextObject>(true);
	stageCounterText->tr.SetParent(&stageInfoPanel->tr);
	stageCounterText->SetFont(fontPath);
	stageCounterText->SetTextColor(whiteColor);
	stageCounterText->SetText(L"0");
	stageCounterText->SetPosition({ -24.0f,-20.0f });
	_stageCountText.push_back(stageCounterText);

	TextObject* playerScoreText = flt::CreateGameObject<TextObject>(true);
	playerScoreText->tr.SetParent(&stageInfoPanel->tr);
	playerScoreText->SetFont(smallFontPath);
	playerScoreText->SetTextColor(whiteColor);
	playerScoreText->SetText(L"0");
	playerScoreText->SetPosition({ 36.0f, 6.0f });
	_playerScoreText.push_back(playerScoreText);

	_levelCountSlot.emplace_back();
	_levelCountBlue.emplace_back();

	for (int i = 0; i < 4; i++)
	{
		SpriteObject* levelCounterSlot = flt::CreateGameObject<SpriteObject>(true);
		levelCounterSlot->tr.SetParent(&stageInfoPanel->tr);
		levelCounterSlot->SetSprite(levelCounterSlotPath);
		levelCounterSlot->SetZOrder(LEVELSLOTZORDER);
		levelCounterSlot->SetPosition({ 62.0f + 50.0f * i, -18.0f });

		SpriteObject* levelCounterBlue = flt::CreateGameObject<SpriteObject>(false);
		levelCounterBlue->tr.SetParent(&stageInfoPanel->tr);
		levelCounterBlue->SetSprite(levelCounterBluePath);
		levelCounterBlue->SetZOrder(LEVELVALUEZORDER);
		levelCounterBlue->SetPosition({ 62.0f + 50.0f * i, -18.0f });

		_levelCountSlot[index].push_back(levelCounterSlot);
		_levelCountBlue[index].push_back(levelCounterBlue);
	}

	// 미리 6개까지 만들어두고 Disable하면서 사용한다.
	SpriteObject* fallCountPanel = flt::CreateGameObject<SpriteObject>(true);
	fallCountPanel->SetOffsetPosition(HPPANEL_OFFSETPOS);
	_fallCountPanel.push_back(fallCountPanel);

	_fallCountSlot.emplace_back();
	_fallCountRed.emplace_back();

	for (int i = 0; i < MAXFALLCOUNT; i++)
	{
		SpriteObject* fallCountSlot = flt::CreateGameObject<SpriteObject>(false);
		fallCountSlot->tr.SetParent(&fallCountPanel->tr);
		fallCountSlot->SetSprite(counterSlotPath);
		fallCountSlot->SetZOrder(FALLCOUNTSLOTZORDER);
		fallCountSlot->SetPosition({ -FALLCOUNTOFFSET * i,0.0f });

		SpriteObject* hpRed = flt::CreateGameObject<SpriteObject>(false);
		hpRed->tr.SetParent(&fallCountPanel->tr);
		hpRed->SetSprite(counterRedPath);
		hpRed->SetZOrder(FALLCOUNTREDZORDER);
		hpRed->SetPosition({ -FALLCOUNTOFFSET * i,0.0f });

		_fallCountSlot[index].push_back(fallCountSlot);
		_fallCountRed[index].push_back(hpRed);
	}

	TextObject* heightCountText = flt::CreateGameObject<TextObject>(true);
	heightCountText->SetParent(fallCountPanel);
	heightCountText->SetPosition({ 0.0f, -25.0f });
	heightCountText->SetFont(smallFontPath);
	heightCountText->SetText(L"0");
	heightCountText->SetTextColor(whiteColor);
	heightCountText->SetTextAlignment(eTextAlignment::RIGHT);
	_heightCountText.push_back(heightCountText);

	TextObject* gameoverTextPanel = flt::CreateGameObject<TextObject>(false);
	gameoverTextPanel->SetOffsetPosition(GAMEOVERPANEL_OFFSETPOS);
	_gameoverTextPanel.push_back(gameoverTextPanel);

	_gameoverText.emplace_back();

	float gameoverOffset1 = 230.0f;
	float gameoverOffset2 = 170.0f;
	float gameoverOffset3 = 110.0f;
	float gameoverOffset4 = 50.0f;

	TextObject* text_G = flt::CreateGameObject<TextObject>(true);
	text_G->SetParent(gameoverTextPanel);
	text_G->SetPosition({ -gameoverOffset1, 0.0f });
	text_G->SetText(L"G");
	text_G->SetFont(bigFontPath);
	text_G->SetTextColor(whiteColor);
	text_G->SetTextAlignment(eTextAlignment::CENTER);
	_gameoverText[index].push_back(text_G);

	TextObject* text_A = flt::CreateGameObject<TextObject>(true);
	text_A->SetParent(gameoverTextPanel);
	text_A->SetPosition({ -gameoverOffset2, 0.0f });
	text_A->SetText(L"A");
	text_A->SetFont(bigFontPath);
	text_A->SetTextColor(whiteColor);
	text_A->SetTextAlignment(eTextAlignment::CENTER);
	_gameoverText[index].push_back(text_A);

	TextObject* text_M = flt::CreateGameObject<TextObject>(true);
	text_M->SetParent(gameoverTextPanel);
	text_M->SetPosition({ -gameoverOffset3, 0.0f });
	text_M->SetText(L"M");
	text_M->SetFont(bigFontPath);
	text_M->SetTextColor(whiteColor);
	text_M->SetTextAlignment(eTextAlignment::CENTER);
	_gameoverText[index].push_back(text_M);

	TextObject* text_E = flt::CreateGameObject<TextObject>(true);
	text_E->SetParent(gameoverTextPanel);
	text_E->SetPosition({ -gameoverOffset4, 0.0f });
	text_E->SetText(L"E");
	text_E->SetFont(bigFontPath);
	text_E->SetTextColor(whiteColor);
	text_E->SetTextAlignment(eTextAlignment::CENTER);
	_gameoverText[index].push_back(text_E);

	TextObject* text_O = flt::CreateGameObject<TextObject>(true);
	text_O->SetParent(gameoverTextPanel);
	text_O->SetPosition({ gameoverOffset4, 0.0f });
	text_O->SetText(L"O");
	text_O->SetFont(bigFontPath);
	text_O->SetTextColor(whiteColor);
	text_O->SetTextAlignment(eTextAlignment::CENTER);
	_gameoverText[index].push_back(text_O);

	TextObject* text_V = flt::CreateGameObject<TextObject>(true);
	text_V->SetParent(gameoverTextPanel);
	text_V->SetPosition({ gameoverOffset3, 0.0f });
	text_V->SetText(L"V");
	text_V->SetFont(bigFontPath);
	text_V->SetTextColor(whiteColor);
	text_V->SetTextAlignment(eTextAlignment::CENTER);
	_gameoverText[index].push_back(text_V);

	TextObject* text_E2 = flt::CreateGameObject<TextObject>(true);
	text_E2->SetParent(gameoverTextPanel);
	text_E2->SetPosition({ gameoverOffset2, 0.0f });
	text_E2->SetText(L"E");
	text_E2->SetFont(bigFontPath);
	text_E2->SetTextColor(whiteColor);
	text_E2->SetTextAlignment(eTextAlignment::CENTER);
	_gameoverText[index].push_back(text_E2);

	TextObject* text_R = flt::CreateGameObject<TextObject>(true);
	text_R->SetParent(gameoverTextPanel);
	text_R->SetPosition({ gameoverOffset1, 0.0f });
	text_R->SetText(L"R");
	text_R->SetFont(bigFontPath);
	text_R->SetTextColor(whiteColor);
	text_R->SetTextAlignment(eTextAlignment::CENTER);
	_gameoverText[index].push_back(text_R);
}

void GameManager::SetBoardAndPlayer(int index, Board* board, Player* player)
{
	if (index < 0 || index >= MAXPLAYERCOUNT)
	{
		ASSERT(false, "Board 및 Player index 오류");
		return;
	}

	_boards.push_back(board);
	_players.push_back(player);
	player->SetPadIndex(index);
	IncreasePlayerCount();
}

void GameManager::ReduceHP(int index, int damage /*= 1*/)
{
	if (_isGameOver[index])
	{
		return;
	}

	_fallCount[index] += damage;

	if (_fallCount[index] > _fallCountMax[index])
	{
		_fallCount[index] = _fallCount[index] % (_fallCountMax[index] + 1);
		_boards[index]->DestroyRow();
		for (int i = 0; i < _fallCountMax[index]; i++)
		{
			_fallCountRed[index][i]->Disable();
		}
	}

	for (int i = _fallCountMax[index] - 1; i >= _fallCountMax[index] - _fallCount[index]; i--)
	{
		_fallCountRed[index][i]->Enable();
	}
}


void GameManager::OnDestroyCubes(int playerIndex, int count, flt::Vector3f pos /*= flt::Vector3f()*/)
{
	if (playerIndex < 0 || playerIndex >= MAXPLAYERCOUNT)
	{
		return;
	}

	if (count <= 0 || count > 100)
	{
		return;
	}

	/// 점수처리
	// 기본 점수 = 제거한 큐브 개수 * 100
	// 1 ~ 3 개 : 기본 점수
	// 4 ~ 6 개 : 기본 점수 * 2 점
	// 7 ~ 9 개 : 기본 점수 * 4 점
	// 10 개 이상 : 기본 점수 * 8 점
	int multiple = 0;

	if (count <= 3)
	{
		multiple = 1;
	}
	else if (count <= 6)
	{
		multiple = 2;
	}
	else if (count <= 9)
	{
		multiple = 4;
	}
	else
	{
		multiple = 8;
	}

	int scoreGet = count * CUBESCORE * multiple;

	AddScore(playerIndex, scoreGet);
	if (count >= 4)
	{
		PrintComboText(playerIndex, count, scoreGet);
	}

	/// 공격 처리

	if (_players.size() == 1)
	{
		return;
	}

	int targetIndex = playerIndex == 0 ? 1 : 0;

	int damage = 0;

	switch (count)
	{
	case 1:
	case 2:
	case 3:
		break;
	case 4:
	case 5:
		damage = 1;
		break;
	case 6:
	case 7:
		damage = 2;
		break;
	case 8:
		damage = 3;
		break;
	case 9:
		damage = 4;
		break;
	default:
		damage = 6;
		break;
	}

	float delay = 0.1f;

	for (int i = 0; i < damage; ++i)
	{
		auto missile = _missilePool.front();
		_missilePool.pop_front();
		missile->Enable();

		std::random_device rd;
		float randomX = fmodf(rd(), 200.0f) - 100.0f;
		float randomY = fmodf(rd(), 200.0f) - 100.0f;

		auto startPos = _gameoverTextPanel[playerIndex]->GetPosition();
		auto popupPos = startPos + flt::Vector2f(randomX, randomY);
		auto endPos = _garbageLineText[targetIndex]->GetPosition();

		missile->SetPosition(startPos);

		auto scaleTween = flt::MakeScaleTween(&missile->tr);
		scaleTween->from({ 0.0f,0.0f,0.0f,1.0f })
			.to({ 2.0f,2.0f,2.0f,1.0f }).during(0.5f).easing(flt::ease::easeOutBack).preDelay(delay * i).postDelay(0.5f)
			.to({ 0.0f,0.0f,0.0f,1.0f }).during(0.1f).onEnd([this, missile]() {this->ReturnMissile(missile); });

		auto posTween = flt::MakePosTween(&missile->tr);
		posTween->from({ startPos.x, startPos.y, 0.0f, 1.0f })
			.to({ popupPos.x, popupPos.y, 0.0f, 1.0f }).during(0.5f).easing(flt::ease::easeOutExpo).preDelay(delay * i)
			.to({ endPos.x,endPos.y, 0.0f, 1.0f }).during(0.5f).easing(flt::ease::easeOutExpo).onEnd([this, targetIndex, damage]() {this->AddAttackedLineCount(targetIndex, 1); });

		flt::StartTween(scaleTween);
		flt::StartTween(posTween);
	}
}

void GameManager::SetStage(int stageNum)
{
	for (int i = 0; i < _players.size(); i++)
	{
		_currentStage[i] = stageNum;

		StageData data = _stageData[_currentStage[i] - 1];

		if (_boards[i] != nullptr)
		{
			_boards[i]->Resize(data.stageWidth, data.stageHeight);
			_boards[i]->Reset();
			_boards[i]->GenerateLevel(data.level[0].levelLayout, data.waveCount, true);
		}

		if (_players[i] != nullptr)
		{
			_players[i]->SetPositionToRatioPosition(0.5f, 0.75f);
		}

		_stageCountText[i]->SetText(std::to_wstring(stageNum));

		// _fallCountMax[i] = data.stageWidth - 1; // 아래 함수에서 처리 
		ResizeFallCountUI(data.stageWidth - 1);
	}
}

void GameManager::ProgressStage(int playerIndex)
{
	++_currentStage[playerIndex];
	int curStage = _currentStage[playerIndex];

	_isGameOver[playerIndex] = false;
	_fallCount[playerIndex] = 0;
	_currentLevel[playerIndex] = 1;

	for (int j = 0; j < _fallCountMax[playerIndex]; ++j)
	{
		_fallCountRed[playerIndex][j]->Disable();
	}

	for (int j = 0; j < 4; j++)
	{
		_levelCountBlue[playerIndex][j]->Disable();
	}

	_levelCountBlue[playerIndex][0]->Enable();

	for (auto& comboText : _liveComboTexts)
	{
		comboText->Disable();
		_comboTextPool.push_back(comboText);
	}
	_liveComboTexts.clear();

	StageData data = _stageData[curStage - 1];

	if (_boards[playerIndex] != nullptr)
	{
		_boards[playerIndex]->Resize(data.stageWidth, data.stageHeight);
		_boards[playerIndex]->Reset();
		_boards[playerIndex]->GenerateLevel(data.level[0].levelLayout, data.waveCount, true);
	}

	if (_players[playerIndex] != nullptr)
	{
		_players[playerIndex]->SetPositionToRatioPosition(0.5f, 0.75f);
	}

	_stageCountText[playerIndex]->SetText(std::to_wstring(curStage));

	ResizeFallCountUI(data.stageWidth - 1);
}

void GameManager::OnStageStart()
{
	//_soundComponent->Play(_soundIndex["BGM3"]);
}

void GameManager::OnEndLevel(int playerIndex)
{
	if (_players.size() == 1)
	{
		++_currentLevel[playerIndex];
		int curStage = _currentStage[playerIndex];
		if (_currentLevel[playerIndex] > _stageData[curStage - 1].levelCount)
		{
			// TODO : 스테이지 클리어
			//_soundComponent->Stop(_soundIndex["BGM3"]);
			ProgressStage(playerIndex);
			return;
		}

		int levelIndex = _currentLevel[playerIndex] - 1;
		auto currentStage = _stageData[curStage - 1];

		if (_boards[playerIndex] != nullptr)
		{
			_boards[playerIndex]->Reset();
			_boards[playerIndex]->GenerateLevel(currentStage.level[levelIndex].levelLayout, currentStage.waveCount);

			for (int i = 0; i < _currentLevel[playerIndex]; ++i)
			{
				_levelCountBlue[playerIndex][i]->Enable();
			}
		}
	}
	else if (_players.size() == 2)
	{
		// TODO : 웨이브가 끝나면 퍼펙트라고 해줄까? 흠.. 흠.. 흠.. 흠..

	}
}

Player* GameManager::GetPlayer(int index)
{
	if (index < 0 || index >= _players.size())
	{
		return nullptr;
	}

	return _players[index];
}

void GameManager::OnStartPlayerFall(int index)
{
	// TODO : UI 전부 끄고 UI 관련된거 전부 멈춰라.

	_stageInfoPanel[index]->Disable();
	_fallCountPanel[index]->Disable();
	_garbageLineText[index]->Disable();
	_isGameOver[index] = true;
}

void GameManager::OnEndPlayerFall(int index)
{
	_gameoverTextPanel[index]->Enable();

	for (auto& text : _gameoverText[index])
	{
		float startScale = 6.0f;
		float endScale = 1.5f;

		auto tweenScale = flt::MakeScaleTween(&text->tr);
		// 	tweenScale->from({ startScale,startScale,startScale,1.0f})
		// 		.to({ endScale,endScale,endScale,1.0f }).during(1.5f).easing(flt::ease::easeInOut);

		tweenScale->from({ 0.0f,1.0f,1.0f,1.0f })
			.to({ 1.0f,1.0f,1.0f,1.0f }).during(2.0f).easing(flt::ease::linear).postDelay(2.0f)
			.to({ 0.0f,1.0f,1.0f,1.0f }).during(2.0f).easing(flt::ease::linear);

		flt::StartTween(tweenScale);
	}
}

void GameManager::OnCheckMinHeight(int index, int height)
{
	if (_garbageLineCount[index] == 0)
	{
		return;
	}

	int garbageCount = _garbageLineCount[index];

	if (height > garbageCount)
	{
		_boards[index]->DropGarbageLine(garbageCount);
		SetAttackedLineCount(index, 0);
	}
}

void GameManager::OnHeightChange(int index, int height)
{
	ChangeHeightCountText(index, height);
}

void GameManager::ReturnMissile(SpriteObject* missile)
{
	_missilePool.push_back(missile);
	missile->Disable();
}

void GameManager::IncreasePlayerCount()
{
	if (_players.size() >= MAXPLAYERCOUNT)
	{
		std::wstring redAlbedoPath = L"..\\Resources\\Textures\\Rob02Red_AlbedoTransparency.png";
		std::wstring blueAlbedoPath = L"..\\Resources\\Textures\\Rob02Blue_AlbedoTransparency.png";
		_players[0]->SetAlbedoPath(redAlbedoPath);
		_players[1]->SetAlbedoPath(blueAlbedoPath);

		constexpr float offSetDelta = 0.5f;
		// TODO : UI 배치 적절히 나눠야 함

		for (int i = 0; i < _players.size(); i++)
		{
			float offSetBase = offSetDelta * i;

			if (_fallCountPanel[i] != nullptr)
			{
				auto originOffset = _fallCountPanel[i]->GetOffsetPosition();
				_fallCountPanel[i]->SetOffsetPosition({ offSetBase + originOffset.x / MAXPLAYERCOUNT, originOffset.y });
			}

			if (_garbageLineText[i] != nullptr)
			{
				auto originOffset = _garbageLineText[i]->GetOffsetPosition();
				_garbageLineText[i]->SetOffsetPosition({ offSetBase + originOffset.x / MAXPLAYERCOUNT, originOffset.y });
			}

			if (_stageInfoPanel[i] != nullptr)
			{
				auto originOffset = _stageInfoPanel[i]->GetOffsetPosition();
				_stageInfoPanel[i]->SetOffsetPosition({ offSetBase + originOffset.x / MAXPLAYERCOUNT, originOffset.y });
			}

			if (_gameoverTextPanel[i] != nullptr)
			{
				auto originOffset = _gameoverTextPanel[i]->GetOffsetPosition();
				_gameoverTextPanel[i]->SetOffsetPosition({ offSetBase + originOffset.x / MAXPLAYERCOUNT, originOffset.y });
			}

			_comboTextPos[i] = { offSetBase + COMBOTEXTPOSITION.x / MAXPLAYERCOUNT, COMBOTEXTPOSITION.y };
		}
	}
}

void GameManager::AddScore(int index, int score)
{
	_playerScore[index] += score;
	std::wstring scoreText = std::to_wstring(_playerScore[index]);
	_playerScoreText[index]->SetText(scoreText);
}

void GameManager::PrintComboText(int index, int count, int score)
{
	if (_comboTextPool.size() >= 2)
	{
		std::wstring combostr = std::to_wstring(count);
		combostr += L" Combo!";

		auto& comboText = _comboTextPool.front();
		// comboText->Enable();		// Enable은 높이에 따라 PostUpdate에서 처리
		comboText->SetText(combostr);
		comboText->SetOffsetPosition(_comboTextPos[index]);
		_liveComboTexts.push_back(comboText);
		_comboTextPool.pop_front();

		auto& scoreText = _comboTextPool.front();
		// scoreText->Enable();		// Enable은 높이에 따라 PostUpdate에서 처리
		scoreText->SetText(std::to_wstring(score));
		scoreText->SetOffsetPosition(_comboTextPos[index] + flt::Vector2f(0.0f, 0.05f));
		_liveComboTexts.push_back(scoreText);
		_comboTextPool.pop_front();
	}
}

void GameManager::AddPlayTime(int index, float time)
{
	if (_players[index] == nullptr)
	{
		return;
	}

	_playTime[index] += time;
}

void GameManager::ReadStageFile()
{
	std::string str_buf;
	std::fstream fs;

	fs.open("../Resources/StageData/Stage Data.csv", std::ios::in);

	if (!fs.eof())
	{
		getline(fs, str_buf);

		for (int i = 0; i < MAXSTAGECOUNT; i++)
		{
			getline(fs, str_buf);

			std::istringstream iss(str_buf);
			std::string token;

			int stageNum;
			int levelCount;
			int waveCount;
			int stageWidth;
			int stageHeight;

			getline(iss, token, ',');
			stageNum = std::stoi(token);

			getline(iss, token, ',');
			levelCount = std::stoi(token);

			getline(iss, token, ',');
			waveCount = std::stoi(token);

			getline(iss, token, ',');
			stageWidth = std::stoi(token);

			getline(iss, token, ',');
			stageHeight = std::stoi(token);

			StageData stageData;
			stageData.stageNum = stageNum;
			stageData.levelCount = levelCount;
			stageData.waveCount = waveCount;
			stageData.stageWidth = stageWidth;
			stageData.stageHeight = stageHeight;

			_stageData.push_back(stageData);
		}
	}
	fs.close();

	for (int stageNum = 1; stageNum <= 9; stageNum++)
	{
		std::string stagePath = "../Resources/StageData/Stage" + std::to_string(stageNum) + ".csv";
		fs.open(stagePath, std::ios::in);

		for (int levelCount = 1; levelCount <= _stageData[stageNum - 1].levelCount; levelCount++)
		{
			getline(fs, str_buf);

			std::istringstream iss(str_buf);
			std::string token;

			int stageNum;
			int levelNum;
			int width;
			int height;

			getline(iss, token, ',');
			stageNum = std::stoi(token);

			getline(iss, token, ',');
			levelNum = std::stoi(token);

			getline(iss, token, ',');
			width = std::stoi(token);

			getline(iss, token, ',');
			height = std::stoi(token);

			Level level;
			level.stageNum = stageNum;
			level.levelNum = levelNum;
			level.width = width;
			level.height = height;

			level.levelLayout.resize(width);
			for (int col = 0; col < width; ++col)
			{
				level.levelLayout[col].resize(height);
			}

			for (int row = 0; row < height; ++row)
			{
				getline(fs, str_buf);
				std::istringstream iss(str_buf);
				std::string token;
				for (int col = 0; col < width; ++col)
				{
					getline(iss, token, ',');
					level.levelLayout[col][row] = std::stoi(token);
				}
			}

			_stageData[stageNum - 1].level.push_back(level);
		}
		fs.close();
	}
}

void GameManager::ResetGame()
{
	for (int i = 0; i < _players.size(); i++)
	{
		_isGameOver[i] = false;
		_fallCount[i] = 0;
		_playTime[i] = 0.0f;
		_playerScore[i] = 0;
		_currentLevel[i] = 1;

		for (int j = 0; j < _fallCountMax[i]; ++j)
		{
			_fallCountRed[i][j]->Disable();
		}

		for (int j = 0; j < 4; j++)
		{
			_levelCountBlue[i][j]->Disable();
		}

		_levelCountBlue[i][0]->Enable();

		for (auto& comboText : _liveComboTexts)
		{
			comboText->Disable();
			_comboTextPool.push_back(comboText);
		}
		_liveComboTexts.clear();
	}
}

void GameManager::SetBattleMode()
{
	StageData data = _stageData[0];

	int width = 4;
	int height = 30;

	for (int i = 0; i < _players.size(); i++)
	{
		if (_boards[i] != nullptr)
		{
			_boards[i]->SetBattleMode();
			_boards[i]->Resize(width, height);
			_boards[i]->Reset();
			_boards[i]->GenerateLevel(data.level[0].levelLayout, 1, true);
		}

		if (_players[i] != nullptr)
		{
			_players[i]->SetPositionToRatioPosition(0.5f, 0.75f);
		}

		_stageCountText[i]->SetText(std::to_wstring(1));

		ResizeFallCountUI(data.stageWidth - 1);
	}
}

void GameManager::ResizeFallCountUI(int nextCount)
{
	if (nextCount > MAXFALLCOUNT)
	{
		nextCount = MAXFALLCOUNT;
	}

	for (int i = 0; i < _players.size(); ++i)
	{
		auto& slots = _fallCountSlot[i];

		if (_fallCountMax[i] < nextCount)
		{
			for (int slotIndex = _fallCountMax[i]; slotIndex < nextCount; ++slotIndex)
			{
				slots[slotIndex]->Enable();
			}
		}
		else if (_fallCountMax[i] > nextCount)
		{
			for (int slotIndex = _fallCountMax[i] - 1; slotIndex >= nextCount; --slotIndex)
			{
				slots[slotIndex]->Disable();
			}
		}

		_fallCountMax[i] = nextCount;
	}
}

void GameManager::AddAttackedLineCount(int index, int count)
{
	_garbageLineCount[index] += count;

	int num = _garbageLineCount[index];
	auto& textObj = _garbageLineText[index];

	textObj->SetText(std::to_wstring(num) + L"");

	if (num > 6)
	{
		textObj->SetTextColor(0.9f, 0.0f, 0.0f);
	}
	else if (num > 5)
	{
		textObj->SetTextColor(1.0f, 0.3f, 0.0f);
	}
	else if (num > 4)
	{
		textObj->SetTextColor(1.0f, 0.8f, 0.0f);
	}
	else if (num > 3)
	{
		textObj->SetTextColor(1.0f, 1.0f, 0.0f);
	}
	else if (num > 2)
	{
		textObj->SetTextColor(1.0f, 1.0f, 0.5f);
	}
	else
	{
		textObj->SetTextColor(1.0f, 1.0f, 1.0f);
	}

	auto tweenScale = flt::MakeScaleTween(&textObj->tr);

	tweenScale->from({ 1.8f,1.8f,1.8f,1.0f })
		.to({ 1.0f,1.0f,1.0f,1.0f }).during(0.3f).easing(flt::ease::easeOutBack);

	flt::StartTween(tweenScale);
}

void GameManager::SetAttackedLineCount(int index, int count)
{
	_garbageLineCount[index] = count;

	int num = _garbageLineCount[index];
	auto& textObj = _garbageLineText[index];

	textObj->SetText(std::to_wstring(num) + L"");

	if (num > 6)
	{
		textObj->SetTextColor(0.9f, 0.0f, 0.0f);
	}
	else if (num > 5)
	{
		textObj->SetTextColor(1.0f, 0.3f, 0.0f);
	}
	else if (num > 4)
	{
		textObj->SetTextColor(1.0f, 0.8f, 0.0f);
	}
	else if (num > 3)
	{
		textObj->SetTextColor(1.0f, 1.0f, 0.0f);
	}
	else if (num > 2)
	{
		textObj->SetTextColor(1.0f, 1.0f, 0.5f);
	}
	else
	{
		textObj->SetTextColor(1.0f, 1.0f, 1.0f);
	}

	auto tweenScale = flt::MakeScaleTween(&textObj->tr);

	tweenScale->from({ 1.8f,1.8f,1.8f,1.0f })
		.to({ 1.0f,1.0f,1.0f,1.0f }).during(0.3f).easing(flt::ease::easeOutBack);

	flt::StartTween(tweenScale);
}

void GameManager::ChangeHeightCountText(int index, int height)
{
	auto& textObj = _heightCountText[index];

	textObj->SetText(std::to_wstring(height) + L" lines");

	auto tweenScale = flt::MakeScaleTween(&textObj->tr);

	tweenScale->from({ 1.8f,1.8f,1.8f,1.0f })
		.to({ 1.0f,1.0f,1.0f,1.0f }).during(0.3f).easing(flt::ease::easeOutBack);

	flt::StartTween(tweenScale);
}
