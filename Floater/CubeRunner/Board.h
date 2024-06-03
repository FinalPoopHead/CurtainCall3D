#pragma once
#include "../FloaterGameEngine/include/EngineMinimal.h"


class Tile;

enum class TileStateFlag
{
	None = 0x0000,
	Player = 0x0001,
	Cube = 0x0002,
	Mine = 0x0004
};

class Board : public flt::GameObject
{
public:
	Board(int width, int height);
	virtual ~Board() {};

	void PreUpdate(float deltaTime) override;

	bool SetTileState(float x, float y, TileStateFlag state);
	bool AddTileState(float x, float y, TileStateFlag state);
	TileStateFlag QueryTileState(float x, float y);

private:
	void ConvertToTileIndex(float x, float y, int& outX, int& outY);

	void UpdateBoard();

private:
	int _width;
	int _height;

	std::vector<std::vector<TileStateFlag>> _tileState;
	std::vector<std::vector<Tile*>> _tiles;
};

