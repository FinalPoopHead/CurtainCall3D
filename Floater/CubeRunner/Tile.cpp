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

	_mine = flt::CreateGameObject<Mine>(false);
	_mine->tr.SetParent(&tr);
	_mine->tr.AddWorldPosition(0.0f, 8.0f, 0.0f);

	_advantageMine = flt::CreateGameObject<AdvantageMine>(false);
	_advantageMine->tr.SetParent(&tr);
	_advantageMine->tr.AddWorldPosition(0.0f, 8.0f, 0.0f);

	_detonated = flt::CreateGameObject<DetonatedMine>(false);
	_detonated->tr.SetParent(&tr);
	_detonated->tr.AddWorldPosition(0.0f, 8.0f, 0.0f);
}

Tile::~Tile()
{

}

void Tile::OnCreate()
{
	std::cout << "Tile OnCreate" << std::endl;
}

void Tile::OnEnable()
{
	std::cout << "Tile OnEnable" << std::endl;
}

void Tile::OnDisable()
{
	std::cout << "Tile OnDisable" << std::endl;
}

void Tile::OnDestroy()
{
	std::cout << "Tile OnDestroy" << std::endl;
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
