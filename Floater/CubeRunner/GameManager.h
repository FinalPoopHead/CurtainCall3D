#pragma once
#include <vector>
#include <list>
#include "../FloaterGameEngine/include/EngineMinimal.h"

class Player;
class Board;
class SpriteObject;
class TextObject;

struct Level
{
	int stageNum;
	int levelNum;
	int width;
	int height;

	std::vector<std::vector<int>> levelLayout;	// 1: normal, 2: advantage, 3: dark
};

struct StageData
{
	int stageNum;
	int levelCount;
	int waveCount;
	int stageWidth;
	int stageHeight;

	std::vector<Level> level;
};

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
	void CreateUI(int index, int width);
	void SetBoardAndPlayer(int index, Board* board, Player* player);

	void ReduceHP(int index, int damage = 1);

	void IncreaseScore(int playerIndex, int count);	// Cube가 수납될 때 Board 객체가 호출하는 이벤트 함수
	void AttackAnotherPlayer(int playerIndex);
	void SetStage(int stageNum);		// 해당 스테이지로 Hard Set한다 (기존 정보 초기화)
	void ProgressStage();				// 다음 스테이지로 넘어간다	

	void OnEndLevel(int playerIndex);

private:
	void IncreasePlayerCount();
	void AddScore(int index, int score);
	void PrintComboText(int index, int count, int score);
	void AddPlayTime(int index, float time);
	void ReadStageFile();
	void ResetGame();
	void ResizeFallCountUI(int nextCount);
	void SetPlayTimeText(int index, float time);

	/// 컨트롤 하는 게임 오브젝트들
private:
	std::vector<Player*> _players;
	std::vector<Board*> _boards;
	
	// UI들
	// TODO : 체력 UI, 시간 UI, 점수 UI, 게임오버 UI, 콤보 UI 등등 추가 필요
	std::vector<SpriteObject*> _stageInfoPanel;
	std::vector<std::vector<SpriteObject*>> _levelCountSlot;
	std::vector<std::vector<SpriteObject*>> _levelCountBlue;
	std::vector<TextObject*> _stageCountText;
	std::vector<TextObject*> _playerScoreText;
	std::vector<SpriteObject*> _fallCountPanel;
	std::vector<std::vector<SpriteObject*>> _fallCountSlot;
	std::vector<std::vector<SpriteObject*>> _fallCountRed;
	std::vector<TextObject*> _playTimeText;
	std::list<TextObject*> _comboTextPool;
	std::list<TextObject*> _liveComboTexts;


	/// 게임 상태들 저장해두는 멤버 변수들
private:
	std::vector<bool> _isGameOver;
	std::vector<int> _fallCount;	
	std::vector<int> _fallCountMax;
	std::vector<float> _playTime;
	std::vector<int> _playerScore;

	std::vector<flt::Vector2f> _comboTextPos;		// 플레이어 별 콤보 텍스트 위치

private:
	std::vector<StageData> _stageData;
	int _currentStage;
	std::vector<int> _currentLevel;
};
