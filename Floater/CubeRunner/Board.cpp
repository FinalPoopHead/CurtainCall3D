#include "Board.h"
#include "../FloaterGameEngine/include/Input.h"

#include "Tile.h"
#include "AdvantageCube.h"
#include "DarkCube.h"
#include "NormalCube.h"
#include "CubeController.h"
#include "GameManager.h"

constexpr int TILECOUNT = 256;
constexpr int CUBECOUNT = 64;
constexpr float ROLLINGTIME = 1.0f;
constexpr float ROLLINGDELAY = 0.5f;	// 아무것도 하지않고 굴러갈때의 딜레이
constexpr float DETONATEDELAY = 2.0f;	// 폭파 후 딜레이
constexpr float FALLTILEDELAY = 5.0f;	// 타일 삭제 딜레이
constexpr float ADDTILEDELAY = 5.0f;	// 타일 추가 딜레이
constexpr float TILEADDTIME = 2.0f;
constexpr float CUBEREMOVETIME = 0.5f;
constexpr float CUBERISINGTIME = 1.0f;
constexpr float CUBERISINGDELAY = 0.3f;
constexpr int CUBEDAMAGE = 1;
constexpr int DARKCUBEDAMAGE = 1;
constexpr float SLOWVALUE = 0.5f;
constexpr float FFDEFAULT = 1.0f;
constexpr float FFVALUE = 8.0f;

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
	, _isGameStart(false)
	, _isWaveRunning(false)
	, _delayRemain(ROLLINGDELAY)
	, _fastForwardValue(FFDEFAULT)
	, _nowRollingCount(0)
	, _nowRisingCount(0)
	, _isPerfect(true)
	, _nowAddTileCount()
	, _destroyRowCount()
	, _minePos({ -1,-1 })
	, _advantageMinePosList()
{

}

Board::~Board()
{

}

void Board::OnCreate()
{
	// Create TilePool
	for (int i = 0; i < TILECOUNT; i++)
	{
		Tile* tile = flt::CreateGameObject<Tile>(false, this);
		tile->tr.SetParent(&this->tr);
		_tilePool.push_back(tile);
	}

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
				TileStateFlag cubeType = (TileStateFlag)((int)_tileState[i][j] & CUBE);

				switch (cubeType)
				{
				case TileStateFlag::NORMALCUBE:
					// NormalCube 수납
					_tiles[i][j]->_cube->GetComponent<CubeController>()->StartRemoving(CUBEREMOVETIME);
					_tiles[i][j]->_cube = nullptr;
					_tileState[i][j] = (int)_tileState[i][j] & ~CUBE;
					break;
				case TileStateFlag::ADVANTAGECUBE:
					// AdvantageCube 수납
					_tiles[i][j]->_cube->GetComponent<CubeController>()->StartRemoving(CUBEREMOVETIME);
					_tiles[i][j]->_cube = nullptr;
					_tileState[i][j] = (int)_tileState[i][j] & ~CUBE;
					break;
				default:
					break;
				}
			}
		}
	}

	if (_nowRisingCount > 0)
	{
		return;
	}

	if (_nowRollingCount > 0)
	{
		return;
	}

	if (_nowRollingCount == 0)
	{
		_delayRemain -= deltaTime * _fastForwardValue;
		if (_delayRemain <= 0.0f)
		{
			bool isCubeDetonated = UpdateDetonate();

			if (isCubeDetonated)		// 폭파 된 것이 있다면 delay 연장.
			{
				_delayRemain = DETONATEDELAY;
			}
			else if (_isWaveRunning)	// 폭파 된 것이 없다면 구르기 시작.
			{
				TickCubesRolling(ROLLINGTIME);
			}
		}
		else
		{
			return;
		}
	}

	if (flt::GetKeyDown(flt::KeyCode::n))
	{
		_isGameStart = true;
	}

	if (_isGameOver || !_isGameStart)
	{
		return;
	}

	if (!_isWaveRunning && _delayRemain <= 0.0f)
	{
		// TODO : 웨이브 클리어 연출 보여주고 웨이브 생성
		_TEST_GenerateRandomWave();
		_isWaveRunning = true;
	}
}

bool Board::SetTileState(float x, float y, TileStateFlag state)
{
	ASSERT(state != TileStateFlag::NONE, "Can`t Set None");

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
		return (int)TileStateFlag::NONE;
	}
	if (tileY < 0 || tileY >= _height)
	{
		return (int)TileStateFlag::NONE;
	}

	return _tileState[tileX][tileY];
}

