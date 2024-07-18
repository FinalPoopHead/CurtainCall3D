#include "Controller.h"
#include "../FloaterGameEngine/include/Input.h"
#include <algorithm>


Controller::Controller() :
	_speed(10.0f)
{

}

void Controller::Update(float deltaSecond)
{
	flt::Transform& transform = _gameObject->tr;
	flt::KeyData rButtonDown = flt::GetKey(flt::KeyCode::mouseRButton);
	if (rButtonDown)
	{
		// 카메라 방향
		flt::KeyData keyData = flt::GetKey(flt::KeyCode::mouseRelativePos);
		if (keyData)
		{
			transform.AddWorldRotation({ 0.0f, 1.0f, 0.0f }, keyData.x * 0.01f);

			flt::Vector3f euler = transform.GetLocalRotation().GetEuler();
			euler.x = std::clamp(euler.x + flt::RadToDeg(keyData.y * 0.01f), -89.0f, 89.0f);
			transform.SetRotation(flt::Quaternion(euler));
		}

		// 오브젝트 이동

		float speed = _speed;
		if (flt::GetKey(flt::KeyCode::lShift))
		{
			speed *= 10.0f;
		}

		if (flt::GetKey(flt::KeyCode::lCtrl))
		{
			speed *= 0.1f;
		}

		if (flt::GetKey(flt::KeyCode::w))
		{
			//transform.AddWorldPosition();
			transform.AddLocalPosition(transform.Forward() * speed * deltaSecond);
		}

		if (flt::GetKey(flt::KeyCode::s))
		{
			transform.AddWorldPosition(-transform.Forward() * speed * deltaSecond);
		}

		if (flt::GetKey(flt::KeyCode::a))
		{
			transform.AddWorldPosition(-transform.Right() * speed * deltaSecond);
		}

		if (flt::GetKey(flt::KeyCode::d))
		{
			transform.AddWorldPosition(transform.Right() * speed * deltaSecond);
		}

		if (flt::GetKey(flt::KeyCode::e))
		{
			transform.AddWorldPosition(flt::Transform::WorldUp() * speed * deltaSecond);
		}

		if (flt::GetKey(flt::KeyCode::q))
		{
			transform.AddWorldPosition(-flt::Transform::WorldUp() * speed * deltaSecond);
		}
	}
}
