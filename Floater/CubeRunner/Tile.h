#pragma once
#include "../FloaterGameEngine/include/EngineMinimal.h"

class Tile : public flt::GameObject
{
public:
	Tile();
	virtual ~Tile();

	virtual void OnCreate() override;
	virtual void OnEnable() override;
	virtual void OnDisable() override;
	virtual void OnDestroy() override;
};
