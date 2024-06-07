#pragma once
#include "../FloaterGameEngine/include/EngineMinimal.h"

/// <summary>
/// 다크 큐브.
/// 제거하면 안된다.
/// 제거하면 체력이 1 감소한다.
/// </summary>
class DarkCube : public flt::GameObject
{
public:
	DarkCube();
	~DarkCube();
};
