#pragma once
#include "../FloaterGameEngine/include/EngineMinimal.h"

enum class eCubeTexture
{
	NEUTRAL
	, GARBAGE
};

/// <summary>
/// 일반 큐브.
/// 맵의 끝에 도달하기 전에 제거해야 한다.
/// 제거하지 못하면 맵의 끝에 도달할 때마다 체력이 1씩 감소한다.
/// </summary>
class NormalCube : public flt::GameObject
{
public:
	NormalCube();
	~NormalCube();

	void SetTexture(eCubeTexture textureType);

private:
	flt::RendererComponent* _renderer;
	eCubeTexture _textureType;
};
