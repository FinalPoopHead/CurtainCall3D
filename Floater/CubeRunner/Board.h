#pragma once
#pragma once
#include <list>
#include "../FloaterGameEngine/include/EngineMinimal.h"


class Tile;
class CubeController;
class AdvantageCube;
class DarkCube;
class NormalCube;
class GameManager;

enum class TileStateFlag
{
	None = 0x0000
	, Tile = 0x0001
	, Player = 0x0002
	, Mine = 0x0004
	, AdvantageMine = 0x0008
	, Detonate = 0x0010
	, NormalCube = 0x0100
	, DarkCube = 0x0200
	, AdvantageCube = 0x0400
};

#define BLOCKED_TILE ((int)TileStateFlag::NormalCube | (int)TileStateFlag::DarkCube | (int)TileStateFlag::AdvantageCube)
#define CUBE ((int)TileStateFlag::NormalCube | (int)TileStateFlag::DarkCube | (int)TileStateFlag::AdvantageCube)

class Board : public flt::GameObject
{
public:
	Board(GameManager* gameManager, int playerIndex, int width, int height, float offset = 4.00f);
	virtual ~Board();

protected:
	void OnCreate() override;
	void OnDestroy() override;
	void PreUpdate(float deltaTime) override;

public:
	void Resize(int width, int height);
	bool SetTileState(float x, float y, TileStateFlag state);
	bool AddTileState(float x, float y, TileStateFlag state);
	int QueryTileState(float x, float y);
	int QueryNextTileState(float x, float y);
	bool GetCenterPosition(float& x, float& y);

	void ConvertToTileIndex(float x, float z, int& outX, int& outZ);
	void ConvertToTilePosition(int x, int z, float& outX, float& outZ);

	void GenerateRandomWave();
	void BackToPool(flt::GameObject* obj);
	void RemoveFromControllerList(CubeController* cubeCtr);
	void SetMine(float x, float z);			// position X,Z에 지뢰를 설치한다.
	void DetonateMine();					// 지뢰를 폭파시킨다.
	void DetonateAdvantageMine();			// 어드밴티지 지뢰를 폭파시킨다.
	void OnEndRolling();					// 큐브 1개가 rolling 끝나면 호출할 함수.

	bool IsMineSet();

	void SetGameOver();
	void ReduceHPbyCubeFalling();

	void FastForward();
	void EndFastForward();

	const float& GetFFValue() { return _fastForwardValue; }

private:
	//void ConvertToTileIndex(float x, float z, int& outX, int& outZ);
	void ConvertToTileLocalPosition(int x, int z, float& outX, float& outZ);

	void UpdateBoard();
	bool UpdateDetonate();		// 수납된 큐브가 있으면 true 아니면 false
	void TickCubesRolling(float rollingTime);			// 일괄적으로 굴리기 시작.

private:
	GameManager* _gameManager;
	int _playerIndex;
	int _width;
	int _height;
	float _tileSize;

	std::vector<std::vector<int>> _tileState;
	std::vector<std::vector<Tile*>> _tiles;

	std::list<CubeController*> _cubeControllers;	// 현재 보드 위에 굴러가는 큐브들
	std::list<AdvantageCube*> _advantageCubePool;								// 어드밴티지 큐브 풀
	std::list<DarkCube*> _darkCubePool;											// 다크 큐브 풀
	std::list<NormalCube*> _normalCubePool;										// 노말 큐브 풀

	bool _isGameOver = false;
	bool _isGameStart = false;
	bool _isWaveRunning = false;
	bool _isRolling = false;
	float _delayRemain;
	float _fastForwardValue;
	int _rollFinishCount;

	std::pair<int, int> _minePos;
	std::list<std::pair<int, int>> _advantageMinePosList;
};
