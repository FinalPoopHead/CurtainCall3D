#include "Board.h"
#include "../FloaterGameEngine/include/Input.h"

#include "Tile.h"
#include "AdvantageCube.h"
#include "DarkCube.h"
#include "NormalCube.h"
#include "CubeController.h"
#include "GameManager.h"

constexpr int TILE_COUNT = 512;
constexpr int CUBE_COUNT = 512;

constexpr float GENERATE_TIME = 1.0f;
constexpr float ROLLING_TIME = 1.0f;
constexpr float REMOVE_TIME = 0.5f;
constexpr float ADDTILE_TIME = 2.0f;

constexpr float GENERATE_DELAY = 3.0f;
constexpr float ROLLING_DELAY = 0.5f;	// 아무것도 하지않고 굴러갈때의 딜레이
constexpr float DETONATE_DELAY = 2.0f;	// 폭파 후 딜레이
constexpr float ADDTILE_DELAY = 4.0f;	// 타일 추가 딜레이
constexpr float WAVECLEAR_DELAY = 4.0f;
constexpr float FALLTILE_DELAY = 2.0f;	// 타일 삭제 딜레이

constexpr int CUBE_DAMAGE = 1;
constexpr int DARKCUBE_DAMAGE = 1;
constexpr float SLOWVALUE = 0.5f;
constexpr float FFDEFAULT = 1.0f;
constexpr float FFVALUE = 12.0f;

