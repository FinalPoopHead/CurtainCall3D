#include "Mine.h"

Mine::Mine()
{
	// TODO : 임시로 Blender Cone FBX 넣어둠.
	std::wstring filePath = L"..\\Resources\\Models\\Cone.fbx";

	flt::RendererComponent* renderer = AddComponent<flt::RendererComponent>(true);
	renderer->SetFilePath(filePath);
	renderer->SetMaterial(0, L"../Resources/Textures/Rob02_Normal.dds", flt::RawMaterial::TextureType::ALBEDO_OPACITY);
}

Mine::~Mine()
{

}
