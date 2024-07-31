#include "ShakeObject.h"
#include "ShakeComponent.h"
#include "../FloaterGameEngine/include/Input.h"

ShakeObject::ShakeObject()
	: _shaker(AddComponent<ShakeComponent>(true))
	, _camera(AddComponent<flt::CameraComponent>(true))
{
}

void ShakeObject::PostUpdate(float deltaSecond)
{

}
