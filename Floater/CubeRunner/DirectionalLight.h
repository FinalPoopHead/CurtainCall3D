#pragma once
#include "../FloaterGameEngine/include/EngineMinimal.h"


class DirectionalLight : public flt::GameObject
{
public:
	DirectionalLight();

private:
	flt::DirectionalLight* _lightComponent;
	flt::Vector3f _direction;
};