Board::Board(GameManager* gameManager, int playerIndex, int width, int height, float offset /*= 4.00f*/) :
	flt::GameObject()
	, _gameManager(gameManager)
	, _playerIndex(playerIndex)
	, _width(width)
	, _height(height)
	, _tileSize(offset)
	, _boardState(eBoardState::NONE)
	, _tilePool()
	, _tileStates()
	, _tiles()
	, _waveCubeControllers()
	, _runningCubeControllers()
	, _advantageCubePool()
	, _darkCubePool()
	, _normalCubePool()
	, _isGameOver(true)
	, _isAttacked(false)
	, _delayRemain(ROLLING_DELAY)
	, _fastForwardValue(FFDEFAULT)
	, _nowRollingCount(0)
	, _nowRisingCount(0)
	, _nowFallingTileCount()
	, _damageCount()
	, _isPerfect(true)
	, _nowAddTileCount()
	, _nextDestroyRow()
	, _minePos({ -1,-1 })
	, _fallingTileCount()
	, _addTiles()
{
	// Create TilePool
	for (int i = 0; i < TILE_COUNT; i++)
	{
		Tile* tile = flt::CreateGameObject<Tile>(false, this);
		tile->tr.SetParent(&this->tr);
		_tilePool.push_back(tile);
	}

	int newWidth = _width;
	int newHeight = _height;

	_width = 0;
	_height = 0;

	Resize(newWidth, newHeight);
	_nextDestroyRow = newHeight - 1;

	// Create CubePool
	for (int i = 0; i < CUBE_COUNT; i++)
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

Board::~Board()
{

}

void Board::OnCreate()
{

}

void Board::OnDestroy()
{
	Resize(0, 0);
}

void Board::PreUpdate(float deltaSecond)
{
	// TODO : FastForward를 플레이어 별로 나눠줘야 함
	flt::KeyData keyData = flt::GetKeyDown(flt::KeyCode::l);
	if (keyData)
	{
		FastForward();
	}

	keyData = flt::GetKeyUp(flt::KeyCode::l);
	if (keyData)
	{
		EndFastForward();
	}

	// TODO : 슬로우모드임
	keyData = flt::GetKeyDown(flt::KeyCode::o);
	if (keyData)
	{
		_fastForwardValue = SLOWVALUE;
	}

	keyData = flt::GetKeyUp(flt::KeyCode::o);
	if (keyData)
	{
		EndFastForward();
	}

	// TODO : 치트키. 블랙큐브 빼고 전부 제거
	keyData = flt::GetKeyDown(flt::KeyCode::p);
	if (keyData)
	{
		for (int i = 0; i < _width; i++)
		{
			for (int j = 0; j < _height; j++)
			{
				eTileStateFlag cubeType = (eTileStateFlag)((int)_tileStates[i][j] & CUBE);

				switch (cubeType)
				{
				case eTileStateFlag::NORMALCUBE:
					// NormalCube 수납
					_tiles[i][j]->_cube->GetComponent<CubeController>()->StartRemove(REMOVE_TIME);
					_tiles[i][j]->_cube = nullptr;
					_tileStates[i][j] = (int)_tileStates[i][j] & ~CUBE;
					break;
				case eTileStateFlag::ADVANTAGECUBE:
					// AdvantageCube 수납
					_tiles[i][j]->_cube->GetComponent<CubeController>()->StartRemove(REMOVE_TIME);
					_tiles[i][j]->_cube = nullptr;
					_tileStates[i][j] = (int)_tileStates[i][j] & ~CUBE;
					break;
				default:
					break;
				}
			}
		}
	}

	if (_isGameOver)
	{
		return;
	}

	switch (_boardState)
	{
	case eBoardState::NONE:
		if (!_isGameOver)
		{
			// TODO : 웨이브 클리어 연출 보여주고 다음 웨이브 출발
			if (_runningCubeControllers.empty() && !_waveCubeControllers.empty())
			{
				_runningCubeControllers = _waveCubeControllers.front();
				_waveCubeControllers.pop_front();
			}
			else if (_waveCubeControllers.empty())
			{
				// TODO : 레벨 끝! 다음 레벨로 넘어가야함
				_gameManager->OnEndLevel(_playerIndex);
			}
		}

		if (!_runningCubeControllers.empty())
		{
			TickCubesRolling(ROLLING_TIME);
		}
		break;
	case eBoardState::WAITING:
		Wait(deltaSecond * _fastForwardValue);
		break;
	case eBoardState::CUBEROLLING:
		if (_runningCubeControllers.empty())
		{
			_boardState = eBoardState::WAITING;
		}
		return;
		break;
	case eBoardState::TILEDESTROYING:
	case eBoardState::CUBERISING:
	case eBoardState::ADDTILE:
		return;
		break;
	default:
		break;
	}
}

void Board::PostUpdate(float deltaSeoncd)
{
	if (_damageCount > 0)
	{
		_gameManager->ReduceHP(_playerIndex, _damageCount);
		_damageCount = 0;
	}
}

bool Board::SetTileState(float x, float y, eTileStateFlag state)
{
	ASSERT(state != eTileStateFlag::NONE, "Can`t Set None");

	int tileX = 0;
	int tileY = 0;
	ConvertToTileIndex(x, y, tileX, tileY);

	ASSERT(tileX >= 0 && tileX < _width, "Out of Range");
	ASSERT(tileY >= 0 && tileY < _height, "Out of Range");

	_tileStates[tileX][tileY] = (int)state;

	return true;
}

bool Board::AddTileState(float x, float y, eTileStateFlag state)
{
	int tileX = 0;
	int tileY = 0;
	ConvertToTileIndex(x, y, tileX, tileY);

	ASSERT(tileX >= 0 && tileX < _width, "Out of Range");
	ASSERT(tileY >= 0 && tileY < _height, "Out of Range");

	_tileStates[tileX][tileY] = (int)_tileStates[tileX][tileY] | (int)state;

	return true;
}

int Board::QueryTileState(float x, float y)
{
	int tileX = 0;
	int tileY = 0;
	ConvertToTileIndex(x, y, tileX, tileY);

	if (tileX < 0 || tileX >= _width)
	{
		return (int)eTileStateFlag::NONE;
	}
	if (tileY < 0 || tileY >= _height)
	{
		return (int)eTileStateFlag::NONE;
	}

	return _tileStates[tileX][tileY];
}

int Board::QueryNextTileState(float x, float y)
{
	int tileX = 0;
	int tileY = 0;
	ConvertToTileIndex(x, y, tileX, tileY);

	if (tileX < 0 || tileX >= _width)
	{
		return (int)eTileStateFlag::NONE;
	}
	if (tileY < 0 || tileY >= _height)
	{
		return (int)eTileStateFlag::NONE;
	}

	int nextTileY = tileY - 1;

	if (_boardState == eBoardState::CUBEROLLING)
	{
		if (nextTileY < 0)
		{
			return _tileStates[tileX][tileY] & ~CUBE;
		}

		return _tileStates[tileX][nextTileY] | (_tileStates[tileX][nextTileY] & CUBE);
	}
	else
	{
		return _tileStates[tileX][tileY];
	}
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
	z = -z;
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

void Board::_TEST_GenerateRandomWave()
{
	if (_isGameOver)
	{
		return;
	}

	_isPerfect = true;

	int _TEST_darkCount = 3;
	float startDelay = 0.15f;

	for (int i = 0; i < _width; ++i)
	{
		int delayCount = 0;
		//if (i == _width / 2) continue;		// TEST 한 줄 비우기 위함
		for (int j = 0; j < _width; ++j)
		{
			//if (j == _width / 2) continue;	// TEST 한 줄 비우기 위함
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
				break;
			case 1:
				if (_TEST_darkCount <= 0)
				{
					--j;
					continue;
				}
				_TEST_darkCount--;
				if (_darkCubePool.empty())
				{
					ASSERT(false, "DarkCubePool is Empty");
				}
				cube = _darkCubePool.front();
				_darkCubePool.pop_front();
				break;
			case 2:
				if (_advantageCubePool.empty())
				{
					ASSERT(false, "AdvantageCubePool is Empty");
				}
				cube = _advantageCubePool.front();
				_advantageCubePool.pop_front();
				break;
			default:
				ASSERT(false, "Invalid Random Value");
				break;
			}

			auto cubeCtr = cube->GetComponent<CubeController>();
			_runningCubeControllers.push_back(cubeCtr);
			cubeCtr->StartGenerate(GENERATE_TIME, startDelay * delayCount);

			cube->tr.SetPosition(x, 0.0f, z);
			cube->Enable();
			_tiles[i][j]->_cube = cube;
			_nowRisingCount++;
			delayCount++;
		}

		delayCount = 0;
	}

	_boardState = eBoardState::CUBERISING;
}

void Board::GenerateLevel(std::vector<std::vector<int>> levelLayout, int waveCount)
{
	_isGameOver = false;

	int width = levelLayout.size();
	int height = levelLayout[0].size();

	_isPerfect = true;

	_waveCubeControllers.clear();
	int waveHeight = height / waveCount;
	int heightCount = 0;
	std::list<CubeController*> waveCubes;
	float startDelay = 0.15f;

	for (int j = 0; j < height; ++j)
	{
		heightCount++;

		for (int i = 0; i < width; ++i)
		{
			float x = _tiles[i][j]->tr.GetWorldPosition().x;
			float z = _tiles[i][j]->tr.GetWorldPosition().z;

			flt::GameObject* cube = nullptr;

			switch (levelLayout[i][j])
			{
			case 1:
				if (_normalCubePool.empty())
				{
					ASSERT(false, "NormalCubePool is Empty");
				}
				cube = _normalCubePool.front();
				_normalCubePool.pop_front();
				break;
			case 2:
				if (_advantageCubePool.empty())
				{
					ASSERT(false, "AdvantageCubePool is Empty");
				}
				cube = _advantageCubePool.front();
				_advantageCubePool.pop_front();
				break;
			case 3:
				if (_darkCubePool.empty())
				{
					ASSERT(false, "DarkCubePool is Empty");
				}
				cube = _darkCubePool.front();
				_darkCubePool.pop_front();
				break;
			default:
				ASSERT(false, "Invalid Random Value");
				break;
			}

			auto cubeCtr = cube->GetComponent<CubeController>();
			waveCubes.push_back(cubeCtr);
			cubeCtr->StartGenerate(GENERATE_TIME, startDelay * j);

			cube->tr.SetPosition(x, 0.0f, z);
			cube->Enable();
			_tiles[i][j]->_cube = cube;
			_nowRisingCount++;
		}

		if (heightCount >= waveHeight)
		{
			heightCount = 0;
			_waveCubeControllers.push_front(waveCubes);
			waveCubes.clear();
		}
	}

	_boardState = eBoardState::CUBERISING;

	_runningCubeControllers = _waveCubeControllers.front();
	_waveCubeControllers.pop_front();
}

void Board::TickCubesRolling(float rollingTime)
{
	for (auto& cubeCtr : _runningCubeControllers)
	{
		if (cubeCtr->StartRoll(rollingTime))
		{
			_nowRollingCount++;
		}
	}

	_boardState = eBoardState::CUBEROLLING;
}

void Board::AddRow()
{
	_nowAddTileCount = _width;
	_nextDestroyRow++;
	_boardState = eBoardState::ADDTILE;

	for (int i = 0; i < _width; i++)
	{
		Tile* tile = GetTileFromPool();
		_addTiles.push_back(tile);

		float x = 0.0f;
		float z = 0.0f;
		ConvertToTileLocalPosition(i, _height + 16, x, z);

		tile->tr.SetPosition({ x, 0.0f, z });

		ConvertToTileLocalPosition(i, _height, x, z);
		x += tr.GetWorldPosition().x;
		z += tr.GetWorldPosition().z;

		tile->StartAddRow(ADDTILE_TIME, { x,0.0f,z });
	}
}

void Board::OnEndWave()
{
	if (_isPerfect)
	{
		AddRow();
		// TODO : 상대방 공격하는게 되긴 하는데.. 좀 요상하다 ㅋㅋㅋㅋ
		//			Board 코드도 조금바꿔서 행동에 우선순위를 정해주고 큐에 담아서 처리하는 식으로 해야될듯..
		// _gameManager->AttackAnotherPlayer(_playerIndex);
	}
	else
	{
		SetDelay(WAVECLEAR_DELAY);
	}
}

Tile* Board::GetTileFromPool()
{
	Tile* tile = nullptr;

	if (_tilePool.empty())
	{
		for (int i = 0; i < TILE_COUNT; i++)
		{
			Tile* newTile = flt::CreateGameObject<Tile>(false, this);
			newTile->tr.SetParent(&this->tr);
			_tilePool.push_back(newTile);
		}
	}

	tile = _tilePool.front();
	_tilePool.pop_front();
	tile->Enable();

	return tile;
}

void Board::ReturnTileToPool(Tile* tile)
{
	tile->DisableAdvantageMine();
	tile->DisableDetonated();
	tile->DisableMine();

	auto pos = tile->tr.GetLocalPosition();
	int x = 0;
	int z = 0;
	ConvertToTileIndex(pos.x, pos.z, x, z);
	if (_minePos.first == x && _minePos.second == z)
	{
		_minePos.first = -1;
		_minePos.second = -1;
	}

	_tilePool.push_back(tile);
	tile->Disable();
}

void Board::BackToPool(flt::GameObject* obj)
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
}

