#include "CubeController.h"

CubeController::CubeController()
	: _rotateTime(1.0f)
{

}

CubeController::~CubeController()
{

}

void CubeController::Update(float deltaSecond)
{
// 	static float elapsedTime = 0.0f;
// 
// 	flt::Quaternion q = _gameObject->tr.GetLocalRotation();
// 	flt::Quaternion q2;
// 	q2.SetEuler({ 0.0f, 0.0f, 90.0f });
// 	q2 = q2 * q;
// 
// 	flt::Quaternion::Slerp(q, q2, elapsedTime);
// 	elapsedTime += deltaSecond;

	_gameObject->tr.AddLocalRotation({ 1.0f,0.0f,0.0f }, _rotateTime * deltaSecond);
	//_gameObject->tr.SetRotation({ 1.0f,0.0f,0.0f }, _rotateTime * deltaSecond);
}
