#include <iostream>
#include "Tile.h"
#include "Board.h"
#include "Mine.h"
#include "AdvantageMine.h"
#include "DetonatedMine.h"

Tile::Tile(Board* board) 
	: _board(board)
	, _cube(nullptr)
	, _mine(nullptr)
	, _advantageMine(nullptr)
	, _detonated(nullptr)
	, _isMoving(false)
	, _movingTime(0.0f)
	, _elapsedTime(0.0f)
	, _targetPos()
{
	std::wstring filePath = L"..\\Resources\\Models\\BrickBlock.fbx";

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

void Tile::OnCreate()
{
	//std::cout << "Tile OnCreate" << std::endl;
}

void Tile::OnEnable()
{
	//std::cout << "Tile OnEnable" << std::endl;
}

void Tile::OnDisable()
{
	//std::cout << "Tile OnDisable" << std::endl;
}

void Tile::OnDestroy()
{
	//std::cout << "Tile OnDestroy" << std::endl;
}

void Tile::Update(float deltaSecond)
{
	if (!_isMoving)
	{
		return;
	}

	_elapsedTime += deltaSecond * _board->GetFFValue();

	if(_elapsedTime >= _movingTime)
	{
		_isMoving = false;
		_elapsedTime = _movingTime;
	}

	auto pos = tr.GetWorldPosition();
	flt::Vector3f curPosition = { pos.x,pos.y,pos.z };
	curPosition = flt::Vector3f::Lerp(curPosition, _targetPos, _elapsedTime/_movingTime);
	tr.SetWorldPosition(curPosition.x, curPosition.y, curPosition.z);

	if (_elapsedTime >= _movingTime)
	{
		_board->OnEndTileAdd();
		Destroy();
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

void Tile::StartAddMoving(float movingTime, flt::Vector3f targetPos)
{
	_isMoving = true;
	_elapsedTime = 0.0f;
	_movingTime = movingTime;
	_targetPos = targetPos;
}
