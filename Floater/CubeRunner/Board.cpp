#include "Board.h"
#include "../FloaterGameEngine/include/Input.h"

#include "Tile.h"
#include "AdvantageCube.h"
#include "DarkCube.h"
#include "NormalCube.h"
#include "CubeController.h"
#include "GameManager.h"

constexpr int CUBECOUNT = 64;
constexpr float ROLLINGTIME = 1.0f;
constexpr float ROLLINGDELAY = 0.5f;	// 아무것도 하지않고 굴러갈때의 딜레이
constexpr float DETONATEDELAY = 2.0f;	// 폭파 후 딜레이
constexpr int CUBEDAMAGE = 1;
constexpr int DARKCUBEDAMAGE = 1;

Board::Board(GameManager* gameManager, int playerIndex, int width, int height, float offset /*= 4.00f*/) :
	flt::GameObject()
	, _gameManager(gameManager)
	, _playerIndex(playerIndex)
	, _width(width)
	, _height(height)
	, _tileSize(offset)
	, _tileState()
	, _tiles()
	, _cubeControllers()
	, _advantageCubePool()
	, _darkCubePool()
	, _normalCubePool()
	, _isGameOver(false)
	, _isStageRunning(false)
	, _isRolling(false)
	, _isDirty(false)
	, _elapsedTime(0.0f)
	, _rollingTime(ROLLINGTIME)
	, _minePos({ -1,-1 })
	, _advantageMinePosList()
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
	/// 업데이트 순서
	/// 1. 이동이 다 끝나면 타일 상태 업데이트
	/// 2. 타일상태 업데이트하면서 플레이어와 겹치면 기절시키기
	/// 3. 플레이어 위치 업데이트해서 타일상태에 기입\

	// TODO : 시간 측정 말고 실제 CubeController가 회전이 끝난것들을 이벤트로 받자.
	if (_isRolling)
	{
		_elapsedTime += deltaTime;
		if (_elapsedTime >= _rollingTime)		// Rolling 끝
		{
			_isRolling = false;
			_elapsedTime = 0.0f;
			_rollingDelay = ROLLINGDELAY;
			UpdateBoard();
			UpdateDetonate();
			return;
		}
		else
		{
			return;
		}
	}

	if (!_isRolling)
	{
		_rollingDelay -= deltaTime;
		if (_rollingDelay <= 0.0f)
		{
			if (UpdateDetonate())
			{
				_rollingDelay = DETONATEDELAY;
			}
			else if (_isStageRunning)
			{
				_isRolling = true;
				_elapsedTime = 0.0f;
				TickCubesRolling(_rollingTime);
			}
		}
	}


	if (flt::GetKeyDown(flt::KeyCode::r))
	{
		if (!_isStageRunning)
		{
			GenerateRandomStage();
			_isStageRunning = true;
		}
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

	_tileState[tileX][tileY] = (int)state;

	return true;
}

bool Board::AddTileState(float x, float y, TileStateFlag state)
{
	int tileX = 0;
	int tileY = 0;
	ConvertToTileIndex(x, y, tileX, tileY);

	ASSERT(tileX >= 0 && tileX < _width, "Out of Range");
	ASSERT(tileY >= 0 && tileY < _height, "Out of Range");

	_tileState[tileX][tileY] = (int)_tileState[tileX][tileY] | (int)state;

	return true;
}

int Board::QueryTileState(float x, float y)
{
	int tileX = 0;
	int tileY = 0;
	ConvertToTileIndex(x, y, tileX, tileY);

	if (tileX < 0 || tileX >= _width)
	{
		return (int)TileStateFlag::None;
	}
	if (tileY < 0 || tileY >= _height)
	{
		return (int)TileStateFlag::None;
	}

	return _tileState[tileX][tileY];
}

bool Board::GetCenterPosition(float& x, float& y)
{
	float leftX = 0.0f;
	float leftY = 0.0f;
	ConvertToTilePosition(0, 0, leftX, leftY);

	float rightX = 0.0f;
	float rightY = 0.0f;
	ConvertToTilePosition(_width - 1, _height - 1, rightX, rightY);

	x = (leftX + rightX) / 2.0f;
	y = (leftY + rightY) / 2.0f;

	return true;
}