int Board::QueryNextTileState(float x, float y)
{
	int tileX = 0;
	int tileY = 0;
	ConvertToTileIndex(x, y, tileX, tileY);

	if (tileX < 0 || tileX >= _width)
	{
		return (int)TileStateFlag::NONE;
	}
	if (tileY < 0 || tileY >= _height)
	{
		return (int)TileStateFlag::NONE;
	}

	int nextTileY = tileY - 1;

	if (_nowRollingCount > 0)
	{
		if (nextTileY < 0)
		{
			return _tileState[tileX][tileY] & ~CUBE;
		}

		return _tileState[tileX][nextTileY] | (_tileState[tileX][nextTileY] & CUBE);
	}
	else
	{
		return _tileState[tileX][tileY];
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
			_cubeControllers.push_back(cubeCtr);
			cubeCtr->StartRising(CUBERISINGTIME, CUBERISINGDELAY * delayCount);

			cube->tr.SetPosition(x, 0.0f, z);
			cube->Enable();
			_tiles[i][j]->_cube = cube;
			_nowRisingCount++;
			delayCount++;
		}

		delayCount = 0;
	}
}

void Board::TickCubesRolling(float rollingTime)
{
	for (auto& cubeCtr : _cubeControllers)
	{
		if (cubeCtr->StartRolling(rollingTime))
		{
			_nowRollingCount++;
		}
	}
}

void Board::AddRow()
{
	// TODO : perfect 클리어 ! 맨 끝에 추가 row 부착.
			//			부착연출 끝나면 Resize함수 호출하면 되겠다.
	_nowAddTileCount = _width;
	_delayRemain = ADDTILEDELAY;

	for (int i = 0; i < _width; i++)
	{
		// TODO : 생성하지말고 pool 두고 갖다쓰자
		Tile* tile = GetTileFromPool();

		float x = 0.0f;
		float z = 0.0f;
		ConvertToTileLocalPosition(i, _height + 16, x, z);

		tile->tr.SetPosition({ x, 0.0f, z });

		ConvertToTileLocalPosition(i, _height, x, z);
		x += tr.GetWorldPosition().x;
		z += tr.GetWorldPosition().z;

		tile->StartAddRow(TILEADDTIME, { x,0.0f,z });
	}
}

void Board::OnEndWave()
{
	_isWaveRunning = false;
	_delayRemain = DETONATEDELAY;

	if (_isPerfect)
	{
		AddRow();
	}
}

Tile* Board::GetTileFromPool()
{
	Tile* tile = nullptr;

	if (_tilePool.empty())
	{
		for (int i = 0; i < TILECOUNT; i++)
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
	auto removeCount = _cubeControllers.remove(cubeCtr);

	if (removeCount == 0)
	{
		return;
	}

	if (_cubeControllers.empty())
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
	if (_tileState[tileX][tileZ] & (int)TileStateFlag::ADVANTAGEMINE
		|| _tileState[tileX][tileZ] & (int)TileStateFlag::MINE)
	{
		return;
	}

	_tileState[tileX][tileZ] = (int)_tileState[tileX][tileZ] | (int)TileStateFlag::MINE;

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
	_tileState[x][y] = (int)_tileState[x][y] & ~((int)TileStateFlag::MINE);
	_tileState[x][y] = (int)_tileState[x][y] | (int)TileStateFlag::DETONATE;
}

void Board::DetonateAdvantageMine()
{
	if (_isGameOver)
	{
		return;
	}

	while (!_advantageMinePosList.empty())
	{
		auto& [x, y] = _advantageMinePosList.front();
		_tileState[x][y] = (int)_tileState[x][y] & ~((int)TileStateFlag::ADVANTAGEMINE);
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

				_tileState[nextX][nextY] = _tileState[nextX][nextY] | (int)TileStateFlag::DETONATE;
				_tiles[nextX][nextY]->EnableDetonated();
			}
		}

		_advantageMinePosList.pop_front();
	}
}

void Board::OnEndRolling()
{
	_nowRollingCount--;
	if (_nowRollingCount <= 0)
	{
		_nowRollingCount = 0;
		_delayRemain = ROLLINGDELAY;
		UpdateBoard();
		if (UpdateDetonate())
		{
			_delayRemain = DETONATEDELAY;
		}
	}
}

void Board::OnEndRising()
{
	_nowRisingCount--;
	if (_nowRisingCount <= 0)
	{
		_nowRisingCount = 0;
		_delayRemain = ROLLINGDELAY;
		UpdateBoard();
	}
}

void Board::OnEndRowAdd()
{
	_nowAddTileCount--;

	if (_nowAddTileCount <= 0)
	{
		_nowAddTileCount = 0;
		Resize(_width, _height + 1);
	}
}

void Board::OnStartTileFall(int x, int z)
{
	_tileState[x][z] = (int)TileStateFlag::NONE;
	// TODO : Player가 위에 있었으면 게임오버 해야함
}

