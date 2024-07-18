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
	if (flt::GetKeyDown(flt::KeyCode::lCtrl))
	{
		_shaker->Impack(20.f, .2f, { 1.0f, 1.0f });
	}
}