void Board::ConvertToTileIndex(float x, float z, int& outX, int& outZ)
{
	flt::Vector4f pos = this->tr.GetWorldPosition();
	x -= pos.x;
	z -= pos.z;

	x += _tileSize / 2.0f;
	z += _tileSize / 2.0f;

	x = x / _tileSize;
	z = z / _tileSize;

	x += x < 0.0f ? -1.0f : 0.0f;
	z += z < 0.0f ? -1.0f : 0.0f;

	//x < 0.0f ? x -= 0.5f : x += 0.5f;
	//z < 0.0f ? z -= 0.5f : z += 0.5f;

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
	if (_isGameOver)
	{
		return;
	}

	std::cout << _normalCubePool.size() << std::endl;
	std::cout << _darkCubePool.size() << std::endl;
	std::cout << _advantageCubePool.size() << std::endl;

	for (int i = 0; i < _width; ++i)
	{
		for (int j = _height - 1; j > _height - _width - 2; --j)
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
				_tileState[i][j] = _tileState[i][j] | (int)TileStateFlag::NormalCube;
				break;
			case 1:
				if (_darkCubePool.empty())
				{
					ASSERT(false, "DarkCubePool is Empty");
				}
				cube = _darkCubePool.front();
				_darkCubePool.pop_front();
				_tileState[i][j] = _tileState[i][j] | (int)TileStateFlag::DarkCube;
				break;
			case 2:
				if (_advantageCubePool.empty())
				{
					ASSERT(false, "AdvantageCubePool is Empty");
				}
				cube = _advantageCubePool.front();
				_advantageCubePool.pop_front();
				_tileState[i][j] = _tileState[i][j] | (int)TileStateFlag::AdvantageCube;
				break;
			default:
				ASSERT(false, "Invalid Random Value");
				break;
			}

			auto cubeCtr = cube->GetComponent<CubeController>();
			_cubeControllers.push_back(cubeCtr);

			cube->tr.SetPosition({ x, 4.0f, z, 1.0f });
			cube->Enable();
			_tiles[i][j]->_cube = cube;
		}
	}
}

void Board::TickCubesRolling(float rollingTime)
{
	for (auto& cubeCtr : _cubeControllers)
	{
		cubeCtr->StartRolling(rollingTime);
	}
}

void Board::BackToPool(flt::GameObject* obj)
{
	std::string temp = typeid(*obj).name();
	if (temp.compare(typeid(NormalCube).name()) == 0)
	{
		auto normalCube = dynamic_cast<NormalCube*>(obj);
		_normalCubePool.push_back(normalCube);
		_gameManager->ReduceHP(_playerIndex, CUBEDAMAGE);
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
		_gameManager->ReduceHP(_playerIndex, CUBEDAMAGE);
	}
	else
	{
		ASSERT(false, "Invalid Cube Type");
	}

	obj->Disable();
}

void Board::RemoveFromControllerList(CubeController* cubeCtr)
{
	_cubeControllers.remove(cubeCtr);

	if (_cubeControllers.empty())
	{
		_isStageRunning = false;
	}
}

bool Board::SetMine(float x, float z)
{
	if (_isGameOver)
	{
		return false;
	}

	if (_minePos.first != -1 && _minePos.second != -1)
	{
		return false;
	}

	_isDirty = true;
	int tileX = 0;
	int tileZ = 0;
	ConvertToTileIndex(x, z, tileX, tileZ);

	ASSERT(tileX >= 0 && tileX < _width, "Out of Range");
	ASSERT(tileZ >= 0 && tileZ < _height, "Out of Range");

	_tileState[tileX][tileZ] = (int)_tileState[tileX][tileZ] | (int)TileStateFlag::Mine;

	_tiles[tileX][tileZ]->EnableMine();

	_minePos = { tileX, tileZ };
	return true;
}

void Board::DetonateMine()
{
	if (_isGameOver)
	{
		return;
	}

	auto&[x,y] = _minePos;
	_isDirty = true;
	_tiles[x][y]->DisableMine();
	_tiles[x][y]->EnableDetonated();
	_tileState[x][y] = (int)_tileState[x][y] & ~((int)TileStateFlag::Mine);
	_tileState[x][y] = (int)_tileState[x][y] | (int)TileStateFlag::Detonate;
}

