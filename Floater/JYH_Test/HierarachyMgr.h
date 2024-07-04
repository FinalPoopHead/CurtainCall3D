#pragma once
#include "../FloaterGameEngine/include/EngineMinimal.h"
#include "HierarchyTest.h"


class HierarachyMgr : public flt::GameObject
{
public:
	HierarachyMgr();

	void PreUpdate(float deltaSeconds) override;
	void PostUpdate(float deltaSeconds) override;

private:
	HierarchyTest* _hierarchyTestArr[10];
};

