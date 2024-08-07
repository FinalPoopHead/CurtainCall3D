﻿#include <random>
#include "Board.h"
#include "../FloaterGameEngine/include/Input.h"

#include "Tile.h"
#include "AdvantageCube.h"
#include "DarkCube.h"
#include "NormalCube.h"
#include "CubeController.h"
#include "GameManager.h"
#include "Player.h"
#include "Camera.h"
#include "ShakeComponent.h"
#include "MainMenuScene.h"

constexpr int TILE_COUNT = 512;
constexpr int CUBE_COUNT = 512;

constexpr float GENERATE_TIME = 1.0f;
constexpr float ROLLING_TIME = 1.0f;
constexpr float REMOVE_TIME = 0.5f;
constexpr float ADDTILE_TIME = 1.0f;

constexpr float GENERATE_DELAY = 3.0f;
constexpr float ROLLING_DELAY = 0.5f;	// 아무것도 하지않고 굴러갈때의 딜레이
constexpr float DETONATE_DELAY = 2.0f;	// 폭파 후 딜레이
constexpr float ADDTILE_DELAY = 4.0f;	// 타일 추가 딜레이
constexpr float WAVECLEAR_DELAY = 4.0f;
constexpr float FALLTILE_DELAY = 2.0f;	// 타일 삭제 딜레이

constexpr int CUBE_DAMAGE = 1;
constexpr int DARKCUBE_DAMAGE = 1;
constexpr float SLOWVALUE = 0.5f;
constexpr float GAMESPEED_DEFAULT = 1.0f;
constexpr float GAMESPEED_FASTFORWARD = 8.0f;
constexpr float GAMESPEED_MAX = 4.0f;

