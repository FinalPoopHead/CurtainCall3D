#include <iostream>
#include "Tile.h"
#include "Board.h"
#include "Mine.h"
#include "AdvantageMine.h"
#include "DetonatedMine.h"

#include "CubeController.h"

constexpr float GRAVITY = 9.8f;
constexpr float STARTFALLSPEED = 20.0f;
constexpr float FALLHEIGHT = -50.0f;

Tile::Tile(Board* board)
	: _board(board)
	, _cube(nullptr)
	, _mine(nullptr)
	, _advantageMine(nullptr)
	, _detonated(nullptr)
	, _isMoving(false)
	, _isFalling(false)
	, _movingTime(0.0f)
	, _elapsedTime(0.0f)
	, _startPos()
	, _targetPos()
	, _fallDelay()
	, _fallSpeed()
	, _x()
	, _z()
{
	std::wstring filePath = L"..\\Resources\\Models\\BrickBlock.fbx";

	// tr.SetScale(0.5f, 0.5f, 0.5f); // FOR DEBUG

	flt::RendererComponent* renderer = AddComponent<flt::RendererComponent>(true);
	renderer->SetFilePath(filePath);
	renderer->SetMaterial(0, L"../Resources/Textures/Concrete 1-diffuse.png", flt::RawMaterial::TextureType::ALBEDO_OPACITY);
	renderer->SetMaterial(0, L"../Resources/Textures/Concrete 1-normal.png", flt::RawMaterial::TextureType::NORMAL);
	renderer->SetMaterial(0, L"../Resources/Textures/Concrete 1-specular.png", flt::RawMaterial::TextureType::METALLIC);

	float coneHeight = 8.0f;

	_mine = flt::CreateGameObject<Mine>(true);
	_mine->tr.SetParent(&tr);
	_mine->Disable();

	_advantageMine = flt::CreateGameObject<AdvantageMine>(true);
	_advantageMine->tr.SetParent(&tr);
	_advantageMine->Disable();

	_detonated = flt::CreateGameObject<DetonatedMine>(true);
	_detonated->tr.SetParent(&tr);
	_detonated->Disable();
}

Tile::~Tile()
{

}

void Tile::OnDisable()
{
	_isMoving = false;
	_isFalling = false;
}

void Tile::PreUpdate(float deltaSecond)
{
	if (_isMoving)
	{
		_elapsedTime += deltaSecond;

		if (_elapsedTime >= _movingTime)
		{
			_isMoving = false;
			_elapsedTime = _movingTime;
		}

		auto pos = tr.GetWorldPosition();
		auto curPosition = flt::Vector3f::Lerp(_startPos, _targetPos, _elapsedTime / _movingTime);
		tr.SetWorldPosition(curPosition.x, curPosition.y, curPosition.z);

		if (_elapsedTime >= _movingTime)
		{
			_isMoving = false;
			_board->OnEndTileAdd(this);
		}
	}

	if (_isFalling)
	{
		Fall(deltaSecond * _board->GetFFValue());
	}
}

void Tile::EnableMine()
{
	_mine->Enable();
}

void Tile::DisableMine()
{
	_mine->Disable();
}

void Tile::EnableDetonated()
{
	_detonated->Enable();
}

void Tile::DisableDetonated()
{
	_detonated->Disable();
}

void Tile::EnableAdvantageMine()
{
	_advantageMine->Enable();
}

void Tile::DisableAdvantageMine()
{
	_advantageMine->Disable();
}

void Tile::StartAddRow(float movingTime, flt::Vector3f targetPos)
{
	_isMoving = true;
	_elapsedTime = 0.0f;
	_movingTime = movingTime;
	auto pos = tr.GetWorldPosition();
	_startPos.x = pos.x;
	_startPos.y = pos.y;
	_startPos.z = pos.z;
	_targetPos = targetPos;
}

void Tile::StartFall(float delay, int row, int col)
{
	_x = row;
	_z = col;

	DisableMine();
	DisableAdvantageMine();
	DisableDetonated();

	_fallSpeed = STARTFALLSPEED;
	_fallDelay = delay;
	_isFalling = true;
}

void Tile::Fall(float deltaSecond)
{
	if (_fallDelay > 0.0f)
	{
		_fallDelay -= deltaSecond;
		if (_fallDelay <= 0.0f)
		{
			_board->OnStartTileFall(_x,_z);

			if(_cube != nullptr)
			{
				auto cubeCtr = _cube->GetComponent<CubeController>();
				cubeCtr->StartFall();
			}
		}
		return;
	}

	_fallSpeed += GRAVITY * deltaSecond;
	tr.AddLocalPosition(0.0f, -_fallSpeed * deltaSecond, 0.0f);

	if (tr.GetWorldPosition().y <= FALLHEIGHT)
	{
		_isFalling = false;
		_board->OnEndTileFall(_x, _z);
	}
}
