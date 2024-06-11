#include "GameManager.h"
#include "Player.h"
#include "Board.h"

constexpr int MAXHP = 10;
constexpr int PLAYERCOUNT = 2;

GameManager::GameManager() :
	_isGameOver(std::vector<bool>(PLAYERCOUNT))
	, _players(std::vector<Player*>(PLAYERCOUNT))
	, _boards(std::vector<Board*>(PLAYERCOUNT))
	, _playerHP(std::vector<int>(PLAYERCOUNT))
{
	for (int i = 0; i < PLAYERCOUNT; i++)
	{
		_playerHP[i] = 0;
		_isGameOver[i] = false;
	}
}

GameManager::~GameManager()
{

}

void GameManager::PostUpdate(float deltaSecond)
{
	for (int i = 0; i < PLAYERCOUNT; i++)
	{
		if (_isGameOver[i] && _boards[i] && _players[i])
		{
			_boards[i]->SetGameOver();
			_players[i]->SetGameOver();
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
