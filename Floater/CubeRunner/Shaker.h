#pragma once
#include "../FloaterGameEngine/include/EngineMinimal.h"

class Shaker : public flt::Component<Shaker>
{
public:
	void Impack(float power = 1.0f, float duration = 0.5f);
	void Sway(float power = 1.0f);
private:
	float screenX;
	float screenY;
	flt::Quaternion rot;
	float zoom;
	float duration;
};

