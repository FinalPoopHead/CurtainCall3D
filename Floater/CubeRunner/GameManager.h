#pragma once
#include <vector>

class Board;

/// <summary>
/// 컴포넌트나 게임오브젝트를 바로 싱글턴으로 만들지 않고
/// 게임매니저가 이 클래스를 사용해 게임을 진행한다.
/// </summary>
class GameManager
{
	// 싱글턴 코드
public:
	static GameManager& Instance()
	{
		static GameManager instance;
		return instance;
	}

protected:
	GameManager() = default;

private:
	GameManager(const GameManager& rhs) = delete;
	GameManager(GameManager&& rhs) = delete;
	GameManager& operator=(const GameManager& rhs) = delete;
	GameManager& operator=(GameManager&& rhs) = delete;

	////////////////////////////////////////////

public:
	void Initialize();
	void Finalize();
	void Update(float deltaSecond);

	void SetBoard(Board* board) { _board = board; }

private:
	Board* _board;
};
