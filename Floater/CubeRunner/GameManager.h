#pragma once
#include <vector>
#include <list>
#include "../FloaterGameEngine/include/EngineMinimal.h"
#include "../FloaterGameEngine/include/MakeTween.h"

class Player;
class Board;
class SpriteObject;
class TextObject;

struct Wave
{
	int width;
	int height;

	std::vector<std::vector<int>> waveLayout;
};

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

struct RankData
{
	int rank;
	std::string name;
	int score;
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
	void CreateUI(int index);
	void SetBoardAndPlayer(int index, Board* board, Player* player);

	void ReduceHP(int index, int damage = 1);

	void OnDestroyCubes(int playerIndex, int count, flt::Vector3f pos = flt::Vector3f());	// Cube가 수납될 때 Board 객체가 호출하는 이벤트 함수 // DarkCube를 섞어서 깨면 어떻게해야되려나..?
	void ResetGame();
	void SetBattleMode();
	void SetStage(int stageNum);			// 해당 스테이지로 Hard Set한다 (기존 정보 초기화) -> 플레이어 별로 해야되나?
	void ProgressStage(int playerIndex);	// 다음 스테이지로 넘어간다	

	void OnStageStart();
	void OnEndLevel(int playerIndex);
	Player* GetPlayer(int index);

	void OnStartPlayerFall(int index);
	void OnEndPlayerFall(int index);
	void OnCheckMinHeight(int index, int height, bool doGenerate);
	void OnHeightChange(int index, int height);
	void SetResultText(int playerIndex, int textIndex, std::wstring key, std::wstring value);
	void SetResultTextColor(int playerIndex, int textIndex, flt::Vector4f color);
	void StartWinLoseTween(int playerIndex, bool isWin);
	void StartResultTween(int playerIndex, int textCount);

	void ReturnMissile(SpriteObject* missile);

	void FadeIn();
	void FadeOut();

private:
	void IncreasePlayerCount();
	void AddScore(int index, int score);
	void PrintComboText(int index, int count, int score);
	void AddPlayTime(float time);
	void ReadStageFile();
	void ReadRankingFile();
	void WriteRankingFile();
	void SortRanking();
	void ResizeFallCountUI(int nextCount);
	void AddAttackedLineCount(int index, int count);
	void SetAttackedLineCount(int index, int count);
	void ChangeHeightCountText(int index, int height);
	void EnableScoreInput();
	bool EnterInput(int index);
	void SetRankText();
	void AddBonusScore(int score);

	/// 사운드 관련
private:
	flt::SoundComponent* _soundComponent;
	std::unordered_map<std::string, int> _soundIndex;

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
	std::vector<TextObject*> _heightCountText;
	std::vector<TextObject*> _bonusText;
	int _bonusScore;
	std::vector<TextObject*> _garbageLineText;
	std::vector<TextObject*> _gameoverTextPanel;
	std::vector<std::vector<TextObject*>> _gameoverText;
	std::vector<TextObject*> _winLoseText;
	std::vector<TextObject*> _resultPanel;
	std::vector<std::vector<TextObject*>> _resultText;
	std::vector<std::vector<TextObject*>> _resultValueText;
	TextObject* _finalScorePanel;
	TextObject* _finalScoreText;
	TextObject* _roundText;
	SpriteObject* _fade;

	std::list<TextObject*> _liveComboTexts;

	std::list<TextObject*> _comboTextPool;
	std::list<SpriteObject*> _missilePool;
	std::vector<flt::Vector2f> _comboTextPos;		// 플레이어 별 콤보 텍스트 위치

	TextObject* _inputPanel;
	SpriteObject* _inputSelector;
	SpriteObject* _inputFieldSprite;
	TextObject* _inputField;
	std::string _inputText;
	int _selectorIndex;

	TextObject* _rankingPanel;
	std::vector<TextObject*> _rankText;
	std::vector<TextObject*> _rankNameText;
	std::vector<TextObject*> _rankScoreText;

	/// 게임 상태들 저장해두는 멤버 변수들
private:
	std::vector<bool> _isGameOver;
	std::vector<int> _fallCount;
	std::vector<int> _fallCountMax;
	float _playTime;
	float _accelTime;
	std::vector<int> _playerScore;
	std::vector<int> _garbageLineCount;
	std::vector<bool> _isBacktoBack;

	/// Stage Data
private:
	std::vector<StageData> _stageData;
	std::unordered_map<int, std::vector<Wave>> _battleWaveData;
	std::vector<int> _currentStage;
	std::vector<int> _currentLevel;

	/// Rank Data
private:
	std::vector<RankData> _rankData;

	/// Tween
private:
	flt::TweenPtr<flt::Vector4f> _roundTextTween;
	flt::TweenPtr<float> _fadeInTween;
	flt::TweenPtr<float> _fadeOutTween;
	std::vector<flt::TweenPtr<flt::Vector4f>> _heightCountTextTween;

	/// 게임 패드 관련 데이터
private:
	float _lastLstickX;
	float _lastLstickY;
};
