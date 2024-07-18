#include "NormalCube.h"
#include "CubeController.h"

std::wstring neutralBaseColor = L"../Resources/Textures/Concrete 1-diffuse.png";
std::wstring neutralNormal = L"../Resources/Textures/Concrete 1-normal.png";
std::wstring neutralMetallic = L"";
std::wstring neutralAO = L"";


std::wstring garbageBaseColor = L"../Resources/Textures/Floor_3_Albedo.png";
std::wstring garbageNormal = L"../Resources/Textures/Floor_3_Normal.png";
std::wstring garbageMetallic = L"../Resources/Textures/Floor_3_Metallic.png";
std::wstring garbageAO = L"../Resources/Textures/Floor_3_Occlusion.png";

NormalCube::NormalCube() :
	_renderer()
	, _textureType(eCubeTexture::NEUTRAL)
{
	std::wstring filePath = L"..\\Resources\\Models\\BrickBlock.fbx";
	_renderer = AddComponent<flt::RendererComponent>(true);
	_renderer->SetFilePath(filePath);
	_renderer->SetMaterial(0, neutralBaseColor, flt::RawMaterial::TextureType::ALBEDO_OPACITY);
	_renderer->SetMaterial(0, neutralNormal, flt::RawMaterial::TextureType::NORMAL);

	const auto& cubeCtr = AddComponent<CubeController>(true);
	cubeCtr->SetCubeType(eCUBETYPE::NORMAL);
}

NormalCube::~NormalCube()
{

}

void NormalCube::SetTexture(eCubeTexture textureType)
{
	if (_textureType == textureType)
	{
		return;
	}

	_textureType = textureType;

	switch (textureType)
	{
	case eCubeTexture::NEUTRAL:
		_renderer->SetMaterial(0, neutralBaseColor, flt::RawMaterial::TextureType::ALBEDO_OPACITY);
		_renderer->SetMaterial(0, neutralNormal, flt::RawMaterial::TextureType::NORMAL);
		_renderer->SetMaterial(0, neutralMetallic, flt::RawMaterial::TextureType::METALLIC);
		_renderer->SetMaterial(0, neutralAO, flt::RawMaterial::TextureType::AO);
		break;
	case eCubeTexture::GARBAGE:
		_renderer->SetMaterial(0, garbageBaseColor, flt::RawMaterial::TextureType::ALBEDO_OPACITY);
		_renderer->SetMaterial(0, garbageNormal, flt::RawMaterial::TextureType::NORMAL);
		_renderer->SetMaterial(0, garbageMetallic, flt::RawMaterial::TextureType::METALLIC);
		_renderer->SetMaterial(0, garbageAO, flt::RawMaterial::TextureType::AO);
		break;
	default:
		break;
	}
}
