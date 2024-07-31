#pragma once
#include "../FloaterGameEngine/include/EngineMinimal.h"

class Mine : public flt::GameObject
{
public:
	Mine();
	~Mine();

public:
	virtual void OnEnable() override;
	virtual void OnDisable() override;

private:
	flt::GameObject* _plane;
	flt::GameObject* _cone;
};
