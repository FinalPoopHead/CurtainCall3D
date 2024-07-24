#include "PlayerModel.h"

PlayerModel::PlayerModel() 
	: _renderer(nullptr)
{
	_renderer = AddComponent<flt::RendererComponent>(true);

	std::wstring filePath = L"..\\Resources\\Models\\Rob02.fbx";
	_renderer->SetFilePath(filePath);
	_renderer->SetMaterial(0, L"..\\Resources\\Textures\\Rob02Yellow_AlbedoTransparency.png", flt::RawMaterial::TextureType::ALBEDO_OPACITY);
	_renderer->SetMaterial(0, L"..\\Resources\\Textures\\Rob02_Normal.dds", flt::RawMaterial::TextureType::NORMAL);
	_renderer->SetMaterial(0, L"..\\Resources\\Textures\\Rob02White_MetallicSmoothness.dds", flt::RawMaterial::TextureType::METALLIC);
	_renderer->SetMaterial(0, L"..\\Resources\\Textures\\Rob02White_Roughness.png", flt::RawMaterial::TextureType::ROUGHNESS);
}

void PlayerModel::PlayAnimation(int index, bool isLoop)
{
	_renderer->PlayAnimation(index, isLoop);
}

void PlayerModel::PlayWalk()
{
	_renderer->PlayAnimation(21, true);
}

void PlayerModel::PlayIdle()
{
	_renderer->PlayAnimation(0, true);
}

void PlayerModel::SetAlbedoPath(std::wstring path)
{
	_renderer->SetMaterial(0, path, flt::RawMaterial::TextureType::ALBEDO_OPACITY);
}