Board::Board(GameManager* gameManager, int playerIndex, int width, int height, float offset /*= 4.00f*/) :
	flt::GameObject()
	, _soundComponent()
	, _soundIndex()
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
	, _isBattleMode(false)
	, _delayRemain(ROLLING_DELAY)
	, _gameSpeed(GAMESPEED_DEFAULT)
	, _battleModeSpeed(GAMESPEED_DEFAULT)
	, _nowRollingCount(0)
	, _currentShakePower()
	, _nowGeneratingCount(0)
	, _nowFallingTileCount()
	, _damageCount()
	, _isPerfect(true)
	, _isOnlyDarkRemain(false)
	, _isCameraWorking(false)
	, _isFirst(false)
	, _isWinner(false)
	, _nowAddTileCount()
	, _nextDestroyRow()
	, _minHeight()
	, _normalCubeDestroyCount()
	, _advantageCubeDestroyCount()
	, _darkCubeDestroyCount()
	, _minePos({ -1,-1 })
	, _fallingTileCount()
	, _addTiles()
{
	std::wstring path = L"../Resources/Sound/";
	_soundComponent = AddComponent<flt::SoundComponent>(true);
	_soundComponent->AddSound(path + L"Generate.mp3");
	_soundComponent->AddSound(path + L"CubeRoll.mp3");
	_soundComponent->AddSound(path + L"CubeDestroy.mp3");
	_soundComponent->AddSound(path + L"CubeDrop.mp3");
	_soundComponent->AddSound(path + L"SetMine.mp3");
	_soundComponent->AddSound(path + L"DetonateMine.mp3");
	_soundComponent->AddSound(path + L"DetonateAdvantage.mp3");
	_soundComponent->AddSound(path + L"TileAdd.mp3");
	_soundComponent->AddSound(path + L"TileDestroy.mp3");
	_soundComponent->AddSound(path + L"MS02gameover.wav");
	_soundComponent->AddSound(path + L"MS03discovery.wav");

	int index = 0;
	_soundIndex = std::unordered_map<std::string, int>();
	_soundIndex["Generate"] = index++;
	_soundIndex["CubeRoll"] = index++;
	_soundIndex["CubeDestroy"] = index++;
	_soundIndex["CubeDrop"] = index++;
	_soundIndex["SetMine"] = index++;
	_soundIndex["DetonateMine"] = index++;
	_soundIndex["DetonateAdvantage"] = index++;
	_soundIndex["TileAdd"] = index++;
	_soundIndex["TileDestroy"] = index++;
	_soundIndex["GameOver"] = index++;
	_soundIndex["StageClear"] = index++;

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

	if (_gameSpeed == GAMESPEED_FASTFORWARD)
	{
		OnFastForwarding();
	}

	// 치트키. 블랙큐브 빼고 전부 제거
	keyData = flt::GetKey(flt::KeyCode::lCtrl);
	if (keyData)
	{
		keyData = flt::GetKeyDown(flt::KeyCode::p);
		if (keyData)
		{
			std::list<CubeController*> removeList;
			for (auto& cubeCtr : _runningCubeControllers)
			{
				if (cubeCtr->GetCubeType() == eCUBETYPE::DARK)
				{
					continue;
				}

				auto pos = cubeCtr->GetPosition();
				int x = 0;
				int z = 0;

				ConvertToTileIndex(pos.x, pos.z, x, z);

				_tiles[x][z]->_cube = nullptr;
				_tileStates[x][z] = (int)_tileStates[x][z] & ~CUBE;
				removeList.push_back(cubeCtr);
			}

			for (auto& cubeCtr : removeList)
			{
				cubeCtr->StartRemove(REMOVE_TIME);
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
			if (_isBattleMode)
			{
				CheckMinHeight();
			}
			// 웨이브 클리어 연출 보여주고 다음 웨이브 출발
			else if (_runningCubeControllers.empty() && !_waveCubeControllers.empty())
			{
				_isPerfect = true;
				_isOnlyDarkRemain = false;
				_runningCubeControllers = _waveCubeControllers.front();
				_waveCubeControllers.pop_front();
				for (auto& cubeCtr : _runningCubeControllers)
				{
					cubeCtr->SetIsRunning(true);
				}
			}
			else if (!_isCutScene && _waveCubeControllers.empty())
			{
				_gameManager->OnEndLevel(_playerIndex);
			}
		}
		else
		{
			break;
		}

		if (!_runningCubeControllers.empty())
		{
			TickCubesRolling(ROLLING_TIME);
		}
		break;
	case eBoardState::WAITING:
		Wait(deltaSecond * _gameSpeed);
		break;
	case eBoardState::CUBEROLLING:
		if (_runningCubeControllers.empty())
		{
			_boardState = eBoardState::WAITING;
		}
		return;
		break;
	case eBoardState::STAGECHANGING:
		// TODO : 스테이지 변경 연출 -> 된걸로알고있음
		return;
		break;
	case eBoardState::TILEDESTROYING:
	case eBoardState::CUBEGENERATING:
	case eBoardState::ADDTILE:
		return;
		break;
	default:
		break;
	}
}

void Board::PostUpdate(float deltaSeoncd)
{
	if (_isGameOver)
	{
		return;
	}

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

void Board::SetTileStateWithCubeCtr(CubeController* cubeCtr)
{
	auto pos = cubeCtr->GetPosition();
	int x = 0;
	int z = 0;
	ConvertToTileIndex(pos.x, pos.z, x, z);

	if (x < 0 || x >= _width || z < 0 || z >= _height)
	{
		return;
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
	float leftTopX = 0.0f;
	float leftTopY = 0.0f;
	ConvertToTilePosition(0, 0, leftTopX, leftTopY);

	float rightBottomX = 0.0f;
	float rightBottomY = 0.0f;
	ConvertToTilePosition(_width - 1, _height - 1, rightBottomX, rightBottomY);

	x = (leftTopX + rightBottomX) / 2.0f;
	y = (leftTopY + rightBottomY) / 2.0f;

	return true;
}

bool Board::GetRatioPosition(float ratioX, float ratioY, float& outX, float& outY)
{
	float leftTopX = 0.0f;
	float leftTopY = 0.0f;
	ConvertToTilePosition(0, 0, leftTopX, leftTopY);

	float rightBottomX = 0.0f;
	float rightBottomY = 0.0f;
	ConvertToTilePosition(_width - 1, _height - 1, rightBottomX, rightBottomY);

	outX = leftTopX + (rightBottomX - leftTopX) * ratioX;
	outY = leftTopY + (rightBottomY - leftTopY) * ratioY;

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

void Board::GenerateLevel(std::vector<std::vector<int>> levelLayout, int waveCount, bool isFirst /*= false*/)
{
	_isGameOver = false;
	_isCutScene = false;

	int width = levelLayout.size();
	int height = levelLayout[0].size();

	_waveCubeControllers.clear();
	int waveHeight = height / waveCount;
	int heightCount = 0;
	std::list<CubeController*> waveCubes;
	float startDelay = 0.15f;
	float delayBase = 0.5f;

	for (int j = 0; j < height; ++j)
	{
		if (j >= _height)
		{
			_waveCubeControllers.push_front(waveCubes);
			waveCubes.clear();
			break;
		}

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
				dynamic_cast<NormalCube*>(cube)->SetTexture(eCubeTexture::NEUTRAL);
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
				ASSERT(false, "Invalid CubeType");
				break;
			}

			auto cubeCtr = cube->GetComponent<CubeController>();
			waveCubes.push_back(cubeCtr);
			cubeCtr->StartGenerate(GENERATE_TIME, delayBase + startDelay * j);

			cube->tr.SetPosition(x, 0.0f, z);
			cube->Enable();
			_tiles[i][j]->_cube = cube;
			_nowGeneratingCount++;
		}

		++heightCount;

		if (heightCount >= waveHeight)
		{
			heightCount = 0;
			_waveCubeControllers.push_front(waveCubes);
			waveCubes.clear();
		}

		_soundComponent->Play(_soundIndex["Generate"]);
	}

	_boardState = eBoardState::CUBEGENERATING;
	if (_height == 0)
	{
		_boardState = eBoardState::NONE;
	}

	_runningCubeControllers = _waveCubeControllers.front();
	_waveCubeControllers.pop_front();
	_isPerfect = true;
	_isOnlyDarkRemain = false;

	for (auto& cubeCtr : _runningCubeControllers)
	{
		cubeCtr->SetIsRunning(true);
	}

	auto player = _gameManager->GetPlayer(_playerIndex);
	if (isFirst)
	{
		float time = 5.0f;
		float x;
		float z;
		GetRatioPosition(1.0f, 0.1f, x, z);
		auto startPos = flt::Vector3f(x + 20.0f, 0.0f, z);
		flt::Quaternion startRot;
		startRot.SetEuler({ 0.0f,-90.0f,0.0f });

		GetRatioPosition(0.5f, 0.75f, x, z);
		auto targetPos = flt::Vector3f(x, 20.0f, z - 40.0f);
		flt::Quaternion targetRot;
		targetRot.SetEuler({ 15.0f,0.0f,0.0f });

		player->camera->tr.SetPosition(startPos);
		player->camera->tr.SetRotation(startRot);
		player->camera->TweenMove(targetPos, time / _battleModeSpeed);
		player->camera->TweenRotate(targetRot, time / _battleModeSpeed);
		_isCameraWorking = true;
	}
	else
	{
		player->camera->LookGenerating();
		_isCameraWorking = true;
	}

	_isFirst = isFirst;
}

void Board::GenerateWave(std::vector<std::vector<int>> waveLayout)
{
	int width = waveLayout.size();
	int height = waveLayout[0].size();

	float startDelay = 0.15f;
	float delayBase = 0.5f;

	for (int j = 0; j < height; ++j)
	{
		if (j >= _height)
		{
			break;
		}

		for (int i = 0; i < width; ++i)
		{
			float x = _tiles[i][j]->tr.GetWorldPosition().x;
			float z = _tiles[i][j]->tr.GetWorldPosition().z;

			flt::GameObject* cube = nullptr;

			switch (waveLayout[i][j])
			{
			case 1:
				if (_normalCubePool.empty())
				{
					ASSERT(false, "NormalCubePool is Empty");
				}
				cube = _normalCubePool.front();
				dynamic_cast<NormalCube*>(cube)->SetTexture(eCubeTexture::NEUTRAL);
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
// 				if (_darkCubePool.empty())
// 				{
// 					ASSERT(false, "DarkCubePool is Empty");
// 				}
// 				cube = _darkCubePool.front();
// 				_darkCubePool.pop_front();
				// 먼저 클리어한 사람에게 어드밴티지 보상
				if (_advantageCubePool.empty())
				{
					ASSERT(false, "AdvantageCubePool is Empty");
				}
				cube = _advantageCubePool.front();
				_advantageCubePool.pop_front();
				break;
			default:
				ASSERT(false, "Invalid CubeType");
				break;
			}

			auto cubeCtr = cube->GetComponent<CubeController>();
			_runningCubeControllers.push_back(cubeCtr);
			cubeCtr->StartGenerate(GENERATE_TIME, delayBase + startDelay * j);

			cube->tr.SetPosition(x, 0.0f, z);
			cube->Enable();
			_tiles[i][j]->_cube = cube;
			_nowGeneratingCount++;
		}

		_soundComponent->Play(_soundIndex["Generate"]);
	}

	_boardState = eBoardState::CUBEGENERATING;
	if (_height == 0)
	{
		_boardState = eBoardState::NONE;
	}

	_isOnlyDarkRemain = false;

	auto player = _gameManager->GetPlayer(_playerIndex);

	player->camera->LookGenerating();
	_isCameraWorking = true;
}

void Board::DropGarbageLine(int lineCount)
{
	if (lineCount <= 0)
	{
		return;
	}

	int width = _width;
	int height = lineCount;

	float heightDelay = 0.15f;
	float randomDelayValue = 0.08f;

	int dropChance = 75;

	int darkCube = 7;
	int advantageCube = 25;
	int normalCube = 100 - darkCube - advantageCube;

	std::random_device rd;

	for (int j = 0; j < height; ++j)
	{
		if (j >= _height)
		{
			break;
		}

		for (int i = 0; i < width; ++i)
		{
			int dropValue = rd() % 100;
			if (dropValue >= dropChance)
			{
				continue;
			}

			float x = _tiles[i][j]->tr.GetWorldPosition().x;
			float z = _tiles[i][j]->tr.GetWorldPosition().z;

			flt::GameObject* cube = nullptr;

			int randomValue = rd() % 100;

			if (randomValue < darkCube)
			{
				if (_darkCubePool.empty())
				{
					ASSERT(false, "DarkCubePool is Empty");
				}
				cube = _darkCubePool.front();
				_darkCubePool.pop_front();
			}
			else if (randomValue < darkCube + advantageCube)
			{
				if (_advantageCubePool.empty())
				{
					ASSERT(false, "AdvantageCubePool is Empty");
				}
				cube = _advantageCubePool.front();
				_advantageCubePool.pop_front();
			}
			else
			{
				if (_normalCubePool.empty())
				{
					ASSERT(false, "NormalCubePool is Empty");
				}
				cube = _normalCubePool.front();
				dynamic_cast<NormalCube*>(cube)->SetTexture(eCubeTexture::GARBAGE);
				_normalCubePool.pop_front();
			}

			float delayVar = randomDelayValue * (rd() % 5);

			auto cubeCtr = cube->GetComponent<CubeController>();
			cube->tr.SetPosition(x, 4.0f, z);		// 높이는 StartDrop에서 수정함
			cubeCtr->StartDrop(delayVar + heightDelay * j);
			_nowDropCubeCtrs.push_back(cubeCtr);
			_runningCubeControllers.push_back(cubeCtr);
			_tiles[i][j]->_cube = cube;
			cube->Enable();
		}
	}
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

	_currentShakePower = _nowRollingCount;

	if (_nowRollingCount > 0)
	{
		_boardState = eBoardState::CUBEROLLING;
	}
}

void Board::AddRow()
{
	_nowAddTileCount = _width;
	++_nextDestroyRow;
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

		tile->StartAdd(ADDTILE_TIME, { x,0.0f,z });
	}
}

void Board::OnEndWave()
{
	if (_isGameOver)
	{
		return;
	}

	if (_isPerfect)
	{
		if (_isBattleMode)
		{
			_gameSpeed = _battleModeSpeed;
		}
		else
		{
			_gameSpeed = GAMESPEED_DEFAULT;
		}
		AddRow();
		MoveCameraToEnd();
	}
	else
	{
		SetDelay(WAVECLEAR_DELAY);
	}
}

void Board::MoveCameraToEnd(bool onlyRight /*= false*/)
{
	float offsetX = 0.5f;
	float offsetY = 1.5f;
	float offsetZ = 1.0f;
	float offsetAngleX = 25.0f;
	float offsetAngleY = 20.0f;

	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;
	ConvertToTilePosition(_width - 1, _height, x, z);
	float tileWidth = 2.0f * _width;
	float tileHeight = 2.0f * _height;

	y += tileHeight / offsetY;
	z -= tileHeight / offsetZ;

	flt::Quaternion rot;

	auto player = _gameManager->GetPlayer(_playerIndex);

	if (onlyRight || player->camera->tr.GetWorldRotation().GetEuler().y < 0.0f)
	{
		x += tileWidth / offsetX;
		rot.SetEuler(offsetAngleX, -offsetAngleY, 0.0f);
	}
	else
	{
		x -= tileWidth + tileWidth / offsetX;
		rot.SetEuler(offsetAngleX, offsetAngleY, 0.0f);
	}

	float time = 0.5f;

	player->camera->TweenMove({ x,y,z }, time / _battleModeSpeed);
	player->camera->TweenRotate(rot, time / _battleModeSpeed);
	_isCameraWorking = true;
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

void Board::CheckMinHeight()
{
	_minHeight = 1000000;

	for (auto& cubeCtr : _runningCubeControllers)
	{
		auto pos = cubeCtr->GetPosition();
		int x = 0;
		int z = 0;
		ConvertToTileIndex(pos.x, pos.z, x, z);

		if (z < _minHeight)
		{
			_minHeight = z;
		}
	}

	std::cout << "MinHeight : " << _minHeight << std::endl;

	bool doGenerate = CheckOnlyDarkRemain() || _runningCubeControllers.empty();

	_gameManager->OnCheckMinHeight(_playerIndex, _minHeight, doGenerate);
}

void Board::ReturnCubeToPool(flt::GameObject* obj)
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

	CheckOnlyDarkRemain();

	if (_runningCubeControllers.empty() && !_isBattleMode)
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

	_soundComponent->Play(_soundIndex["SetMine"]);
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
	_soundComponent->Play(_soundIndex["DetonateMine"]);
}

void Board::DetonateAdvantageMine()
{
	if (_isGameOver)
	{
		return;
	}

	bool isSuccess = false;

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

				isSuccess = true;
			}

		}
	}

	if (isSuccess)
	{
		_soundComponent->Play(_soundIndex["DetonateAdvantage"]);
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
		_soundComponent->Play(_soundIndex["CubeRoll"]);
		
		float power = _currentShakePower / 100.0f;

		_gameManager->GetPlayer(_playerIndex)->camera->GetShakeComponent()->Impack(power/2.0f);
		//_gameManager->GetPlayer(_playerIndex)->SetPadVibration(false, power, 0.5f);
		_gameManager->GetPlayer(_playerIndex)->SetPadVibration(true, power, 0.5f);
	}
}

