#pragma once
#include "../FloaterGameEngine/include/EngineMinimal.h"
#include "../FloaterUtil/include/SparseSet.h"
#include <functional>

float ImpactFunc(float t);

class ShakeComponent : public flt::Component<ShakeComponent>
{
	class Shaker
	{
	public:
		Shaker(float power, float duration, flt::Vector2f dir, std::function<float(float)> shakeFunction);
	
		bool Shake(float deltaSecond, flt::Transform& transform);
	private:
		float _power;
		float _duration;
		float _elapsed;
		float _offset;
		flt::Vector2f _dir;
		std::function<float(float)> _shakeFunction;
	};

public:
	ShakeComponent();
	void Impack(float power = 1.0f, float duration = 0.5f, flt::Vector2f dir = {1.0f, 1.0f}, std::function<float(float)> ShakeFunc = ImpactFunc);
	void DoIt(float amplitude, float frequency, float duration);

protected:
	void PreUpdate(float deltaSecond) override;

private:
	flt::SparseSet<Shaker> _shakers;
};
