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
	, RISING = 0x0800
};

enum class eBoardState
{
	NONE
	, WAITING
	, CUBEROLLING
	, TILEDESTROYING
	, CUBERISING
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
	void Resize(int width, int height);
	void Reset();
	bool SetTileState(float x, float y, eTileStateFlag state);
	bool AddTileState(float x, float y, eTileStateFlag state);
	int QueryTileState(float x, float y);
	int QueryNextTileState(float x, float y);
	bool GetCenterPosition(float& x, float& y);

	void ConvertToTileIndex(float x, float z, int& outX, int& outZ);
	void ConvertToTilePosition(int x, int z, float& outX, float& outZ);

	void _TEST_GenerateRandomWave();		// 임시로 랜덤 생성 용
	void GenerateLevel(std::vector<std::vector<int>> levelLayout, int waveCount);
	void BackToPool(flt::GameObject* obj);
	void RemoveFromControllerList(CubeController* cubeCtr);
	void SetMine(float x, float z);			// position X,Z에 지뢰를 설치한다.
	void DetonateMine();					// 지뢰를 폭파시킨다.
	void DetonateAdvantageMine();			// 어드밴티지 지뢰를 폭파시킨다.
	void OnEndCubeRoll();					// 큐브 1개가 rolling 끝나면 호출할 함수.
	void OnEndCubeGenerate();						// 큐브 1개가 rising 끝나면 호출할 함수.
	void OnEndTileAdd(Tile* tile);
	void OnStartTileFall(int x, int z);		// x,z index의 타일이 떨어지기 시작함.
	void OnEndTileFall(int x, int z);

	void DestroyRow();
	void DeferredDestroyRow();

	bool IsMineSet();

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
	void SetDelay(float second);
	void AddDelay(float second);
	
	void UpdateBoard();
	bool UpdateDetonate();		// 수납된 큐브가 있으면 true 아니면 false
	void TickCubesRolling(float rollingTime);			// 일괄적으로 굴리기 시작.
	void AddRow();
	void OnEndWave();

	Tile* GetTileFromPool();
	void ReturnTileToPool(Tile* tile);

private:
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
	int _nowRisingCount;
	int _nowFallingTileCount;
	int _damageCount;

	bool _isPerfect;
	int _nowAddTileCount;
	int _nextDestroyRow;

	std::pair<int, int> _minePos;
	std::unordered_map<int,int> _fallingTileCount;	// key는 heightIndex
	std::list<Tile*> _addTiles;
};
