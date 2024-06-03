#include "Tile.h"
#include "../FloaterRendererCommon/include/ModelLoader.h"
#include <iostream>


Tile::Tile()
{
	std::wstring filePath = L"..\\x64\\fbx\\SM_Box_Cargo.fbx";

	flt::RendererComponent* renderer = AddComponent<flt::RendererComponent>();

	flt::ModelLoader loader;
	flt::RawScene rawScene;
	loader.Load(filePath, &rawScene);
	flt::RawNode* node = rawScene.nodes[0];

	renderer->SetRawNode(node);
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
