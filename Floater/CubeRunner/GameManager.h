#pragma once
#include <vector>
#include <list>
#include "../FloaterGameEngine/include/EngineMinimal.h"

class Player;
class Board;
class SpriteObject;
class TextObject;

class GameManager : public flt::GameObject
{
public:
	GameManager();
	~GameManager();

protected:
	virtual void Update(float deltaSecond) override;
	virtual void PostUpdate(float deltaSecond) override;

	/// [0] : player1 , [1] : player2
public:
	void SetPlayer(int index, Player* player);
	void SetBoard(int index, Board* board);
	void AddPlayerHPPanel(int index, SpriteObject* hpPanel);
	void AddPlayerHPSlot(int index, SpriteObject* hpSlot);
	void AddPlayerHPValue(int index, SpriteObject* hpValue);
	void AddPlayTimeText(int index, TextObject* playTimeText);
	void AddPlayerScoreText(int index, TextObject* playerScoreText);

	void ReduceHP(int index, int damage);

	void OnCubeDestroy(int playerIndex, int count);	// Cube가 수납될 때 Board 객체가 호출하는 이벤트 함수

private:
	void IncreasePlayerCount();
	void AddScore(int index, int score);
	void PrintComboText(int index, int count, int score);

	/// 컨트롤 하는 게임 오브젝트들
private:
	std::vector<Player*> _players;
	std::vector<Board*> _boards;
	
	// UI들
	// TODO : 체력 UI, 시간 UI, 점수 UI, 게임오버 UI, 콤보 UI 등등 추가 필요
	std::vector<SpriteObject*> _playerHPPanel;
	std::vector<std::vector<SpriteObject*>> _playerHPSlots;
	std::vector<std::vector<SpriteObject*>> _playerHPValues;
	std::vector<TextObject*> _playTimeText;
	std::vector<TextObject*> _playerScoreText;
	std::list<TextObject*> _comboTextPool;
	std::list<TextObject*> _liveComboTexts;


	/// 게임 상태들 저장해두는 멤버 변수들
private:
	int _currentPlayerCount;		// 플레이어 수
	std::vector<bool> _isGameOver;
	std::vector<int> _playerHP;	
	std::vector<int> _playerMaxHP;
	std::vector<float> _gameTime;
	std::vector<int> _playerScore;

	std::vector<flt::Vector2f> _comboTextPos;		// 플레이어 별 콤보 텍스트 위치
};
