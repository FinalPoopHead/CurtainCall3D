#include "Tile.h"
#include "../FloaterRendererCommon/include/ModelLoader.h"
#include <iostream>


Tile::Tile()
{
	std::wstring filePath = L"..\\Resources\\Models\\cube.fbx";

	flt::RendererComponent* renderer = AddComponent<flt::RendererComponent>(true);
	renderer->SetFilePath(filePath);
	renderer->SetMaterial(0, L"NormalCube.png", flt::RawMaterial::TextureType::ALBEDO_OPACITY);
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
