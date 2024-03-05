#pragma once
#include "..\\RocketEngine\\RocketAPI.h"

class PlayerController : public Rocket::Component
{
public:
	PlayerController();
	~PlayerController();

	void Update() override;

private:
	float _moveSpeed;
};