void Board::RemoveFromControllerList(CubeController* cubeCtr)
{
	auto removeCount = _runningCubeControllers.remove(cubeCtr);

	if (removeCount == 0)
	{
		return;
	}

	if (_runningCubeControllers.empty())
	{
		OnEndWave();
	}
}

void Board::SetMine(float x, float z)
{
	if (_isGameOver)
	{
		return;
	}

	if (_minePos.first != -1 && _minePos.second != -1)
	{
		return;
	}

	int tileX = 0;
	int tileZ = 0;
	ConvertToTileIndex(x, z, tileX, tileZ);

	ASSERT(tileX >= 0 && tileX < _width, "Out of Range");
	ASSERT(tileZ >= 0 && tileZ < _height, "Out of Range");

	// 해당 위치에 이미 지뢰가 설치되어 있다면 설치하지 않는다.
	if (_tileStates[tileX][tileZ] & (int)eTileStateFlag::ADVANTAGEMINE
		|| _tileStates[tileX][tileZ] & (int)eTileStateFlag::MINE)
	{
		return;
	}

	_tileStates[tileX][tileZ] = (int)_tileStates[tileX][tileZ] | (int)eTileStateFlag::MINE;

	_tiles[tileX][tileZ]->EnableMine();

	_minePos = { tileX, tileZ };
}

