#pragma once
#pragma once
#include <list>
#include <unordered_map>
#include "../FloaterGameEngine/include/EngineMinimal.h"


class Tile;
class CubeController;
class AdvantageCube;
class DarkCube;
class NormalCube;
class GameManager;
class Player;

enum class eTileStateFlag
{
	NONE = 0x0000
	, TILE = 0x0001
	, PLAYER = 0x0002
	, MINE = 0x0004
	, ADVANTAGEMINE = 0x0008
	, DETONATE = 0x0010
	, NORMALCUBE = 0x0100
	, DARKCUBE = 0x0200
	, ADVANTAGECUBE = 0x0400
	, GENERATING = 0x0800
};

enum class eBoardState
{
	NONE
	, WAITING
	, CUBEROLLING
	, STAGECHANGING
	, TILEDESTROYING
	, CUBEGENERATING
	, ADDTILE
};

#define BLOCKED_TILE ((int)eTileStateFlag::NORMALCUBE | (int)eTileStateFlag::DARKCUBE | (int)eTileStateFlag::ADVANTAGECUBE)
#define CUBE ((int)eTileStateFlag::NORMALCUBE | (int)eTileStateFlag::DARKCUBE | (int)eTileStateFlag::ADVANTAGECUBE)

class Board : public flt::GameObject
{
public:
	Board(GameManager* gameManager, int playerIndex, int width, int height, float offset = 4.00f);
	virtual ~Board();

protected:
	virtual void OnCreate() override;
	virtual void OnDestroy() override;
	virtual void PreUpdate(float deltaSecond) override;
	virtual void PostUpdate(float deltaSeoncd) override;

public:
	void Resize(int newWidth, int newHeight);
	void Reset();
	bool SetTileState(float x, float y, eTileStateFlag state);
	bool AddTileState(float x, float y, eTileStateFlag state);
	int QueryTileState(float x, float y);
	int QueryNextTileState(float x, float y);
	bool GetCenterPosition(float& x, float& y);
	bool GetRatioPosition(float ratioX, float ratioY, float& outX, float& outY);

	void ConvertToTileIndex(float x, float z, int& outX, int& outZ);
	void ConvertToTilePosition(int x, int z, float& outX, float& outZ);

	void GenerateLevel(std::vector<std::vector<int>> levelLayout, int waveCount, bool isFirst = false);
	void GenerateGarbageLine(int lineCount);
	void ReturnCubeToPool(flt::GameObject* obj);
	void RemoveFromControllerList(CubeController* cubeCtr);
	void SetMine(float x, float z);			// position X,Z에 지뢰를 설치한다.
	void DetonateMine();					// 지뢰를 폭파시킨다.
	void DetonateAdvantageMine();			// 어드밴티지 지뢰를 폭파시킨다.
	void OnEndCubeRoll();					// 큐브 1개가 rolling 끝나면 호출할 함수.
	void OnEndCubeGenerate();						// 큐브 1개가 rising 끝나면 호출할 함수.
	void OnEndTileAdd(Tile* tile);
	void OnStartTileFall(int x, int z);		// x,z index의 타일이 떨어지기 시작함.
	void OnEndTileFall(int x, int z);

	void AddColumn();
	void DestroyRow();

	bool IsMineSet();

	void OnEndPlayerFalling();
	void SetGameOver();
	void AddCubeFallCount();

	void FastForward();
	void EndFastForward();

	const float& GetFFValue() { return _fastForwardValue; }

private:
	//void ConvertToTileIndex(float x, float z, int& outX, int& outZ);
	void ConvertToTileLocalPosition(int x, int z, float& outX, float& outZ);

	void Wait(float deltaSecond);
	void OnWaiting();
	void OnEndWaiting();
	void OnFastForwarding();
	void SetDelay(float second);
	void AddDelay(float second);
	
	void UpdateBoard();
	void SetTileStateWithCubeCtr(CubeController* cubeCtr);
	bool UpdateDetonate();		// 수납된 큐브가 있으면 true 아니면 false
	bool CheckOnlyDarkRemain();		// 다크큐브 제외하고 전부 제거됐는지 체크.
	void TickCubesRolling(float rollingTime);			// 일괄적으로 굴리기 시작.
	void AddRow();
	void OnEndWave();

	void MoveCameraToEnd();

	Tile* GetTileFromPool();
	void ReturnTileToPool(Tile* tile);

private:
	flt::SoundComponent* _soundComponent;
	std::unordered_map<std::string, int> _soundIndex;

	GameManager* _gameManager;
	int _playerIndex;
	int _width;
	int _height;
	float _tileSize;
	eBoardState _boardState;

	std::list<Tile*> _tilePool;

	std::vector<std::vector<int>> _tileStates;
	std::vector<std::vector<Tile*>> _tiles;

	std::list<std::list<CubeController*>> _waveCubeControllers;	// 웨이브별 큐브들
	std::list<CubeController*> _runningCubeControllers;	// 현재 보드 위에 굴러가는 큐브들
	std::list<AdvantageCube*> _advantageCubePool;								// 어드밴티지 큐브 풀
	std::list<DarkCube*> _darkCubePool;											// 다크 큐브 풀
	std::list<NormalCube*> _normalCubePool;										// 노말 큐브 풀

	bool _isGameOver;
	bool _isAttacked;
	float _delayRemain;
	float _fastForwardValue;
	int _nowRollingCount;
	int _nowGeneratingCount;
	int _nowFallingTileCount;
	int _damageCount;

	bool _isPerfect;
	bool _isOnlyDarkRemain;
	bool _isCameraWorking;
	bool _isFirst;
	int _nowAddTileCount;
	int _nextDestroyRow;

	std::pair<int, int> _minePos;
	std::unordered_map<int,int> _fallingTileCount;	// key는 heightIndex
	std::list<Tile*> _addTiles;

	int _testIndex;
	float _testValue[5];
};
