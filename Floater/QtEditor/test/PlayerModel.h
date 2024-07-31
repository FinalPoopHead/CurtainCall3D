#pragma once
#include "../FloaterGameEngine/include/EngineMinimal.h"

class PlayerModel : public flt::GameObject
{
public:
	PlayerModel();

	void PlayAnimation(int index, bool isLoop);
	void PlayWalk();
	void PlayIdle();
	void SetAlbedoPath(std::wstring path);
private:
	flt::RendererComponent* _renderer;
};
