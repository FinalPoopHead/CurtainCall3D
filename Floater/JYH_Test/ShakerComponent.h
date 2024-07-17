#pragma once
#include "../FloaterGameEngine/include/EngineMinimal.h"
#include <functional>

class Shaker : public flt::Component<Shaker>
{
public:
	Shaker();
	void Impack(float power = 1.0f, float duration = 0.5f);
	void DoIt(float amplitude, float frequency, float duration);

protected:
	void PreUpdate(float deltaSecond) override;
	float ImpactFunc(float t);

private:
	bool _doShake;
	float _duration;
	float _elapsed;
	float _diffX;
	float _diffY;
	float _diffRad;
	float _power;

	std::function<float(float)> _noiseFunc;
	std::function<float(float)> _impactFunc;
};

