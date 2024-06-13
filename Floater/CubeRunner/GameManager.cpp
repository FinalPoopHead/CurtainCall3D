#include "GameManager.h"
#include "Player.h"
#include "Board.h"

constexpr int MAXHP = 10000;
constexpr int PLAYERCOUNT = 2;
constexpr int CUBESCORE = 100;

GameManager::GameManager() :
	_players(std::vector<Player*>(PLAYERCOUNT))
	, _boards(std::vector<Board*>(PLAYERCOUNT))
	, _isGameOver(std::vector<bool>(PLAYERCOUNT))
	, _playerHP(std::vector<int>(PLAYERCOUNT))
	, _gameTime(std::vector<float>(PLAYERCOUNT))
	, _playerScore(std::vector<int>(PLAYERCOUNT))
{
	for (int i = 0; i < PLAYERCOUNT; i++)
	{
		_isGameOver[i] = false;
		_playerHP[i] = 0;
		_gameTime[i] = 0.0f;
		_playerScore[i] = 0;
	}
}

GameManager::~GameManager()
{

}

void GameManager::PostUpdate(float deltaSecond)
{
	for (int i = 0; i < PLAYERCOUNT; i++)
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
	if (index < 0 || index >= PLAYERCOUNT)
	{
		ASSERT(false, "Player index 오류");
		return;
	}

	_players[index] = player;
	_playerHP[index] = MAXHP;
}

void GameManager::SetBoard(int index, Board* board)
{
	if (index < 0 || index >= PLAYERCOUNT)
	{
		ASSERT(false, "Board index 오류");
		return;
	}

	_boards[index] = board;
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
	
	if (playerIndex < 0 || playerIndex >= PLAYERCOUNT)
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
