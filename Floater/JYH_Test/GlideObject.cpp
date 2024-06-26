﻿#include "GlideObject.h"
#include "../FloaterGameEngine/include/internal/BuiltinComponent.h"
#include "../FloaterGameEngine/include/internal/GameEngine.h"
#include "../FloaterGameEngine/include/Input.h"
#include "GlideComponent.h"
#include "Controller.h"

GlideObject::GlideObject()
	: flt::GameObject()
{
	AddComponent<flt::CameraComponent>(true);
	AddComponent<Controller>(true);
	//AddComponent<GlideComponent>();
	flt::BoxColliderComponent* boxCollider = AddComponent<flt::BoxColliderComponent>(true);
	boxCollider->UseKinematic(true);
}

GlideObject::~GlideObject()
{

}

void GlideObject::Update(float deltaTime) 
{

}
