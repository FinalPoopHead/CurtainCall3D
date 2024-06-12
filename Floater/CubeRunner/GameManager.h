#pragma once
#include <vector>
#include "../FloaterGameEngine/include/EngineMinimal.h"

class Player;
class Board;

class GameManager : public flt::GameObject
{
public:
	GameManager();
	~GameManager();

protected:
	virtual void PostUpdate(float deltaSecond) override;

public:
	void SetPlayer(int index, Player* player);	// [0] : player1 , [1] : player2
	void SetBoard(int index, Board* board);		// [0] : player1 , [1] : player2

	void ReduceHP(int index, int damage);		// [0] : player1 , [1] : player2

private:
	std::vector<bool> _isGameOver;
	std::vector<Player*> _players;
	std::vector<Board*> _boards;
	std::vector<int> _playerHP;	// [0] : player1 , [1] : player2
};
