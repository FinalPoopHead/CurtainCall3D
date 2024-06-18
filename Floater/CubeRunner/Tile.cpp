#include <iostream>
#include "Tile.h"
#include "Mine.h"
#include "AdvantageMine.h"
#include "DetonatedMine.h"

Tile::Tile()
	: _cube(nullptr)
	, _mine(nullptr)
	, _advantageMine(nullptr)
	, _detonated(nullptr)
{
	std::wstring filePath = L"..\\Resources\\Models\\cube.fbx";

	flt::RendererComponent* renderer = AddComponent<flt::RendererComponent>(true);
	renderer->SetFilePath(filePath);
	renderer->SetMaterial(0, L"../Resources/Textures/NormalCube.png", flt::RawMaterial::TextureType::ALBEDO_OPACITY);

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
