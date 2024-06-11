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
	None			= 0x0000,
	Tile			= 0x0001,
	Player			= 0x0002,
	Mine			= 0x0004,
	AdvantageMine	= 0x0008,
	Detonate		= 0x0010,
	NormalCube		= 0x0100,
	DarkCube		= 0x0200,
	AdvantageCube	= 0x0400,
	CubeMoving		= 0x1000
};

#define BLOCKED_TILE ((int)TileStateFlag::NormalCube | (int)TileStateFlag::DarkCube | (int)TileStateFlag::AdvantageCube | (int)TileStateFlag::CubeMoving)
#define CUBE ((int)TileStateFlag::NormalCube | (int)TileStateFlag::DarkCube | (int)TileStateFlag::AdvantageCube)

class Board : public flt::GameObject
{
public:
	Board(GameManager* gameManager, int playerIndex, int width, int height, float offset = 4.00f);
	virtual ~Board();

	void OnCreate() override;
	void OnDestroy() override;
	void PreUpdate(float deltaTime) override;

public:
	void Resize(int width, int height);
	bool SetTileState(float x, float y, TileStateFlag state);
	bool AddTileState(float x, float y, TileStateFlag state);
	int QueryTileState(float x, float y);
	bool GetCenterPosition(float& x, float& y);


	void ConvertToTileIndex(float x, float z, int& outX, int& outZ);
	void ConvertToTilePosition(int x, int z, float& outX, float& outZ);

	void GenerateRandomStage();
	void TickCubesRolling(float RollingTime);			// 일괄적으로 굴리기 시작.
	void BackToPool(flt::GameObject* obj);
	void RemoveFromControllerList(CubeController* cubeCtr);
	bool SetMine(float x, float z);		// position X,Z에 지뢰를 설치한다.
	void DetonateMine();					// 지뢰를 폭파시킨다.
	void DetonateAdvantageMine();			// 어드밴티지 지뢰를 폭파시킨다.

	void SetGameOver();

private:
	//void ConvertToTileIndex(float x, float z, int& outX, int& outZ);
	void ConvertToTileLocalPosition(int x, int z, float& outX, float& outZ);

	void UpdateBoard();
	void UpdateDetonate();

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

	bool _isStageRunning = false;
	bool _isRolling = false;
	bool _justFinishedRolling = false;
	bool _isDirty = false;
	float _elapsedTime;

	std::pair<int,int> _minePos;
	std::list<std::pair<int,int>> _advantageMinePosList;
};