void Board::OnEndCubeGenerate()
{
	_nowGeneratingCount--;
	if (_nowGeneratingCount <= 0)
	{
		_nowGeneratingCount = 0;

		for (auto& col : _tileStates)
		{
			for (auto& tileState : col)
			{
				tileState = tileState & ~(int)eTileStateFlag::GENERATING;
			}
		}

		UpdateBoard();
		SetDelay(GENERATE_DELAY);

		for (auto& cubeCtr : _runningCubeControllers)
		{
			cubeCtr->SetIsRunning(true);
		}
	}
}

void Board::OnEndAddRowTile(Tile* tile)
{
	--_nowAddTileCount;
	ReturnTileToPool(tile);
	_addTiles.remove(tile);

	if (_nowAddTileCount == 0)
	{
		_nowAddTileCount = 0;
		Resize(_width, _height + 1);
		SetDelay(ADDTILE_DELAY);
		_soundComponent->Play(_soundIndex["TileAdd"]);
		_gameManager->GetPlayer(_playerIndex)->camera->GetShakeComponent()->Impack();
		_gameManager->GetPlayer(_playerIndex)->SetPadVibration(false, 1.0f, 0.5f);
		_gameManager->GetPlayer(_playerIndex)->SetPadVibration(true, 1.0f, 0.5f);
	}
	else if (_nowAddTileCount < 0)
	{
		_nowAddTileCount = 0;
	}
}

