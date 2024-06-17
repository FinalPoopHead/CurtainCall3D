#include "GameManager.h"
#include "../FloaterGameEngine/include/Input.h"

#include "Player.h"
#include "Board.h"
#include "SpriteObject.h"
#include "TextObject.h"

constexpr int MAXHP = 10000;
constexpr int MAXPLAYERCOUNT = 2;
constexpr int CUBESCORE = 100;
constexpr int COMBOTEXTCOUNT = 20;
constexpr float COMBOTEXTSPEED = 0.2f;

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
	, _gameTime(std::vector<float>(MAXPLAYERCOUNT))
	, _playerScore(std::vector<int>(MAXPLAYERCOUNT))
{
	for (int i = 0; i < MAXPLAYERCOUNT; i++)
	{
		_isGameOver[i] = false;
		_playerHP[i] = 0;
		_gameTime[i] = 0.0f;
		_playerScore[i] = 0;
	}

	std::wstring fontPath = L"../Resources/Fonts/LineSeedSansKR_KoreanCompatible_25.spritefont";
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
	flt::KeyData keyData = flt::GetKeyDown(flt::KeyCode::c);
	if (keyData && !_comboTextPool.empty())
	{
		auto& comboText = _comboTextPool.front();
		comboText->Enable();
		comboText->SetText(L"Combo 10");
		comboText->SetOffsetPosition({ 0.15f,0.85f });
		_liveComboTexts.push_back(comboText);
		_comboTextPool.pop_front();
	}

	for (auto& comboText : _liveComboTexts)
	{
		auto originOffset = comboText->GetOffsetPosition();
		comboText->SetOffsetPosition({ originOffset.x, originOffset.y - COMBOTEXTSPEED * deltaSecond });
	}
}

void GameManager::PostUpdate(float deltaSecond)
{
	if (!_liveComboTexts.empty())
	{
		auto& comboText = _liveComboTexts.front();
		auto offsetPos = comboText->GetOffsetPosition();

		if (offsetPos.y <= 0.15f)
		{
			comboText->Disable();
			_comboTextPool.push_back(comboText);
			_liveComboTexts.pop_front();
		}
	}

	for (int i = 0; i < MAXPLAYERCOUNT; i++)
	{
		_gameTime[i] += deltaSecond;

		if (_isGameOver[i] && _boards[i] && _players[i])
		{
			_players[i]->SetGameOver();
			_boards[i]->SetGameOver();
		}
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

	if (_playerHP[index] <= 0)
	{
		_playerHP[index] = 0;
		_isGameOver[index] = true;
	}
}


void GameManager::OnCubeDestroy(int playerIndex, int count)
{
	// TODO : 파괴된 갯수 대로 콤보 계산
	// TODO : 콤보 UI 출력

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
	if (count <= 3)
	{
		_playerScore[playerIndex] += count * CUBESCORE;
	}
	else if (count <= 6)
	{
		_playerScore[playerIndex] += count * CUBESCORE * 2;
	}
	else if (count <= 9)
	{
		_playerScore[playerIndex] += count * CUBESCORE * 4;
	}
	else
	{
		_playerScore[playerIndex] += count * CUBESCORE * 8;
	}
}

void GameManager::IncreasePlayerCount()
{
	_currentPlayerCount++;

	if (_currentPlayerCount >= MAXPLAYERCOUNT)
	{
		constexpr float offSetDelta = 0.5f;
		// TODO : UI 배치 적절히 나눠야 함
		// TODO : 이거 왜 동작 안하냐.
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
		}
	}
}
