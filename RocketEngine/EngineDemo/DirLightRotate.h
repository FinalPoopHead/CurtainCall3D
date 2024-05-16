#pragma once
#include "../RocketEngine/RocketAPI.h"

class DirLightRotate : public Rocket::Component
{
public:
	DirLightRotate();

public:
	virtual void Start() override;
	virtual void Update() override;

public:
	float rotateSpeed;
};