void Board::OnEndAddColumnTile(Tile* tile)
{
	--_nowAddTileCount;
	ReturnTileToPool(tile);
	_addTiles.remove(tile);

	if (_nowAddTileCount == 0)
	{
		_nowAddTileCount = 0;
		Resize(_width + 1, _height);
		SetDelay(ADDTILE_DELAY);
		_soundComponent->Play(_soundIndex["TileAdd"]);
	}
	else if (_nowAddTileCount < 0)
	{
		_nowAddTileCount = 0;
	}
}

void Board::OnStartTileFall(int x, int z)
{
	_tileStates[x][z] = (int)eTileStateFlag::NONE;
}

void Board::OnEndTileFall(int x, int z)
{
	_fallingTileCount[z]--;
	_nowFallingTileCount--;

	if (_fallingTileCount[z] == 0)
	{
		_fallingTileCount[z] = 0;
		Resize(_width, z);
		SetDelay(FALLTILE_DELAY);
	}
	else if (_fallingTileCount[z] < 0)
	{
		_fallingTileCount[z] = 0;
	}
}

void Board::OnEndCubeDrop(CubeController* cubeCtr)
{
	auto temp = _nowDropCubeCtrs.remove(cubeCtr);

	if (_nowDropCubeCtrs.empty())
	{
		for (auto& cubeCtr : _runningCubeControllers)
		{
			cubeCtr->SetIsRunning(true);
		}

		UpdateBoard();
		_soundComponent->Play(_soundIndex["CubeDrop"]);

		_gameManager->GetPlayer(_playerIndex)->camera->GetShakeComponent()->Impack();
		_gameManager->GetPlayer(_playerIndex)->SetPadVibration(false, 1.0f, 0.5f);
		_gameManager->GetPlayer(_playerIndex)->SetPadVibration(true, 1.0f, 0.5f);
	}
}

