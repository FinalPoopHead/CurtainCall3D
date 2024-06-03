#include "Board.h"
#include "Tile.h"

Board::Board(int width, int height)
	: flt::GameObject()
	, _width(width)
	, _height(height)
	, _tileState()
{
	_tileState.resize(_width);
	for (int i = 0; i < _width; ++i)
	{
		_tileState[i].resize(_height);
	}

	_tiles.resize(_width);
	for (int i = 0; i < _width; ++i)
	{
		_tiles[i].reserve(_height);

		for (int j = 0; j < _height; ++j)
		{
			_tiles[i].push_back(flt::CreateGameObject<Tile>(true));
		}
	}
}

void Board::PreUpdate(float deltaTime)
{
	UpdateBoard();
}

bool Board::SetTileState(float x, float y, TileStateFlag state)
{
	ASSERT(state != TileStateFlag::None, "Can`t Set None");

	int tileX = 0;
	int tileY = 0;
	ConvertToTileIndex(x, y, tileX, tileY);

	ASSERT(tileX >= 0 && tileX < _width, "Out of Range");
	ASSERT(tileY >= 0 && tileY < _height, "Out of Range");

	_tileState[tileX][tileY] = state;

	return true;
}

bool Board::AddTileState(float x, float y, TileStateFlag state)
{
	int tileX = 0;
	int tileY = 0;
	ConvertToTileIndex(x, y, tileX, tileY);

	ASSERT(tileX >= 0 && tileX < _width, "Out of Range");
	ASSERT(tileY >= 0 && tileY < _height, "Out of Range");

	_tileState[tileX][tileY] = (TileStateFlag)((int)_tileState[tileX][tileY] | (int)state);

	return true;
}

TileStateFlag Board::QueryTileState(float x, float y)
{
	int tileX = 0;
	int tileY = 0;
	ConvertToTileIndex(x, y, tileX, tileY);

	ASSERT(tileX >= 0 && tileX < _width, "Out of Range");
	ASSERT(tileY >= 0 && tileY < _height, "Out of Range");

	return _tileState[tileX][tileY];
}

void Board::ConvertToTileIndex(float x, float y, int& outX, int& outY)
{

}

void Board::UpdateBoard()
{

}
