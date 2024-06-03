#include "Board.h"
#include "Tile.h"

Board::Board(int width, int height)
	: flt::GameObject()
	, _width(0)
	, _height(0)
	, _tileState()
{
	Resize(width, height);
}

Board::~Board()
{
	Resize(0, 0);
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

void Board::Resize(int newWidth, int newHeight)
{
	_tileState.resize(newWidth);
	for (int i = 0; i < newWidth; ++i)
	{
		_tileState[i].resize(newHeight);
	}

	// 타일 높이가 변화시 먼저 처리
	if (newHeight < _height)
	{
		for (auto& tileHeight : _tiles)
		{
			for (int j = newHeight; j < _height; ++j)
			{
				tileHeight[j]->Destroy();
				//tileHeight[j] = nullptr;
			}

			tileHeight.resize(newHeight);
		}
	}
	else if (newHeight > _height)
	{
		for (auto& tileHeight : _tiles)
		{
			tileHeight.resize(newHeight);
			for (int j = _height; j < newHeight; ++j)
			{
				tileHeight[j] = flt::CreateGameObject<Tile>(true);
				//tileHeight[j] = (Tile*)1;
			}
		}
	}

	// 타일 너비가 작아지는경우 타일을 삭제
	if (newWidth < _width)
	{
		for (int i = newWidth; i < _width; ++i)
		{
			for (auto& tile : _tiles[i])
			{
				tile->Destroy();
			}
		}
	}
	_tiles.resize(newWidth);

	// 타일이 늘어나는 경우 타일을 추가
	for (int i = _width; i < newWidth; ++i)
	{
		_tiles[i].resize(newHeight);

		for (int j = 0; j < newHeight; ++j)
		{
			_tiles[i][j] = flt::CreateGameObject<Tile>(true);
		}
	}

	_width = newWidth;
	_height = newHeight;
}

void Board::UpdateBoard()
{

}