void Board::DetonateMine()
{
	if (_isGameOver)
	{
		return;
	}

	auto& [x, y] = _minePos;

	if (x < 0 || x >= _width || y < 0 || y >= _height)
	{
		return;
	}

	_tiles[x][y]->DisableMine();
	_tiles[x][y]->EnableDetonated();
	_tileStates[x][y] = (int)_tileStates[x][y] & ~((int)eTileStateFlag::MINE);
	_tileStates[x][y] = (int)_tileStates[x][y] | (int)eTileStateFlag::DETONATE;
}

void Board::DetonateAdvantageMine()
{
	if (_isGameOver)
	{
		return;
	}

	for (int i = 0; i < _width; i++)
	{
		for (int j = 0; j < _height; j++)
		{
			if (_tileStates[i][j] & (int)eTileStateFlag::ADVANTAGEMINE)
			{
				_tileStates[i][j] = _tileStates[i][j] & ~((int)eTileStateFlag::ADVANTAGEMINE);
				_tiles[i][j]->DisableAdvantageMine();

				for (int m = -1; m <= 1; m++)
				{
					for (int n = -1; n <= 1; n++)
					{
						int nextX = i + m;
						int nextY = j + n;
						if (nextX < 0 || nextX >= _width || nextY < 0 || nextY >= _height)
						{
							continue;
						}

						_tileStates[nextX][nextY] = _tileStates[nextX][nextY] | (int)eTileStateFlag::DETONATE;
						_tiles[nextX][nextY]->EnableDetonated();
					}
				}
			}

		}
	}
}

