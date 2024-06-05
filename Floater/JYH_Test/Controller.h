#pragma once
#include "../FloaterGameEngine/include/internal/Component.h"
#include "../FloaterGameEngine/include/internal/GameObject.h"

namespace flt
{
	class Transform;
}

class Controller : public flt::Component<Controller>
{
public:
	Controller();
	virtual void Update(float deltaSecond) override;

private:
	float _speed;
};

