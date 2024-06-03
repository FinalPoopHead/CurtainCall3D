#pragma once
#include "../FloaterGameEngine/include/EngineMinimal.h"

class RotateComponent : public flt::Component<RotateComponent>
{
public:
	RotateComponent(flt::GameObject* gameObject) : 
		flt::Component<RotateComponent>(gameObject),
		speed(40.0f)
	{
	}

	virtual void Update(float deltaSecond)
	{
		flt::Transform& transform = _gameObject->tr;

		transform.AddLocalRotation({0.0f, 1.0f, 0.0f}, speed * deltaSecond);
	}
private:
	float speed;
};