void Board::OnEndCubeRoll()
{
	_nowRollingCount--;
	if (_nowRollingCount <= 0)
	{
		float delay = ROLLING_DELAY;
		_nowRollingCount = 0;

		UpdateBoard();
		if (UpdateDetonate())
		{
			delay = DETONATE_DELAY;
		}

		SetDelay(delay);
	}
}

void Board::OnEndCubeGenerate()
{
	_nowRisingCount--;
	if (_nowRisingCount <= 0)
	{
		_nowRisingCount = 0;

		for (auto& col : _tileStates)
		{
			for (auto& tileState : col)
			{
				tileState = tileState & ~(int)eTileStateFlag::RISING;
			}
		}

		UpdateBoard();
		SetDelay(GENERATE_DELAY);
	}
}

void Board::OnEndTileAdd(Tile* tile)
{
	_nowAddTileCount--;
	ReturnTileToPool(tile);
	_addTiles.remove(tile);

	if (_nowAddTileCount <= 0)
	{
		_nowAddTileCount = 0;
		Resize(_width, _height + 1);
		SetDelay(ADDTILE_DELAY);
	}
}

void Board::OnStartTileFall(int x, int z)
{
	_tileStates[x][z] = (int)eTileStateFlag::NONE;
	// TODO : Player가 위에 있었으면 게임오버 해야함
}

void Board::OnEndTileFall(int x, int z)
{
	_fallingTileCount[z]--;
	_nowFallingTileCount--;

	if (_fallingTileCount[z] <= 0)
	{
		_fallingTileCount[z] = 0;
		Resize(_width, z);
		SetDelay(FALLTILE_DELAY);
	}
}

void Board::DestroyRow()
{
	_boardState = eBoardState::TILEDESTROYING;

	_fallingTileCount[_nextDestroyRow] = 0;

	float delay = 0.3f;
	float delayDelta = 0.1f;

	for (int i = 0; i < _width; i++)
	{
		if (_minePos.first == i && _minePos.second == _nextDestroyRow)
		{
			_minePos.first = -1;
			_minePos.second = -1;
		}

		_tiles[i][_nextDestroyRow]->StartFall(delay + delayDelta * i, i, _nextDestroyRow);
		_fallingTileCount[_nextDestroyRow]++;
		_nowFallingTileCount++;
	}
	_nextDestroyRow--;
}

