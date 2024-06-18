#pragma once
#include "../FloaterGameEngine/include/EngineMinimal.h"

class AdvantageMine : public flt::GameObject
{
public:
	AdvantageMine();
	~AdvantageMine();

public:
	virtual void OnEnable() override;
	virtual void OnDisable() override;

private:
	flt::GameObject* _plane;
	flt::GameObject* _cone;
};

