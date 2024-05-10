#pragma once
#include "../FloaterGameEngine/include/GameObject.h"
#include "../FloaterGameEngine/include/BuiltinComponent.h"
#include "../FloaterGameEngine/include/GameEngine.h"
#include "../FloaterGameEngine/include/Input.h"
#include "GlideComponent.h"

class GlideObject : public flt::GameObject 
{
public:
	GlideObject();
	~GlideObject();

	void Update(float deltaTime) override;
};

GlideObject::GlideObject()
	: flt::GameObject()
{
	AddComponent<flt::CameraComponent>();
	AddComponent<GlideComponent>();
	AddComponent<flt::BoxColliderComponent>();
}

GlideObject::~GlideObject()
{

}

void GlideObject::Update(float deltaTime)
{
	flt::KeyData keyData = flt::GetKey(flt::KeyCode::spacebar);
	if (keyData)
	{
		auto boxCollider = GetComponent<flt::BoxColliderComponent>();
		boxCollider->SetSize(flt::Vector3f(1.0f, 1.0f, 1.0f));
	}
}
