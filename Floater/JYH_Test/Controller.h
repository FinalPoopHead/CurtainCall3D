#pragma once
#include "../FloaterGameEngine/include/Component.h"
#include "../FloaterGameEngine/include/GameObject.h"

namespace flt
{
	class Transform;
}

class Controller : public flt::Component<Controller>
{
public:
	Controller(flt::GameObject* gameObject);
	virtual void Update(float deltaSecond) override;

private:
	flt::Transform& transform;
	float _speed;
};