void Board::ShowBattleResult()
{
	// 먼저 모든 큐브 제거
	std::list<CubeController*> removeList;
	for (auto& cubeCtr : _runningCubeControllers)
	{
		auto pos = cubeCtr->GetPosition();
		int x = 0;
		int z = 0;

		ConvertToTileIndex(pos.x, pos.z, x, z);

		_tiles[x][z]->_cube = nullptr;
		_tileStates[x][z] = (int)_tileStates[x][z] & ~CUBE;
		removeList.push_back(cubeCtr);
	}

	for (auto& cubeCtr : removeList)
	{
		cubeCtr->StartRemove(REMOVE_TIME);
	}

	auto player = _gameManager->GetPlayer(_playerIndex);
	auto camera = player->camera;
	
	camera->StopCamera();

	_gameManager->SetResultText(_playerIndex, 0, L"NORMAL CUBE", L": " + std::to_wstring(_normalCubeDestroyCount));
	_gameManager->SetResultTextColor(_playerIndex, 0, { 1.0f,1.0f,1.0f,1.0f });

	_gameManager->SetResultText(_playerIndex, 1, L"ADVANTAGE CUBE", L": " + std::to_wstring(_advantageCubeDestroyCount));
	_gameManager->SetResultTextColor(_playerIndex, 1, { 0.3f,0.9f,0.3f,1.0f });

	_gameManager->SetResultText(_playerIndex, 2, L"DARK CUBE", L": " + std::to_wstring(_darkCubeDestroyCount));
	_gameManager->SetResultTextColor(_playerIndex, 2, { 0.5f,0.5f,0.5f,1.0f });

	_gameManager->StartWinLoseTween(_playerIndex, _isWinner);

	float forwardRatio = 10.0f;
	float upRatio = 1.5f;
	float rightRatio = 2.0f;

	// Winner를 바라보는 카메라 워크
	if (_isWinner)
	{
		auto playerPos = player->tr.GetWorldPosition();
		auto targetPos = playerPos;
		targetPos += (player->tr.Forward() * forwardRatio);
		targetPos += (player->tr.Up() * upRatio);

		auto secondPos = targetPos + (player->tr.Right() * rightRatio);

		auto posTween = flt::MakePosTween(&camera->tr);
		posTween->from(camera->tr.GetWorldPosition())
			.to(targetPos).during(0.5f).postDelay(3.0f)
			.to(secondPos).during(0.4f).easing(flt::ease::easeOutQuint)
			.onEnd([this]() {
			_gameManager->StartResultTween(_playerIndex, 3); 
			_gameManager->StartResultTween((_playerIndex + 1) % 2, 3);
				});

		auto targetRot = player->tr.GetWorldRotation();
		auto euler = targetRot.GetEuler();
		targetRot.SetEuler(euler.x, euler.y + 180, euler.z);

		auto rotTween = flt::MakeRotTween(&camera->tr);
		rotTween->from(camera->tr.GetWorldRotation())
			.to(targetRot).during(0.5f).postDelay(15.0f)
			.onEnd([]() {flt::SetScene(flt::CreateScene<MainMenuScene>()); });

		flt::StartTween(posTween);
		flt::StartTween(rotTween);
	}
	else
	{
		// Loser는 그냥 결과창만..

	}
}

