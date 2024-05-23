#include "GlideObject.h"
#include "../FloaterGameEngine/include/BuiltinComponent.h"
#include "../FloaterGameEngine/include/GameEngine.h"
#include "../FloaterGameEngine/include/Input.h"
#include "GlideComponent.h"
#include "Controller.h"

GlideObject::GlideObject()
	: flt::GameObject()
{
	AddComponent<flt::CameraComponent>();
	AddComponent<Controller>();
	//AddComponent<GlideComponent>();
	flt::BoxColliderComponent* boxCollider = AddComponent<flt::BoxColliderComponent>();
	boxCollider->UseKinematic(false);
}

GlideObject::~GlideObject()
{

}

void GlideObject::Update(float deltaTime) 
{

}
