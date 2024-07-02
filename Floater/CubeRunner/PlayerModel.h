#pragma once
#include "../FloaterGameEngine/include/EngineMinimal.h"

class PlayerModel : public flt::GameObject
{
public:
	PlayerModel();

	void PlayWalk();
	void PlayIdle();
	void SetAlbedoPath(std::wstring path);
private:
	flt::RendererComponent* _renderer;
};
