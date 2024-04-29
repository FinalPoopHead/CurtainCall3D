#pragma once
#include "../FloaterGameEngine/include/GameObject.h"
#include "../FloaterGameEngine/include/CameraComponent.h"
#include "GlideComponent.h"

class GlideObject : public flt::GameObject 
{
public:
	GlideObject();
	~GlideObject();
};

GlideObject::GlideObject()
	: flt::GameObject()
{
	AddComponent<flt::CameraComponent>();
	AddComponent<GlideComponent>();
}

GlideObject::~GlideObject()
{

}