void Board::DeferredDestroyRow()
{
	_isAttacked = true;
}

bool Board::IsMineSet()
{
	auto& [x, y] = _minePos;
	return x != -1 && y != -1;
}

void Board::SetGameOver()
{
	_isGameOver = true;
}

void Board::AddCubeFallCount()
{
	_damageCount++;
	_isPerfect = false;
}

void Board::FastForward()
{
	_fastForwardValue = FFVALUE;
}

void Board::EndFastForward()
{
	_fastForwardValue = FFDEFAULT;
}

void Board::Resize(int newWidth, int newHeight)
{
	// 타일 높이가 변화시 먼저 처리
	if (newHeight < _height)
	{
		for (int i = 0; i < _width; ++i)
		{
			for (int j = newHeight; j < _height; ++j)
			{
				ReturnTileToPool(_tiles[i][j]);
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
				_tiles[i][j] = GetTileFromPool();

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
				ReturnTileToPool(tile);
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
			_tiles[i][j] = GetTileFromPool();

			float x = 0.0f;
			float z = 0.0f;
			ConvertToTileLocalPosition(i, j, x, z);
			_tiles[i][j]->tr.SetPosition({ x, 0.0f, z });
		}
	}

	_tileStates.resize(newWidth);

	for (int i = 0; i < newWidth; ++i)
	{
		_tileStates[i].resize(newHeight);
		for (int j = 0; j < newHeight; ++j)
		{
			if (_tileStates[i][j] == (int)eTileStateFlag::NONE)
			{
				_tileStates[i][j] = (int)eTileStateFlag::TILE;
			}
		}
	}

	_width = newWidth;
	_height = newHeight;
}

void Board::Reset()
{
	// TODO : 진행중이던 게임 전부 리셋해야함.
	// 큐브풀로 다 갖고오고 타일풀로 다 갖고오고 진행중이던거 전부 제거하고..
	// 조금 까다롭군

	for (auto& tileCol : _tileStates)
	{
		for (auto& tileState : tileCol)
		{
			tileState = (int)eTileStateFlag::TILE;
		}
	}

	for (auto& tileCol : _tiles)
	{
		for (auto& tile : tileCol)
		{
			tile->DisableAdvantageMine();
			tile->DisableDetonated();
			tile->DisableMine();
		}
	}

	_boardState = eBoardState::NONE;
	_nowRollingCount = 0;
	_nowRisingCount = 0;
	_nowFallingTileCount = 0;
	_nowAddTileCount = 0;
	_nextDestroyRow = _height - 1;

	_minePos.first = -1;
	_minePos.second = -1;

	for (auto& wave : _waveCubeControllers)
	{
		for (auto& cubeCtr : wave)
		{
			BackToPool(cubeCtr->GetGameObject());
		}
	}

	while (!_runningCubeControllers.empty())
	{
		int size = _runningCubeControllers.size();

		auto& cubeCtr = _runningCubeControllers.front();
		BackToPool(cubeCtr->GetGameObject());

		if(size == _runningCubeControllers.size())
		{
			_runningCubeControllers.pop_front();
		}
	}

	//for (auto& cubeCtr : _runningCubeControllers)
	//{
	//	BackToPool(cubeCtr->GetGameObject());
	//}

	for (auto& tile : _addTiles)
	{
		ReturnTileToPool(tile);
	}
	_addTiles.clear();
}

void Board::ConvertToTileLocalPosition(int x, int z, float& outX, float& outZ)
{
	outX = (float)x;
	outZ = (float)z;

	outX *= _tileSize;
	outZ *= -_tileSize;
}

void Board::Wait(float deltaSecond)
{
	_delayRemain -= deltaSecond;
	OnWaiting();

	if (_delayRemain <= 0.0f)
	{
		_delayRemain = 0.0f;
		_boardState = eBoardState::NONE;
		OnEndWaiting();
	}
}

void Board::OnWaiting()
{
	if (_isAttacked)
	{
		_isAttacked = false;
		DestroyRow();
	}
}

void Board::OnEndWaiting()
{
	bool isCubeDetonated = UpdateDetonate();

	if (isCubeDetonated)		// 폭파 된 것이 있다면 delay 연장.
	{
		SetDelay(DETONATE_DELAY);
	}
	else if (!_runningCubeControllers.empty())	// 폭파 된 것이 없다면 구르기 시작.
	{
		TickCubesRolling(ROLLING_TIME);
	}
}

void Board::SetDelay(float second)
{
	_delayRemain = second;
	_boardState = eBoardState::WAITING;
}

void Board::AddDelay(float second)
{
	_delayRemain += second;
	_boardState = eBoardState::WAITING;
}

void Board::UpdateBoard()
{
	for (int i = 0; i < _width; i++)
	{
		for (int j = 0; j < _height; j++)
		{
			_tileStates[i][j] = _tileStates[i][j] & ~CUBE;	// 큐브 타입만 제거
			_tiles[i][j]->_cube = nullptr;
		}
	}

	for (auto& cubeCtr : _runningCubeControllers)
	{
		auto pos = cubeCtr->GetPosition();
		int x = 0;
		int z = 0;
		ConvertToTileIndex(pos.x, pos.z, x, z);

		if (x < 0 || x >= _width || z < 0 || z >= _height)
		{
			continue;
		}

		eTileStateFlag state = eTileStateFlag::NONE;

		switch (cubeCtr->GetCubeType())
		{
		case eCUBETYPE::NORMAL:
			state = eTileStateFlag::NORMALCUBE;
			break;
		case eCUBETYPE::DARK:
			state = eTileStateFlag::DARKCUBE;
			break;
		case eCUBETYPE::ADVANTAGE:
			state = eTileStateFlag::ADVANTAGECUBE;
			break;

		default:
			break;
		}

		AddTileState(pos.x, pos.z, state);
		_tiles[x][z]->_cube = cubeCtr->GetGameObject();
	}
}

bool Board::UpdateDetonate()
{
	bool result = false;
	int destroyCount = 0;

	for (int i = 0; i < _width; i++)
	{
		for (int j = 0; j < _height; j++)
		{
			// 3. 폭파 상태 처리
			if ((int)_tileStates[i][j] & (int)eTileStateFlag::DETONATE)
			{
				eTileStateFlag cubeType = (eTileStateFlag)((int)_tileStates[i][j] & CUBE);

				switch (cubeType)
				{
				case eTileStateFlag::NORMALCUBE:
					// NormalCube 수납
					destroyCount++;
					_tiles[i][j]->_cube->GetComponent<CubeController>()->StartRemove(REMOVE_TIME);
					break;
				case eTileStateFlag::DARKCUBE:
					// DarkCube 수납 및 스테이지 한 줄 삭제
					_tiles[i][j]->_cube->GetComponent<CubeController>()->StartRemove(REMOVE_TIME);
					_isPerfect = false;
					DestroyRow();
					break;
				case eTileStateFlag::ADVANTAGECUBE:
					// AdvantageCube 수납 및 AdvantageMine 설치
					_tileStates[i][j] = _tileStates[i][j] | (int)eTileStateFlag::ADVANTAGEMINE;
					_tiles[i][j]->EnableAdvantageMine();
					destroyCount++;
					_tiles[i][j]->_cube->GetComponent<CubeController>()->StartRemove(REMOVE_TIME);
					break;
				default:
					break;
				}

				if ((int)cubeType)
				{
					result = true;
				}

				_tileStates[i][j] = (int)_tileStates[i][j] & ~((int)eTileStateFlag::DETONATE);
				_tiles[i][j]->DisableDetonated();

				_tileStates[i][j] = (int)_tileStates[i][j] & ~CUBE;
				_tiles[i][j]->_cube = nullptr;
			}
		}
	}

	_gameManager->IncreaseScore(_playerIndex, destroyCount);

	for (int i = 0; i < _width; i++)
	{
		for (int j = 0; j < _height; j++)
		{
			if (_tileStates[i][j] & (int)eTileStateFlag::MINE)
			{
				return result;
			}
		}
	}
	_minePos.first = -1;
	_minePos.second = -1;

	return result;
}