void Board::AddBattleModeSpeed(float value)
{
	if (_battleModeSpeed >= GAMESPEED_MAX)
	{
		_battleModeSpeed = GAMESPEED_MAX;
		return;
	}

	_battleModeSpeed += value;
	_gameSpeed = _battleModeSpeed;
}

void Board::AddColumn()
{
	_nowAddTileCount = _height;
	_boardState = eBoardState::ADDTILE;

	for (int i = 0; i < _height; i++)
	{
		Tile* tile = GetTileFromPool();
		_addTiles.push_back(tile);

		float x = 0.0f;
		float z = 0.0f;
		ConvertToTileLocalPosition(_width + 16, i, x, z);

		tile->tr.SetPosition({ x, 0.0f, z });

		ConvertToTileLocalPosition(_width, i, x, z);
		x += tr.GetWorldPosition().x;
		z += tr.GetWorldPosition().z;

		tile->StartAdd(ADDTILE_TIME, { x,0.0f,z }, eAddType::COLUMN);
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

		_fallingTileCount[_nextDestroyRow]++;
		_nowFallingTileCount++;
		_tiles[i][_nextDestroyRow]->StartFall(delay + delayDelta * i, i, _nextDestroyRow);
	}
	--_nextDestroyRow;

	_soundComponent->Play(_soundIndex["TileDestroy"]);
	_gameManager->GetPlayer(_playerIndex)->SetPadVibration(false, 1.0f, 1.0f);
}

