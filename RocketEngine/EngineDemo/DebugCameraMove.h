#pragma once
#include "..\\RocketEngine\\RocketAPI.h"

namespace Rocket
{
	class Camera;
}


class DebugCameraMove : public Rocket::Component
{
public:
	DebugCameraMove();

public:
	virtual void Start() override;
	virtual void Update() override;

public:
	void OnMouseMove();

public:
	float moveSpeed;
	float accelRatio;
	float thisFrameSpeed;

private:
	Rocket::Camera* _camera;
};
