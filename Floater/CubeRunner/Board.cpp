﻿#include "Board.h"
#include "../FloaterGameEngine/include/Input.h"

#include "Tile.h"
#include "AdvantageCube.h"
#include "DarkCube.h"
#include "NormalCube.h"
#include "CubeController.h"

constexpr int CUBECOUNT = 64;
constexpr float ROLLINGTIME = 1.0f;

Board::Board(int width, int height, float offset) :
	flt::GameObject(),
	_width(width),
	_height(height),
	_offset(offset),
	_tileState()
{

}

Board::~Board()
{

}

void Board::OnCreate()
{
	int width = _width;
	int height = _height;

	_width = 0;
	_height = 0;

	Resize(width, height);

	// Create CubePool
	for (int i = 0; i < CUBECOUNT; i++)
	{
		NormalCube* normalCube = flt::CreateGameObject<NormalCube>(false);
		normalCube->GetComponent<CubeController>()->SetBoard(this);
		_normalCubePool.push_back(normalCube);

		AdvantageCube* advantageCube = flt::CreateGameObject<AdvantageCube>(false);
		advantageCube->GetComponent<CubeController>()->SetBoard(this);
		_advantageCubePool.push_back(advantageCube);

		DarkCube* darkCube = flt::CreateGameObject<DarkCube>(false);
		darkCube->GetComponent<CubeController>()->SetBoard(this);
		_darkCubePool.push_back(darkCube);
	}
}

void Board::OnDestroy()
{
	Resize(0, 0);
}

void Board::PreUpdate(float deltaTime)
{
	UpdateBoard();

	if (flt::GetKeyDown(flt::KeyCode::r))
	{
		if (!_isGenerated)
		{
			GenerateRandomStage();
			_isGenerated = true;
		}
	}

	if (flt::GetKeyDown(flt::KeyCode::spacebar))
	{
		RollCubes(ROLLINGTIME);
	}
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

	if (tileX < 0 || tileX >= _width)
	{
		return TileStateFlag::None;
	}
	if (tileY < 0 || tileY >= _height)
	{
		return TileStateFlag::None;
	}

	return _tileState[tileX][tileY];
}

void Board::ConvertToTileIndex(float x, float z, int& outX, int& outZ)
{
	flt::Vector4f pos = this->tr.GetWorldPosition();
	x -= pos.x;
	z -= pos.z;

	x = x / _offset;
	z = z / _offset;

	x < 0.0f ? x -= 0.5f : x += 0.5f;
	z < 0.0f ? z -= 0.5f : z += 0.5f;

	outX = (int)x;
	outZ = (int)z;
}

void Board::ConvertToTilePosition(int x, int z, float& outX, float& outZ)
{
	ConvertToTileLocalPosition(x, z, outX, outZ);

	flt::Vector4f pos = this->tr.GetWorldPosition();
	outX += pos.x;
	outZ += pos.z;
}

void Board::GenerateRandomStage()
{
	std::cout << _normalCubePool.size() << std::endl;
	std::cout << _darkCubePool.size() << std::endl;
	std::cout << _advantageCubePool.size() << std::endl;

	for (int i = 0; i < _width; ++i)
	{
		for (int j = 0; j < _height; ++j)
		{
			int randValue = rand() % 3;

			float x = _tiles[i][j]->tr.GetWorldPosition().x;
			float z = _tiles[i][j]->tr.GetWorldPosition().z;

			flt::GameObject* cube = nullptr;

			switch (randValue)
			{
			case 0:
				if (_normalCubePool.empty())
				{
					ASSERT(false, "NormalCubePool is Empty");
				}
				cube = _normalCubePool.front();
				_normalCubePool.pop_front();
				_tileState[i][j] = TileStateFlag::NormalCube;
				break;
			case 1:
				if (_darkCubePool.empty())
				{
					ASSERT(false, "DarkCubePool is Empty");
				}
				cube = _darkCubePool.front();
				_darkCubePool.pop_front();
				_tileState[i][j] = TileStateFlag::DarkCube;
				break;
			case 2:
				if (_advantageCubePool.empty())
				{
					ASSERT(false, "AdvantageCubePool is Empty");
				}
				cube = _advantageCubePool.front();
				_advantageCubePool.pop_front();
				_tileState[i][j] = TileStateFlag::AdvantageCube;
				break;
			default:
				ASSERT(false, "Invalid Random Value");
			break;
			}

			auto cubeCtr = cube->GetComponent<CubeController>();
			_cubeControllers.push_back(cubeCtr);

			cube->tr.SetPosition({ x, 4.0f, z, 1.0f });
			cube->Enable();
		}
	}
}

void Board::RollCubes(float RollingTime)
{
	for (auto& cubeCtr : _cubeControllers)
	{
		cubeCtr->StartRolling(RollingTime);
	}
}

void Board::BackToPool(flt::GameObject* obj, CubeController* cubeCtr)
{
	std::string temp = typeid(*obj).name();
	if (temp.compare(typeid(NormalCube).name()) == 0)
	{
		auto normalCube = dynamic_cast<NormalCube*>(obj);
		_normalCubePool.push_back(normalCube);
	}
	else if (temp.compare(typeid(DarkCube).name()) == 0)
	{
		auto darkCube = dynamic_cast<DarkCube*>(obj);
		_darkCubePool.push_back(darkCube);
	}
	else if (temp.compare(typeid(AdvantageCube).name()) == 0)
	{
		auto advantageCube = dynamic_cast<AdvantageCube*>(obj);
		_advantageCubePool.push_back(advantageCube);
	}
	else
	{
		ASSERT(false, "Invalid Cube Type");
	}

	obj->Disable();
	_cubeControllers.remove(cubeCtr);
}

void Board::Resize(int newWidth, int newHeight)
{
	_tileState.resize(newWidth);

	for (int i = 0; i < newWidth; ++i)
	{
		_tileState[i].resize(newHeight);
		for (int j = 0; j < newHeight; ++j)
		{
			_tileState[i][j] = TileStateFlag::Tile;
		}
	}

	// 타일 높이가 변화시 먼저 처리
	if (newHeight < _height)
	{
		for (int i = 0; i < _width; ++i)
		{
			for (int j = newHeight; j < _height; ++j)
			{
				_tiles[i][j]->Destroy();
			}

			_tiles[i].resize(newHeight);
		}
	}
	else if (newHeight > _height)
	{
		for (int i = 0; i < _width; ++i)
		{
			_tiles[i].resize(newHeight);
			for (int j = _height; j < newHeight; ++j)
			{
				_tiles[i][j] = flt::CreateGameObject<Tile>(true);
				_tiles[i][j]->tr.SetParent(&this->tr);

				float x = 0.0f;
				float z = 0.0f;
				ConvertToTileLocalPosition(i, j, x, z);
				_tiles[i][j]->tr.SetPosition({ x, 0.0f, z });
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
			_tiles[i][j]->tr.SetParent(&this->tr);

			float x = 0.0f;
			float z = 0.0f;
			ConvertToTileLocalPosition(i, j, x, z);
			_tiles[i][j]->tr.SetPosition({ x, 0.0f, z });
		}
	}

	for (int i = 0; i < newWidth; ++i)
	{
		for(int j=0;j<newHeight;++j)
		{
			_tileState[i][j] = TileStateFlag::Tile;
		}
	}

	_width = newWidth;
	_height = newHeight;
}

void Board::ConvertToTileLocalPosition(int x, int z, float& outX, float& outZ)
{
	outX = (float)x;
	outZ = (float)z;

	outX *= _offset;
	outZ *= _offset;
}

void Board::UpdateBoard()
{

}