bool Board::IsMineSet()
{
	auto& [x, y] = _minePos;
	return x != -1 && y != -1;
}

void Board::OnEndPlayerFalling()
{
	_gameManager->OnEndPlayerFall(_playerIndex);
	_soundComponent->Play(_soundIndex["GameOver"]);
}

void Board::OnStartPlayerFall()
{
	_isGameOver = true;
	_gameManager->OnStartPlayerFall(_playerIndex);
}

void Board::AddCubeFallCount()
{
	_damageCount++;
	_isPerfect = false;
}

void Board::FastForward()
{
	_gameSpeed = GAMESPEED_FASTFORWARD;
}

void Board::EndFastForward()
{
	if (_isBattleMode)
	{
		_gameSpeed = _battleModeSpeed;
	}
	else
	{
		_gameSpeed = GAMESPEED_DEFAULT;
	}

	if (_isOnlyDarkRemain)
	{
		auto player = _gameManager->GetPlayer(_playerIndex);
		player->camera->TracePlayer();
		_isCameraWorking = false;
	}
}

void Board::SetBattleMode()
{
	_isBattleMode = true;

	// For Resource Load
	auto cube = _normalCubePool.back();
	dynamic_cast<NormalCube*>(cube)->SetTexture(eCubeTexture::GARBAGE);
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

	bool isHeightChange = _height != newHeight;

	_width = newWidth;
	_height = newHeight;

	if (isHeightChange)
	{
		_gameManager->OnHeightChange(_playerIndex, _height);
	}
}

void Board::Reset()
{
	// 진행중이던 게임 전부 리셋
	// 큐브풀로 다 갖고오고 타일풀로 다 갖고오고 진행중이던거 전부 제거하고..

	for (int i = 0; i < _width; ++i)
	{
		for (int j = 0; j < _height; ++j)
		{
			auto& tile = _tiles[i][j];

			float x = 0.0f;
			float z = 0.0f;
			ConvertToTileLocalPosition(i, j, x, z);

			tile->tr.SetPosition({ x, 0.0f, z });
			tile->DisableAdvantageMine();
			tile->DisableDetonated();
			tile->DisableMine();
			tile->_isFalling = false;
			tile->_isMoving = false;
			tile->_cube = nullptr;
		}
	}

	for (auto& tileCol : _tileStates)
	{
		for (auto& tileState : tileCol)
		{
			tileState = (int)eTileStateFlag::TILE;
		}
	}

	_boardState = eBoardState::NONE;
	_nowRollingCount = 0;
	_nowGeneratingCount = 0;
	_nowFallingTileCount = 0;
	_fallingTileCount.clear();
	_nowAddTileCount = 0;
	_nextDestroyRow = _height - 1;

	_minePos.first = -1;
	_minePos.second = -1;

	for (auto& wave : _waveCubeControllers)
	{
		for (auto& cubeCtr : wave)
		{
			ReturnCubeToPool(cubeCtr->GetGameObject());
		}
	}
	_waveCubeControllers.clear();

	for (auto& cubeCtr : _runningCubeControllers)
	{
		ReturnCubeToPool(cubeCtr->GetGameObject());
	}
	_runningCubeControllers.clear();

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

	if (_isFirst)
	{
		_gameManager->OnStageStart();
		_isFirst = false;
	}

	if (_isCameraWorking && !_isOnlyDarkRemain)
	{
		if (_isBattleMode)
		{
			_gameSpeed = _battleModeSpeed;
		}
		else
		{
			_gameSpeed = GAMESPEED_DEFAULT;
		}

		auto player = _gameManager->GetPlayer(_playerIndex);
		player->camera->TracePlayer();
		_isCameraWorking = false;

		SetDelay(0.5f);		// 카메라 워크가 끝나면서 갑자기 웨이브가 시작되므로
	}
}

