#include "Mine.h"

Mine::Mine()
{
	// TODO : 임시로 아무 모델링파일 넣어둠
	std::wstring filePath = L"..\\Resources\\Models\\SK_TP_CH_Default.fbx";

	flt::RendererComponent* renderer = AddComponent<flt::RendererComponent>(true);
	renderer->SetFilePath(filePath);
	renderer->SetMaterial(0, L"../Resources/Textures/T_TP_CH_Camo_006_003_D.png", flt::RawMaterial::TextureType::ALBEDO_OPACITY);
}

Mine::~Mine()
{

}
