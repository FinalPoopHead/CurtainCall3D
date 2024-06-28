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
constexpr int COMBOTEXTCOUNT = 20;
constexpr float COMBOTEXTSPEED = 0.2f;
constexpr flt::Vector2f COMBOTEXTPOSITION = { 0.05f,0.85f };

// UI 관련
constexpr flt::Vector2f HPPANEL_OFFSETPOS = { 0.9f,0.95f };
constexpr flt::Vector2f TIMEPANEL_OFFSETPOS = { 0.8f,0.05f };
constexpr flt::Vector2f SCOREPANEL_OFFSETPOS = { 0.05f,0.05f };

constexpr float HPOFFSETSIZE = 80.0f;
constexpr flt::Vector2f HPUISIZE = { 64.0f,20.0f };

constexpr float HPSlotZorder = 0.2f;
constexpr float HPValueZorder = 0.3f;

constexpr flt::Vector4f TimeTextColor = { 1.0f,1.0f,1.0f,1.0f };
constexpr flt::Vector4f ScoreTextColor = { 1.0f,1.0f,1.0f,1.0f };

GameManager::GameManager() :
	_players(std::vector<Player*>(MAXPLAYERCOUNT))
	, _boards(std::vector<Board*>(MAXPLAYERCOUNT))
	, _fallCountPanel(std::vector<SpriteObject*>(MAXPLAYERCOUNT))
	, _fallCountSlot(std::vector<std::vector<SpriteObject*>>(MAXPLAYERCOUNT))
	, _fallCountRed(std::vector<std::vector<SpriteObject*>>(MAXPLAYERCOUNT))
	, _playTimeText(std::vector<TextObject*>(MAXPLAYERCOUNT))
	, _playerScoreText(std::vector<TextObject*>(MAXPLAYERCOUNT))
	, _comboTextPool()
	, _liveComboTexts()
	, _currentPlayerCount(0)
	, _isGameOver(std::vector<bool>(MAXPLAYERCOUNT))
	, _fallCount(std::vector<int>(MAXPLAYERCOUNT))
	, _fallCountMax(std::vector<int>(MAXPLAYERCOUNT))
	, _gameTime(std::vector<float>(MAXPLAYERCOUNT))
	, _playerScore(std::vector<int>(MAXPLAYERCOUNT))
	, _comboTextPos(std::vector<flt::Vector2f>(MAXPLAYERCOUNT))
	, _stageData(std::vector<StageData>(MAXSTAGECOUNT))
	, _currentStage()
	, _currentLevel(std::vector<int>(MAXPLAYERCOUNT))
{
	for (int i = 0; i < MAXPLAYERCOUNT; i++)
	{
		_isGameOver[i] = false;
		_fallCount[i] = 0;
		_gameTime[i] = 0.0f;
		_playerScore[i] = 0;
		_comboTextPos[i] = COMBOTEXTPOSITION;
	}

	std::wstring fontPath = L"../Resources/Fonts/LineSeedSansKR_KoreanCompatible_40.spritefont";
	flt::Vector4f fontColor = { 1.0f,1.0f,1.0f,1.0f };

	// Create comboTextPool
	for (int i = 0; i < COMBOTEXTCOUNT; i++)
	{
		TextObject* comboText = flt::CreateGameObject<TextObject>(false);
		comboText->SetFont(fontPath);
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
		SetStage(1);
	}

	keyData = flt::GetKeyDown(flt::KeyCode::key2);
	if (keyData)
	{
		SetStage(2);
	}

	keyData = flt::GetKeyDown(flt::KeyCode::key3);
	if (keyData)
	{
		SetStage(3);
	}

	keyData = flt::GetKeyDown(flt::KeyCode::key4);
	if (keyData)
	{
		SetStage(4);
	}

	keyData = flt::GetKeyDown(flt::KeyCode::key5);
	if (keyData)
	{
		SetStage(5);
	}

	keyData = flt::GetKeyDown(flt::KeyCode::key6);
	if (keyData)
	{
		SetStage(6);
	}

	keyData = flt::GetKeyDown(flt::KeyCode::key7);
	if (keyData)
	{
		SetStage(7);
	}

	keyData = flt::GetKeyDown(flt::KeyCode::key8);
	if (keyData)
	{
		SetStage(8);
	}

	keyData = flt::GetKeyDown(flt::KeyCode::key9);
	if (keyData)
	{
		SetStage(9);
	}

// 	keyData = flt::GetKeyDown(flt::KeyCode::g);
// 	if (keyData)
// 	{
// 		StageData currentStage = _stageData[_currentStage - 1];
// 
// 		for (int i = 0; i < MAXPLAYERCOUNT; i++)
// 		{
// 			if (_boards[i] != nullptr)
// 			{
// 				_boards[i]->Reset();
// 				_boards[i]->GenerateLevel(currentStage.level[0].levelLayout, currentStage.waveCount);
// 			}
// 		}
// 	}
}

