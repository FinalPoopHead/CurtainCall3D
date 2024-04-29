#pragma once
#include "../FloaterGameEngine/include/Component.h"
#include "../FloaterGameEngine/include/CameraComponent.h"
#include "../FloaterGameEngine/include/GameObject.h"
#include "../FloaterGameEngine/include/Input.h"
#include "../FloaterRendererCommon/include/Transform.h"
#include "../FloaterMath/include/Vector3f.h"
#include "../FloaterMath/include/Quaternion.h"
#include "../FloaterMath/include/Common.h"
#include <cmath>
#include <algorithm>


class GlideComponent : public flt::ComponentBase<GlideComponent>
{
public:
	GlideComponent(flt::GameObject* gameObject) :
		ComponentBase<GlideComponent>(gameObject),
		MaxThrustSpeed(100.0f),
		MinThrustSpeed(10.0f),
		ThrustFactor(10.0f),
		DragFactor(0.5f),
		MinDrag(0.1f),
		RotationSpeed(5.0f),
		CurrentThrustSpeed(0.0f),
		TiltValue(0.0f),
		LerpValue(0.0f),
		velocity(0.0f, 0.0f, 0.0f),
		drag(0.0f),
		transform(gameObject->tr),
		cameraTransform(gameObject->tr)
	{

	}

	virtual void Update(float deltaSecond) override
	{
		flt::KeyData keyData = flt::GetKey(flt::KeyCode::mouseRelativePos);
		if (keyData)
		{
			transform.AddWorldRotation({ 0.0f, 1.0f, 0.0f }, keyData.x * 0.01f);

			flt::Vector3f euler = transform.GetLocalRotation().GetEuler();
			euler.x = std::clamp(euler.x + flt::RadToDeg(keyData.y * 0.01f), -89.0f, 89.0f);
			transform.SetRotation(flt::Quaternion(euler));
		}
		
		//printf("yaw : %.3f\n", flt::DegToRad(transform.GetLocalRotation().GetEuler().y));
		//printf("pitch : %.3f\n", flt::DegToRad(transform.GetLocalRotation().GetEuler().x));


		GlidingMovement(deltaSecond);
	}

private:
	void GlidingMovement(float deltaSecond)
	{
		float yaw = flt::DegToRad(transform.GetLocalRotation().GetEuler().y);
		float pitch = flt::DegToRad(transform.GetLocalRotation().GetEuler().x);

		float yawcos = cosf(yaw);
		float yawsin = sinf(yaw);
		float pitchcos = cosf(pitch);
		float pitchsin = sinf(pitch);

		float lookX = yawsin * pitchcos;
		float lookY = -pitchsin;
		float lookZ = yawcos * pitchcos;

		float hvel = sqrtf(velocity.x * velocity.x + velocity.z * velocity.z);
		float hlook = pitchcos;
		float sqrpitchcos = pitchcos * pitchcos;

		velocity.y += -0.08f + sqrpitchcos * 0.06f;

		if (velocity.y < 0 && hlook > 0)
		{
			float yacc = velocity.y * -0.1f * sqrpitchcos;
			velocity.y += yacc;
			velocity.x += lookX * yacc / hlook;
			velocity.z += lookZ * yacc / hlook;
		}
		if (pitch < 0)
		{
			float yacc = hvel * -pitchsin * 0.04f;
			velocity.y += yacc * 3.5f;
			velocity.x -= lookX * yacc / hlook;
			velocity.z -= lookZ * yacc / hlook;
		}
		if (hlook > 0)
		{
			velocity.x += (lookX / hlook * hvel - velocity.x) * 0.1f;
			velocity.z += (lookZ / hlook * hvel - velocity.z) * 0.1f;
		}

		velocity.x *= 0.99f;
		velocity.y *= 0.98f;
		velocity.z *= 0.99f;

		// 50 프레임인 fixedUpdate용에서 일반 Update용으로 사용하기 위해.
		transform.AddLocalPosition(velocity * 20.0f * deltaSecond);

		flt::Vector4f position = transform.GetWorldPosition();
		printf("positon : %.3f, %.3f %.3f | pitch : %.3f\n", position.x, position.y, position.z, pitch);
	}

private:
	float BaseSpeed = 30.0f;
	float MaxThrustSpeed;
	float MinThrustSpeed;
	float ThrustFactor;
	float DragFactor;
	float MinDrag;
	float RotationSpeed;
	float TiltStrength = 90.0f;
	const float LowPercent = 0.1;
	const float HighPercent = 1.0f;

	float CurrentThrustSpeed;
	float TiltValue, LerpValue;

	flt::Vector3f velocity;
	float drag;

	flt::Transform& transform;
	flt::Transform& cameraTransform;

	//Transform* CameraTransform; // Assuming Transform is defined elsewhere
	//Rigidbody* Rb; // Assuming Rigidbody is defined elsewhere
};
