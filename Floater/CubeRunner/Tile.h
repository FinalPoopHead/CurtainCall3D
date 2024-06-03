#pragma once
#include "../FloaterGameEngine/include/EngineMinimal.h"

class Tile : public flt::GameObject
{
public:

	virtual void OnEnable() override;
	virtual void OnDisable() override;
	virtual void OnDestroy() override;
};
