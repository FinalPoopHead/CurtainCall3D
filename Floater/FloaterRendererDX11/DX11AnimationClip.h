#pragma once
#include "../FloaterRendererCommon/include/RawAnimationClip.h"
#include "../FloaterMath/include/Vector3f.h"
#include "../FloaterMath/include/Quaternion.h"

namespace flt
{
	struct DX11AnimationClip
	{
#pragma region Nasted
		using KeyPosition = RawAnimationClip::KeyPosition;
		using KeyRotation = RawAnimationClip::KeyRotation;
		using KeyScale = RawAnimationClip::KeyScale;
#pragma endregion
		DX11AnimationClip() : startTime(0.0f), endTime(0.0f), keyPosition(), keyRotation(), keyScale(), positionIndex(0), rotationIndex(0), scaleIndex(0) {}
		DX11AnimationClip& operator=(const RawAnimationClip& other)
		{
			startTime = other.startTime;
			endTime = other.endTime;

			keyPosition = other.keyPosition;
			keyRotation = other.keyRotation;
			keyScale = other.keyScale;

			return *this;
		}

		void GetPosition(float time, Vector3f* outPos);
		void GetRotation(float time, Quaternion* outRot);
		void GetScale(float time, Vector3f* outScl);

		float startTime;
		float endTime;

		std::vector<KeyPosition> keyPosition;
		std::vector<KeyRotation> keyRotation;
		std::vector<KeyScale> keyScale;

		int positionIndex;
		int rotationIndex;
		int scaleIndex;
	};
}


