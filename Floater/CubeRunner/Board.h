#pragma once
#include <list>
#include "../FloaterGameEngine/include/EngineMinimal.h"


class Tile;
class CubeController;
class AdvantageCube;
class DarkCube;
class NormalCube;

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
	CubeLeaving		= 0x1000,
	CubeArriving	= 0x2000
};

class Board : public flt::GameObject
{
public:
	Board(int width, int height, float offset = 4.00f);
	virtual ~Board();

	void OnCreate() override;
	void OnDestroy() override;
	void PreUpdate(float deltaTime) override;

public:
	void Resize(int width, int height);
	bool SetTileState(float x, float y, TileStateFlag state);
	bool AddTileState(float x, float y, TileStateFlag state);
	TileStateFlag QueryTileState(float x, float y);

	void ConvertToTileIndex(float x, float z, int& outX, int& outZ);
	void ConvertToTilePosition(int x, int z, float& outX, float& outZ);

	void GenerateRandomStage();
	void RollCubes(float RollingTime);

private:
	//void ConvertToTileIndex(float x, float z, int& outX, int& outZ);
	void ConvertToTileLocalPosition(int x, int z, float& outX, float& outZ);

	void UpdateBoard();

private:
	int _width;
	int _height;
	float _offset;

	std::vector<std::vector<TileStateFlag>> _tileState;
	std::vector<std::vector<Tile*>> _tiles;

	std::list<std::pair<const type_info&, CubeController*>> _cubeControllers;				// 현재 보드 위에 굴러가는 큐브들
	std::list<AdvantageCube*> _advantageCubePool;		// 어드밴티지 큐브 풀
	std::list<DarkCube*> _darkCubePool;				// 다크 큐브 풀
	std::list<NormalCube*> _normalCubePool;			// 노말 큐브 풀
};

