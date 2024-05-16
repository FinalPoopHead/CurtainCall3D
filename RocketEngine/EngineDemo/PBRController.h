#pragma once
#include "..\\RocketEngine\\RocketAPI.h"

class PBRController : public Rocket::Component
{
public:
	PBRController();
	~PBRController();

	void Start() override;
	void Update() override;

private:
	float _delta;
	float _metallic;
	float _roughness;
	Rocket::MeshRenderer* _meshRenderer;
};

