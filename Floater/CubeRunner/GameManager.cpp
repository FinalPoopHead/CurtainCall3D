#include "GameManager.h"
#include "../FloaterGameEngine/include/Input.h"

#include "Player.h"
#include "Board.h"
#include "SpriteObject.h"
#include "TextObject.h"

constexpr int MAXHP = 3;		// TODO : 현재 UI 개수에 맞게 3개로만 해뒀음.
constexpr int MAXPLAYERCOUNT = 2;
constexpr int CUBESCORE = 100;
constexpr int COMBOTEXTCOUNT = 20;
constexpr float COMBOTEXTSPEED = 0.2f;
constexpr flt::Vector2f COMBOTEXTPOSITION = { 0.05f,0.85f };

GameManager::GameManager() :
	_players(std::vector<Player*>(MAXPLAYERCOUNT))
	, _boards(std::vector<Board*>(MAXPLAYERCOUNT))
	, _playerHPPanel(std::vector<SpriteObject*>(MAXPLAYERCOUNT))
	, _playerHPSlots(std::vector<std::vector<SpriteObject*>>(MAXPLAYERCOUNT))
	, _playerHPValues(std::vector<std::vector<SpriteObject*>>(MAXPLAYERCOUNT))
	, _playTimeText(std::vector<TextObject*>(MAXPLAYERCOUNT))
	, _playerScoreText(std::vector<TextObject*>(MAXPLAYERCOUNT))
	, _comboTextPool()
	, _liveComboTexts()
	, _currentPlayerCount(0)
	, _isGameOver(std::vector<bool>(MAXPLAYERCOUNT))
	, _playerHP(std::vector<int>(MAXPLAYERCOUNT))
	, _playerMaxHP(std::vector<int>(MAXPLAYERCOUNT))
	, _gameTime(std::vector<float>(MAXPLAYERCOUNT))
	, _playerScore(std::vector<int>(MAXPLAYERCOUNT))
	, _comboTextPos(std::vector<flt::Vector2f>(MAXPLAYERCOUNT))
{
	for (int i = 0; i < MAXPLAYERCOUNT; i++)
	{
		_isGameOver[i] = false;
		_playerHP[i] = 0;
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

void GameManager::SetPlayer(int index, Player* player)
{
	if (index < 0 || index >= MAXPLAYERCOUNT)
	{
		ASSERT(false, "Player index 오류");
		return;
	}

	IncreasePlayerCount();
	_players[index] = player;
	_playerHP[index] = MAXHP;
	_playerMaxHP[index] = MAXHP;
}

void GameManager::SetBoard(int index, Board* board)
{
	if (index < 0 || index >= MAXPLAYERCOUNT)
	{
		ASSERT(false, "Board index 오류");
		return;
	}

	_boards[index] = board;
}

void GameManager::AddPlayerHPPanel(int index, SpriteObject* hpPanel)
{
	_playerHPPanel[index] = hpPanel;
}

void GameManager::AddPlayerHPSlot(int index, SpriteObject* hpSlot)
{
	_playerHPSlots[index].push_back(hpSlot);
}

void GameManager::AddPlayerHPValue(int index, SpriteObject* hpValue)
{
	_playerHPValues[index].push_back(hpValue);
}

void GameManager::AddPlayTimeText(int index, TextObject* playTimeText)
{
	if (index < 0 || index >= MAXPLAYERCOUNT)
	{
		ASSERT(false, "playTime index 오류");
		return;
	}

	_playTimeText[index] = playTimeText;
}

void GameManager::AddPlayerScoreText(int index, TextObject* playerScoreText)
{
	if (index < 0 || index >= MAXPLAYERCOUNT)
	{
		ASSERT(false, "playerScoreText index 오류");
		return;
	}

	_playerScoreText[index] = playerScoreText;
}

void GameManager::ReduceHP(int index, int damage)
{
	if (_isGameOver[index])
	{
		return;
	}

	_playerHP[index] -= damage;

	for (int i = 0; i < _playerMaxHP[index] - _playerHP[index]; i++)
	{
		_playerHPValues[index][i]->Disable();
	}

	if (_playerHP[index] <= 0)
	{
		_playerHP[index] = 0;
		_isGameOver[index] = true;
		_players[index]->SetGameOver();
		_boards[index]->SetGameOver();
	}
}


void GameManager::OnCubeDestroy(int playerIndex, int count)
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

void GameManager::IncreasePlayerCount()
{
	_currentPlayerCount++;

	if (_currentPlayerCount >= MAXPLAYERCOUNT)
	{
		constexpr float offSetDelta = 0.5f;
		// TODO : UI 배치 적절히 나눠야 함

		for (int i = 0; i < MAXPLAYERCOUNT; i++)
		{
			float offSetBase = offSetDelta * i;

			if (_playerHPPanel[i] != nullptr)
			{
				auto originOffset = _playerHPPanel[i]->GetOffsetPosition();
				_playerHPPanel[i]->SetOffsetPosition({ offSetBase + originOffset.x / MAXPLAYERCOUNT, originOffset.y });
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