void GameManager::PostUpdate(float deltaSecond)
{
	for (auto& liveText : _liveComboTexts)
	{
		auto offsetPos = liveText->GetOffsetPosition();

		if (offsetPos.y <= 0.85f && !liveText->isEnable())
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

	for (int i = 0; i < MAXPLAYERCOUNT; i++)
	{
		if (_isGameOver[i])
		{
			continue;
		}

		AddPlayTime(i, deltaSecond);
	}
}

void GameManager::CreateUI(int index, int width)
{
	_fallCount[index] = 0;
	_fallCountMax[index] = width - 1;

	std::wstring counterSlotPath = L"../Resources/Sprites/FallCounterSlot.png";
	std::wstring counterRedPath = L"../Resources/Sprites/FallCounterRed.png";
	std::wstring fontPath = L"../Resources/Fonts/LineSeedSansKR_KoreanCompatible_40.spritefont";

	TextObject* playTimeText = flt::CreateGameObject<TextObject>(true);
	playTimeText->SetOffsetPosition(TIMEPANEL_OFFSETPOS);
	playTimeText->SetText(L"00:00");
	playTimeText->SetFont(fontPath);
	playTimeText->SetTextColor(TimeTextColor);

	TextObject* playerScoreText = flt::CreateGameObject<TextObject>(true);
	playerScoreText->SetOffsetPosition(SCOREPANEL_OFFSETPOS);
	playerScoreText->SetText(L"0");
	playerScoreText->SetFont(fontPath);
	playerScoreText->SetTextColor(ScoreTextColor);

	SpriteObject* hpPanel = flt::CreateGameObject<SpriteObject>(true);
	hpPanel->SetOffsetPosition(HPPANEL_OFFSETPOS);

	_playTimeText[index] = playTimeText;
	_playerScoreText[index] = playerScoreText;
	_fallCountPanel[index] = hpPanel;

	for (int i = 0; i < width - 1; i++)
	{
		SpriteObject* hpSlot = flt::CreateGameObject<SpriteObject>(true);
		hpSlot->tr.SetParent(&hpPanel->tr);
		hpSlot->SetSprite(counterSlotPath);
		hpSlot->SetSize(HPUISIZE);
		hpSlot->SetZOrder(HPSlotZorder);
		hpSlot->SetPosition({ -HPOFFSETSIZE * i,0.0f });

		SpriteObject* hpRed = flt::CreateGameObject<SpriteObject>(false);
		hpRed->tr.SetParent(&hpPanel->tr);
		hpRed->SetSprite(counterRedPath);
		hpRed->SetSize(HPUISIZE);
		hpRed->SetZOrder(HPValueZorder);
		hpRed->SetPosition({ -HPOFFSETSIZE * i,0.0f });

		_fallCountSlot[index].push_back(hpSlot);
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

	_boards[index] = board;
	_players[index] = player;
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
	for (int i = 0; i < MAXPLAYERCOUNT; i++)
	{
		_currentLevel[i] = 1;
	}

	StageData data = _stageData[_currentStage - 1];

	for (int i = 0; i < MAXPLAYERCOUNT; i++)
	{
		if (_boards[i] != nullptr)
		{
			_boards[i]->Resize(data.stageWidth, data.stageHeight);
			_boards[i]->Reset();
			_boards[i]->GenerateLevel(data.level[0].levelLayout, data.waveCount);
		}

		if(_players[i] != nullptr)
		{
			_players[i]->SetPositiontoCenter();
		}
	}
}

void GameManager::OnEndLevel(int playerIndex)
{
	++_currentLevel[playerIndex];
	if(_currentLevel[playerIndex] > _stageData[_currentStage - 1].levelCount)
	{
		// TODO : 스테이지 클리어
		//_currentLevel[playerIndex] = 1;
		return;
	}

	int levelIndex = _currentLevel[playerIndex] - 1;
	auto currentStage = _stageData[_currentStage - 1];

	if (_boards[playerIndex] != nullptr)
	{
		_boards[playerIndex]->Reset();
		_boards[playerIndex]->GenerateLevel(currentStage.level[levelIndex].levelLayout, currentStage.waveCount);
	}
}

void GameManager::IncreasePlayerCount()
{
	_currentPlayerCount++;

	if (_currentPlayerCount >= MAXPLAYERCOUNT)
	{
		std::wstring redAlbedoPath = L"..\\Resources\\Textures\\Rob02Red_AlbedoTransparency.png";
		std::wstring blueAlbedoPath = L"..\\Resources\\Textures\\Rob02Blue_AlbedoTransparency.png";
		_players[0]->SetAlbedoPath(redAlbedoPath);
		_players[1]->SetAlbedoPath(blueAlbedoPath);

		constexpr float offSetDelta = 0.5f;
		// TODO : UI 배치 적절히 나눠야 함

		for (int i = 0; i < MAXPLAYERCOUNT; i++)
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

			if (_playerScoreText[i] != nullptr)
			{
				auto originOffset = _playerScoreText[i]->GetOffsetPosition();
				_playerScoreText[i]->SetOffsetPosition({ offSetBase + originOffset.x / MAXPLAYERCOUNT, originOffset.y });
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

	_gameTime[index] += time;

	int gameTime = static_cast<int>(_gameTime[index]);

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

			_stageData[i].stageNum = stageNum;
			_stageData[i].levelCount = levelCount;
			_stageData[i].waveCount = waveCount;
			_stageData[i].stageWidth = stageWidth;
			_stageData[i].stageHeight = stageHeight;
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
