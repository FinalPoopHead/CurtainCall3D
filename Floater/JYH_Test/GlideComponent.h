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


class GlideComponent : public flt::Component<GlideComponent>
{
public:
	GlideComponent(flt::GameObject* gameObject) :
		Component<GlideComponent>(gameObject),
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
		cameraTransform(gameObject->tr),
		initPos(0.0f, 200.0f, -400.0f, 1.0f)
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

		if (flt::GetKey(flt::KeyCode::spacebar))
		{
			_gameObject->tr.SetPosition(initPos);
			velocity = flt::Vector3f(0.0f, 0.0f, 0.0f);
			_gameObject->tr.SetRotation(10.0f, 0.0f, 0.0f);
		}


	}

	virtual void FixedUpdate() override
	{
		flt::Vector4f position = transform.GetWorldPosition();
		//printf("positon : %.3f, %.3f %.3f\n", position.x, position.y, position.z);
		GlidingMovement();
	}

private:
	void GlidingMovement()
	{
		float p1 = 0.1f;

		float yaw = flt::DegToRad(transform.GetLocalRotation().GetEuler().y);
		float pitch = flt::DegToRad(transform.GetLocalRotation().GetEuler().x);

		float yawcos = cosf(yaw);
		float yawsin = sinf(yaw);
		float pitchcos = cosf(pitch);
		float pitchsin = sinf(pitch);

		float lookX = yawsin * pitchcos;
		float lookY = -pitchsin;
		float lookZ = yawcos * pitchcos;

		float hSpeed = sqrtf(velocity.x * velocity.x + velocity.z * velocity.z);
		float hlook = pitchcos;
		float sqrpitchcos = pitchcos * pitchcos;

		// 기본중력 적용과 방향에 따른 양력 적용
		velocity.y += -0.08f + sqrpitchcos * 0.06f;

		if (pitchcos < 0)
		{
			ASSERT(false, "pitchcos < 0");
		}
		//
		if (velocity.y < 0 && hlook > 0)
		{
			float accY = velocity.y * -0.1f * sqrpitchcos;
			velocity.y += accY;
			velocity.x += lookX * accY / hlook;
			velocity.z += lookZ * accY / hlook;
		}
		// 위를 바라보고 있을 때
		if (pitch < 0)
		{
			float accY = hSpeed * lookY * 0.04f;
			velocity.y += accY * 3.5f;
			velocity.x -= lookX * accY / hlook;
			velocity.z -= lookZ * accY / hlook;
		}
		if (hlook > 0)
		{
			velocity.x += (lookX / hlook * hSpeed - velocity.x) * 0.1f;
			velocity.z += (lookZ / hlook * hSpeed - velocity.z) * 0.1f;
		}

		// drag 적용
		velocity.x *= 0.99f;
		velocity.y *= 0.98f;
		velocity.z *= 0.99f;

		////속도 느리게 테스트
		//velocity.x *= 0.8f;
		//velocity.y *= 0.8f;
		//velocity.z *= 0.8f;

		transform.AddLocalPosition(velocity);

		flt::Vector4f position = transform.GetWorldPosition();

		float speed = velocity.Norm();
		printf("positon : %.3f, %.3f %.3f | pitch : %.3f | sqrpitchcos : %.3f | pitchcos : %.3f\n", 
			position.x, position.y, position.z, 
			pitch, 
			sqrpitchcos,
			pitchcos);
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
	const float LowPercent = 0.1f;
	const float HighPercent = 1.0f;

	float CurrentThrustSpeed;
	float TiltValue, LerpValue;

	flt::Vector3f velocity;
	float drag;

	flt::Transform& transform;
	flt::Transform& cameraTransform;

	flt::Vector4f initPos;

	//Transform* CameraTransform; // Assuming Transform is defined elsewhere
	//Rigidbody* Rb; // Assuming Rigidbody is defined elsewhere
};
