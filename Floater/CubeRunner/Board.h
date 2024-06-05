#pragma once
#include "../FloaterGameEngine/include/EngineMinimal.h"


class Tile;

enum class TileStateFlag
{
	None = 0x0000,
	Tile = 0x0001,
	Player = 0x0002,
	Mine = 0x0004,
	cube1 = 0x0010,
	cube2 = 0x0020,
	cube3 = 0x0040,
	cube4 = 0x0080,
	cube5 = 0x0100,
	cube6 = 0x0200,
	cube7 = 0x0400,
	cube8 = 0x0800
};

class Board : public flt::GameObject
{
public:
	Board(int width, int height, float offset = 5.0f);
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
};

