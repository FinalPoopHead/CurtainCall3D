#pragma once
#include "../FloaterGameEngine/include/EngineMinimal.h"

/// <summary>
/// 어드밴티지 큐브.
/// 맵의 끝에 도달하기 전에 제거해야한다.
/// 제거하면 해당 위치를 중심으로 3x3 영역에 추가적인 지뢰를 설치한다.
/// 맵의 끝에 도달하면 갯수만큼 체력을 1씩 감소시킨다.
/// </summary>
class AdvantageCube : public flt::GameObject
{
public:
	AdvantageCube();
	~AdvantageCube();
};
