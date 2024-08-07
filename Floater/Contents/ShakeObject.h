#pragma once
#include "../FloaterGameEngine/include/EngineMinimal.h"

namespace flt
{
	class CameraComponent;
}

class ShakeComponent;

class ShakeObject : public flt::GameObject
{
public:
	ShakeObject();

protected:
	virtual void PostUpdate(float deltaSecond) override;

protected:
	ShakeComponent* _shaker;
	flt::CameraComponent* _camera;
};