void Board::DetonateAdvantageMine()
{
	if (_isGameOver)
	{
		return;
	}

	_isDirty = true;

	while (!_advantageMinePosList.empty())
	{
		auto& [x, y] = _advantageMinePosList.front();
		_tileState[x][y] = (int)_tileState[x][y] & ~((int)TileStateFlag::AdvantageMine);
		_tiles[x][y]->DisableAdvantageMine();

		for (int m = -1; m <= 1; m++)
		{
			for (int n = -1; n <= 1; n++)
			{
				int nextX = x + m;
				int nextY = y + n;
				if (nextX < 0 || nextX >= _width || nextY < 0 || nextY >= _height)
				{
					continue;
				}

				_tileState[nextX][nextY] = _tileState[nextX][nextY] | (int)TileStateFlag::Detonate;
				_tiles[nextX][nextY]->EnableDetonated();
			}
		}

		_advantageMinePosList.pop_front();
	}
}

void Board::SetGameOver()
{
	_isGameOver = true;
}

void Board::Resize(int newWidth, int newHeight)
{
	_tileState.resize(newWidth);

	for (int i = 0; i < newWidth; ++i)
	{
		_tileState[i].resize(newHeight);
		for (int j = 0; j < newHeight; ++j)
		{
			_tileState[i][j] = (int)TileStateFlag::Tile;
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
		for (int j = 0; j < newHeight; ++j)
		{
			_tileState[i][j] = (int)TileStateFlag::Tile;
		}
	}

	_width = newWidth;
	_height = newHeight;
}

void Board::ConvertToTileLocalPosition(int x, int z, float& outX, float& outZ)
{
	outX = (float)x;
	outZ = (float)z;

	outX *= _tileSize;
	outZ *= _tileSize;
}

void Board::UpdateBoard()
{
	// 1. 큐브 이동 처리
	// 2. Player 깔렸는지 확인
	// 3. 폭파 상태 처리

	bool isPlayerCrushed = false;

	for (int i = 0; i < _width; i++)
	{
		for (int j = 0; j < _height; j++)
		{
			if (j + 1 >= _height)
			{
				_tileState[i][j] = (int)_tileState[i][j] & ~CUBE;
				_tiles[i][j]->_cube = nullptr;
				continue;
			}
			else
			{
				// 1. 큐브 이동
				_tileState[i][j] = (int)_tileState[i][j] & ~CUBE;		// 큐브 타입만 제거
				_tileState[i][j] = (int)_tileState[i][j] | ((int)_tileState[i][j + 1] & CUBE);	// 큐브 타입 이동
				_tiles[i][j]->_cube = _tiles[i][j + 1]->_cube;
				_tiles[i][j + 1]->_cube = nullptr;

				// 2. Check Player Crushed
				if ((int)_tileState[i][j] & (int)TileStateFlag::Player)
				{
					if ((int)_tileState[i][j] & CUBE)
					{
						isPlayerCrushed = true;
					}
				}
			}
		}
	}
}

bool Board::UpdateDetonate()
{
	bool result = false;

	for (int i = 0; i < _width; i++)
	{
		for (int j = 0; j < _height; j++)
		{
			// 3. 폭파 상태 처리
			if ((int)_tileState[i][j] & (int)TileStateFlag::Detonate)
			{
				TileStateFlag cubeType = (TileStateFlag)((int)_tileState[i][j] & CUBE);

				switch (cubeType)
				{
				case TileStateFlag::NormalCube:
					// NormalCube 수납				
					BackToPool(_tiles[i][j]->_cube);
					break;
				case TileStateFlag::DarkCube:
					// DarkCube 수납 및 체력 감소
					_gameManager->ReduceHP(_playerIndex, DARKCUBEDAMAGE);
					BackToPool(_tiles[i][j]->_cube);
					break;
				case TileStateFlag::AdvantageCube:
					// AdvantageCube 수납 및 AdvantageMine 설치
					_tileState[i][j] = _tileState[i][j] | (int)TileStateFlag::AdvantageMine;
					_tiles[i][j]->EnableAdvantageMine();
					_advantageMinePosList.push_back({ i,j });
					BackToPool(_tiles[i][j]->_cube);\
					break;
				default:
					result = false;
					break;
				}

				if ((int)cubeType)
				{
					result = true;
				}

				_tileState[i][j] = (int)_tileState[i][j] & ~((int)TileStateFlag::Detonate);
				_tiles[i][j]->DisableDetonated();

				_tileState[i][j] = (int)_tileState[i][j] & ~CUBE;
			}
		}
	}

	for (int i = 0; i < _width; i++)
	{
		for (int j = 0; j < _height; j++)
		{
			if (_tileState[i][j] & (int)TileStateFlag::Mine)
			{
				return result;
			}
		}
	}
	_minePos.first = -1;
	_minePos.second = -1;

	return result;
}
