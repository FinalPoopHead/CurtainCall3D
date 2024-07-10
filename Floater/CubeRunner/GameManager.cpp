#include <fstream>
#include <sstream>
#include "GameManager.h"
#include "../FloaterGameEngine/include/Input.h"

#include "Player.h"
#include "Board.h"
#include "SpriteObject.h"
#include "TextObject.h"

constexpr int MAXPLAYERCOUNT = 2;
constexpr int MAXSTAGECOUNT = 9;
constexpr int CUBESCORE = 100;
constexpr int COMBOTEXTPOOLCOUNT = 20;
constexpr int MAXFALLCOUNT = 6;			// 최대 fallcount 개수
constexpr float COMBOTEXTSPEED = 0.2f;
constexpr flt::Vector2f COMBOTEXTPOSITION = { 0.05f,0.85f };

// UI 관련
constexpr flt::Vector2f HPPANEL_OFFSETPOS = { 0.9f,0.95f };
constexpr flt::Vector2f TIMEPANEL_OFFSETPOS = { 0.8f,0.05f };
constexpr flt::Vector2f STAGEINFOPANEL_OFFSETPOS = { 0.05f,0.1f };

constexpr float STAGESLOTZORDER = 0.2f;

constexpr float LEVELSLOTZORDER = 0.2f;
constexpr float LEVELVALUEZORDER = 0.3f;

constexpr float FALLCOUNTSLOTZORDER = 0.2f;
constexpr float FALLCOUNTREDZORDER = 0.3f;
constexpr float FALLCOUNTOFFSET = 50.0f;

constexpr flt::Vector4f TextColor = { 1.0f,1.0f,1.0f,1.0f };

std::wstring counterSlotPath = L"../Resources/Sprites/FallCounterSlot.png";
std::wstring counterRedPath = L"../Resources/Sprites/FallCounterRed.png";

GameManager::GameManager() :
	_players()
	, _boards()
	, _stageInfoPanel()
	, _levelCountSlot()
	, _levelCountBlue()
	, _stageCountText()
	, _playerScoreText()
	, _fallCountPanel()
	, _fallCountSlot()
	, _fallCountRed()
	, _playTimeText()
	, _comboTextPool()
	, _liveComboTexts()
	, _isGameOver(std::vector<bool>(MAXPLAYERCOUNT))
	, _fallCount(std::vector<int>(MAXPLAYERCOUNT))
	, _fallCountMax(std::vector<int>(MAXPLAYERCOUNT))
	, _playTime(std::vector<float>(MAXPLAYERCOUNT))
	, _playerScore(std::vector<int>(MAXPLAYERCOUNT))
	, _comboTextPos(std::vector<flt::Vector2f>(MAXPLAYERCOUNT))
	, _stageData()
	, _currentStage()
	, _currentLevel(std::vector<int>(MAXPLAYERCOUNT))
{
	for (int i = 0; i < MAXPLAYERCOUNT; i++)
	{
		_isGameOver[i] = false;
		_fallCount[i] = 0;
		_playTime[i] = 0.0f;
		_playerScore[i] = 0;
		_comboTextPos[i] = COMBOTEXTPOSITION;
	}

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
	std::wstring fontPath = L"../Resources/Fonts/LineSeedSansKR_KoreanCompatible_40.spritefont";
	std::wstring smallFontPath = L"../Resources/Fonts/LineSeedSansKR_KoreanCompatible_25.spritefont";

	TextObject* playTimeText = flt::CreateGameObject<TextObject>(true);
	playTimeText->SetOffsetPosition(TIMEPANEL_OFFSETPOS);
	playTimeText->SetText(L"00:00");
	playTimeText->SetFont(smallFontPath);
	playTimeText->SetTextColor(TextColor);
	_playTimeText.push_back(playTimeText);

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
	stageCounterText->SetTextColor(TextColor);
	stageCounterText->SetText(L"0");
	stageCounterText->SetPosition({ -24.0f,-20.0f });
	_stageCountText.push_back(stageCounterText);

	TextObject* playerScoreText = flt::CreateGameObject<TextObject>(true);
	playerScoreText->tr.SetParent(&stageInfoPanel->tr);
	playerScoreText->SetFont(smallFontPath);
	playerScoreText->SetTextColor(TextColor);
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
		levelCounterSlot->SetPosition({ 62.0f + 50.0f * i, -18.0f});

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


void GameManager::IncreaseScore(int playerIndex, int count)
{
	if (playerIndex < 0 || playerIndex >= MAXPLAYERCOUNT)
	{
		return;
	}

	if (count <= 0 || count > 100)
	{
		return;
	}

	// 기본 점수 = 제거한 큐브 개수 * 100
	// ~ 3 개 : 기본 점수
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
}

void GameManager::AttackAnotherPlayer(int playerIndex)
{
	int targetIndex = playerIndex == 0 ? 1 : 0;

	_boards[targetIndex]->DeferredDestroyRow();
}

void GameManager::SetStage(int stageNum)
{
	_currentStage = stageNum;

	StageData data = _stageData[_currentStage - 1];

	for (int i = 0; i < _players.size(); i++)
	{
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
	++_currentStage;

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

	StageData data = _stageData[_currentStage - 1];

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

	_stageCountText[playerIndex]->SetText(std::to_wstring(_currentStage));

	ResizeFallCountUI(data.stageWidth - 1);
}

void GameManager::OnEndLevel(int playerIndex)
{
	++_currentLevel[playerIndex];
	if (_currentLevel[playerIndex] > _stageData[_currentStage - 1].levelCount)
	{
		// TODO : 스테이지 클리어
		ProgressStage(playerIndex);
		return;
	}

	int levelIndex = _currentLevel[playerIndex] - 1;
	auto currentStage = _stageData[_currentStage - 1];

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

Player* GameManager::GetPlayer(int index)
{
	if(index < 0 || index >= _players.size())
	{
		return nullptr;
	}

	return _players[index];
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

			if (_playTimeText[i] != nullptr)
			{
				auto originOffset = _playTimeText[i]->GetOffsetPosition();
				_playTimeText[i]->SetOffsetPosition({ offSetBase + originOffset.x / MAXPLAYERCOUNT, originOffset.y });
			}

			if (_stageInfoPanel[i] != nullptr)
			{
				auto originOffset = _stageInfoPanel[i]->GetOffsetPosition();
				_stageInfoPanel[i]->SetOffsetPosition({ offSetBase + originOffset.x / MAXPLAYERCOUNT, originOffset.y });
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

	SetPlayTimeText(index, time);
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
			for (int row = 0; row < width; row++)
			{
				level.levelLayout[row].resize(height);
			}

			for (int col = 0; col < height; col++)
			{
				getline(fs, str_buf);
				std::istringstream iss(str_buf);
				std::string token;
				for (int row = 0; row < width; row++)
				{
					getline(iss, token, ',');
					level.levelLayout[row][col] = std::stoi(token);
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

		SetPlayTimeText(i, 0.0f);

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

void GameManager::SetPlayTimeText(int index, float time)
{
	int gameTime = static_cast<int>(_playTime[index]);

	std::wstring timestr;

	if (gameTime / 60 <= 9)
	{
		timestr = L"0" + std::to_wstring(gameTime / 60);
	}
	else
	{
		timestr = std::to_wstring(gameTime / 60);
	}

	timestr += L":";

	if (gameTime % 60 <= 9)
	{
		timestr += L"0" + std::to_wstring(gameTime % 60);
	}
	else
	{
		timestr += std::to_wstring(gameTime % 60);
	}

	_playTimeText[index]->SetText(timestr);
}