void Board::OnFastForwarding()
{
	if (_isOnlyDarkRemain)
	{
		MoveCameraToEnd();
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

	for (auto& wave : _waveCubeControllers)
	{
		for (auto& cubeCtr : wave)
		{
			SetTileStateWithCubeCtr(cubeCtr);
		}
	}

	for (auto& cubeCtr : _runningCubeControllers)
	{
		SetTileStateWithCubeCtr(cubeCtr);
	}

	if (_isBattleMode)
	{
		CheckMinHeight();
	}
}

bool Board::UpdateDetonate()
{
	bool result = false;
	int destroyCount = 0;

	std::list<flt::Vector3f> cubePos;

	for (int i = 0; i < _width; i++)
	{
		for (int j = 0; j < _height; j++)
		{
			// 3. 폭파 상태 처리
			if ((int)_tileStates[i][j] & (int)eTileStateFlag::DETONATE)
			{
				eTileStateFlag cubeType = (eTileStateFlag)((int)_tileStates[i][j] & CUBE);

				CubeController* cubeCtr = nullptr;
				if (_tiles[i][j]->_cube != nullptr)
				{
					cubeCtr = _tiles[i][j]->_cube->GetComponent<CubeController>();
				}

				switch (cubeType)
				{
				case eTileStateFlag::NORMALCUBE:
					// NormalCube 수납
					if (cubeCtr->IsRunning())
					{
						++destroyCount;
						++_normalCubeDestroyCount;
						cubeCtr->StartRemove(REMOVE_TIME);

						float x = 0.0f, y = 0.0f, z = 0.0f;
						ConvertToTilePosition(i, j, x, z);
						cubePos.push_back({ x,y,z });
					}
					break;
				case eTileStateFlag::DARKCUBE:
					// DarkCube 수납 및 스테이지 한 줄 삭제
					if (cubeCtr->IsRunning())
					{
						++_darkCubeDestroyCount;
						_isPerfect = false;
						DestroyRow();
						cubeCtr->StartRemove(REMOVE_TIME);
					}
					break;
				case eTileStateFlag::ADVANTAGECUBE:
					// AdvantageCube 수납 및 AdvantageMine 설치
					if (cubeCtr->IsRunning())
					{
						_tileStates[i][j] = _tileStates[i][j] | (int)eTileStateFlag::ADVANTAGEMINE;
						_tiles[i][j]->EnableAdvantageMine();
						++destroyCount;
						++_advantageCubeDestroyCount;
						cubeCtr->StartRemove(REMOVE_TIME);

						float x = 0.0f, y = 0.0f, z = 0.0f;
						ConvertToTilePosition(i, j, x, z);
						cubePos.push_back({ x,y,z });
					}
					break;
				default:
					break;
				}

				if ((int)cubeType)
				{
					result = true;
					_soundComponent->Play(_soundIndex["CubeDestroy"]);
					_gameManager->GetPlayer(_playerIndex)->SetPadVibration(true, 1.0f, 1.0f);
				}

				_tileStates[i][j] = (int)_tileStates[i][j] & ~((int)eTileStateFlag::DETONATE);
				_tiles[i][j]->DisableDetonated();

				_tileStates[i][j] = (int)_tileStates[i][j] & ~CUBE;
				_tiles[i][j]->_cube = nullptr;
			}
		}
	}

	flt::Vector3f averagePos = {0.0f,0.0f,0.0f};

	for (auto& pos : cubePos)
	{
		averagePos += pos;
	}

	averagePos /= destroyCount;

	_gameManager->OnDestroyCubes(_playerIndex, destroyCount, averagePos);

	if (_isBattleMode)
	{
		CheckMinHeight();
	}

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

bool Board::CheckOnlyDarkRemain()
{
	if (_runningCubeControllers.empty())
	{
		_isOnlyDarkRemain = false;
		return false;
	}

	for (auto& cubeCtr : _runningCubeControllers)
	{
		auto cubeType = cubeCtr->GetCubeType();

		if (cubeType == eCUBETYPE::ADVANTAGE
			|| cubeType == eCUBETYPE::NORMAL)
		{
			_isOnlyDarkRemain = false;
			return false;
		}
	}

	_isOnlyDarkRemain = true;
	return true;
}
