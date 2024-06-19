#pragma once
#include "../FloaterGameEngine/include/EngineMinimal.h"

class DetonatedMine : public flt::GameObject
{
public:
	DetonatedMine();
	~DetonatedMine();

public:
	virtual void OnEnable() override;
	virtual void OnDisable() override;

private:
	flt::GameObject* _plane;
	flt::GameObject* _cone;
};
