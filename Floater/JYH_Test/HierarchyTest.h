#pragma once
#include "../FloaterGameEngine/include/EngineMinimal.h"


class HierarchyTest : public flt::GameObject
{
public:
	HierarchyTest();

protected:
	virtual void Update(float deltaSeconds);

	virtual void OnEnable() override;
	virtual void OnDisable() override;
};

