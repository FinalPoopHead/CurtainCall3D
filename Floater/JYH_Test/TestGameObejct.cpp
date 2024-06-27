#include "TestGameObejct.h"
#include "../FloaterRendererCommon/include/ModelLoader.h"
#include "../FloaterGameEngine/include/EngineMinimal.h"
#include "../FloaterGameEngine/include/Input.h"
#include <iostream>


TestGameObejct::TestGameObejct()
	: flt::GameObject()
{
	flt::RendererComponent* renderer = AddComponent<flt::RendererComponent>(true);
	std::wstring filePath = L"..\\Resources\\Models\\A_TP_CH_Sprint_F.fbx";
	renderer->SetFilePath(filePath);
	renderer->SetMaterial(0, L"..\\Resources\\Textures\\T_TP_CH_Camo_001_006_D.png", flt::RawMaterial::TextureType::ALBEDO_OPACITY);

	flt::BoxColliderComponent* boxCollider = AddComponent<flt::BoxColliderComponent>(true);
	boxCollider->UseKinematic(true);
	boxCollider->SetSize({ 100.0f, 100.0f, 100.0f });

	flt::SoundComponent* sound = AddComponent<flt::SoundComponent>(true);
	sound->AddSound(L"..\\Resources\\sound\\door.mp3");
}

TestGameObejct::~TestGameObejct()
{

}

void TestGameObejct::Update(float deltaTime)
{
	flt::GamePadState outState;
	bool ret = GetGamePadState(0, &outState);

	//_elapsedTime += deltaTime;
	//static float enableTime = 2.0f;
	//static float disableTime = 1.0f;

	//if (_elapsedTime > enableTime)
	//{
	//	flt::RendererComponent* renderer = GetComponent<flt::RendererComponent>();
	//	renderer->Enable();
	//	_elapsedTime = 0.0f;
	//}
	//else if (_elapsedTime > disableTime)
	//{
	//	flt::RendererComponent* renderer = GetComponent<flt::RendererComponent>();
	//	//renderer->Disable();
	//	this->Destroy();
	//}

	flt::KeyData keyData = GetKey(flt::KeyCode::spacebar);
	if (keyData)
	{
		flt::SoundComponent* sound = GetComponent<flt::SoundComponent>();
		if (sound)
		{
			sound->Play(0);
		}
	}

	keyData = GetKey(flt::KeyCode::lCtrl);
	if (keyData)
	{
		flt::SoundComponent* sound = GetComponent<flt::SoundComponent>();
		if (sound)
		{
			sound->Stop(0);
		}
	}

	keyData = GetKey(flt::KeyCode::lAlt);
	if (keyData)
	{
		flt::SoundComponent* sound = GetComponent<flt::SoundComponent>();
		if (sound)
		{
			sound->Pause(0);
		}
	}

	keyData = GetKey(flt::KeyCode::lShift);
	if (keyData)
	{
		std::vector<std::wstring> animList = GetComponent<flt::RendererComponent>()->GetAnimaionList();

		for (auto& anim : animList)
		{
			std::wcout << anim << std::endl;
		}
	}

	keyData = GetKey(flt::KeyCode::key1);
	if (keyData)
	{
		GetComponent<flt::RendererComponent>()->PlayAnimation(0, true);
	}

	keyData = GetKey(flt::KeyCode::key2);
	if (keyData)
	{
		GetComponent<flt::RendererComponent>()->PlayAnimation(1, true);
	}

	keyData = GetKeyDown(flt::KeyCode::f);
	if (keyData)
	{
		std::cout << "KeyDown: f" << std::endl;
	}

	keyData = GetKeyUp(flt::KeyCode::f);
	if (keyData)
	{
		std::cout << "KeyUp: f" << std::endl;
	}
}