void Board::OnEndTileFall(int x, int z)
{
	_fallingTileCount[z]--;

	if (_fallingTileCount[z] <= 0)
	{
		_fallingTileCount[z] = 0;
		Resize(_width, _height - _destroyRowCount);
		_destroyRowCount--;
	}
}

void Board::DestroyRow()
{
	_destroyRowCount++;

	float delay = 0.3f;
	int destroyHeight =	_height - _destroyRowCount;
	_fallingTileCount[destroyHeight] = 0;

	for (int i = 0; i < _width; i++)
	{
		int randValue = rand() % 2;
		float delayDelta = (float)randValue / 10.0f + 0.08f;

		if (_minePos.first == i && _minePos.second == _height - 1)
		{
			_minePos.first = -1;
			_minePos.second = -1;
		}

		_tiles[i][destroyHeight]->StartFall(delay + delayDelta * i, i, destroyHeight);
		_fallingTileCount[destroyHeight]++;
	}

	_delayRemain = FALLTILEDELAY;
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

void Board::ReduceHPbyCubeFalling()
{
	_gameManager->ReduceHP(_playerIndex, CUBEDAMAGE);
	_isPerfect = false;
	// TODO : 에너지 다 깎이면 한 줄 삭제하고 체력 다시 채워줘야함.
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
	_tileState.resize(newWidth);

	for (int i = 0; i < newWidth; ++i)
	{
		_tileState[i].resize(newHeight);
		for (int j = 0; j < newHeight; ++j)
		{
			_tileState[i][j] = (int)TileStateFlag::TILE;
		}
	}

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

	for (int i = 0; i < newWidth; ++i)
	{
		for (int j = 0; j < newHeight; ++j)
		{
			_tileState[i][j] = (int)TileStateFlag::TILE;
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
	outZ *= -_tileSize;
}

void Board::UpdateBoard()
{
	for (int i = 0; i < _width; i++)
	{
		for (int j = 0; j < _height; j++)
		{
			_tileState[i][j] = _tileState[i][j] & ~CUBE;	// 큐브 타입만 제거
			_tiles[i][j]->_cube = nullptr;
		}
	}

	for (auto& cubeCtr : _cubeControllers)
	{
		auto pos = cubeCtr->GetPosition();
		int x = 0;
		int z = 0;
		ConvertToTileIndex(pos.x, pos.z, x, z);

		if (x < 0 || x >= _width || z < 0 || z >= _height)
		{
			continue;
		}

		TileStateFlag state = TileStateFlag::NONE;

		switch (cubeCtr->GetCubeType())
		{
		case eCUBETYPE::NORMAL:
			state = TileStateFlag::NORMALCUBE;
			break;
		case eCUBETYPE::DARK:
			state = TileStateFlag::DARKCUBE;
			break;
		case eCUBETYPE::ADVANTAGE:
			state = TileStateFlag::ADVANTAGECUBE;
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
			if ((int)_tileState[i][j] & (int)TileStateFlag::DETONATE)
			{
				TileStateFlag cubeType = (TileStateFlag)((int)_tileState[i][j] & CUBE);

				switch (cubeType)
				{
				case TileStateFlag::NORMALCUBE:
					// NormalCube 수납
					destroyCount++;
					_tiles[i][j]->_cube->GetComponent<CubeController>()->StartRemoving(CUBEREMOVETIME);
					break;
				case TileStateFlag::DARKCUBE:
					// DarkCube 수납 및 스테이지 한 줄 삭제
					_tiles[i][j]->_cube->GetComponent<CubeController>()->StartRemoving(CUBEREMOVETIME);
					_isPerfect = false;
					DestroyRow();
					break;
				case TileStateFlag::ADVANTAGECUBE:
					// AdvantageCube 수납 및 AdvantageMine 설치
					_tileState[i][j] = _tileState[i][j] | (int)TileStateFlag::ADVANTAGEMINE;
					_tiles[i][j]->EnableAdvantageMine();
					_advantageMinePosList.push_back({ i,j });
					destroyCount++;
					_tiles[i][j]->_cube->GetComponent<CubeController>()->StartRemoving(CUBEREMOVETIME);
					break;
				default:
					break;
				}

				if ((int)cubeType)
				{
					result = true;
				}

				_tileState[i][j] = (int)_tileState[i][j] & ~((int)TileStateFlag::DETONATE);
				_tiles[i][j]->DisableDetonated();

				_tileState[i][j] = (int)_tileState[i][j] & ~CUBE;
				_tiles[i][j]->_cube = nullptr;
			}
		}
	}

	_gameManager->OnCubeDestroy(_playerIndex, destroyCount);

	for (int i = 0; i < _width; i++)
	{
		for (int j = 0; j < _height; j++)
		{
			if (_tileState[i][j] & (int)TileStateFlag::MINE)
			{
				return result;
			}
		}
	}
	_minePos.first = -1;
	_minePos.second = -1;

	return result;
}
