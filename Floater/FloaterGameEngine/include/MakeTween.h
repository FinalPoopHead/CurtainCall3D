#pragma once
#include "./internal/Scene.h"
#include "../../FloaterUtil/include/FLTween.h"
#include "../../FloaterUtil/include/OwningPtr.h"
#include "../../FloaterRendererCommon/include/Transform.h"
#include "../../FloaterMath/include/Ease.h"
#include <type_traits>

namespace flt
{

	void StartTween(IFLTween* tween);
	void StopTween(IFLTween* tween);
	void ReleaseTween(IFLTween* tween);
	bool IsActiveTween(IFLTween* tween);

	namespace internal
	{
		void RegistTween(IFLTween* tween);
		void RegistPosTween(FLTween<Vector4f>* tween, Transform* tr);
		void RegistScaleTween(FLTween<Vector4f>* tween, Transform* tr);
		void RegistRotTween(FLTween<Quaternion>* tween, Transform* tr);

		flt::Transform TrLerpFunc(const Transform& a, const Transform& b, float t);
		flt::Quaternion QuatLerpFunc(const Quaternion& a, const Quaternion& b, float t);

		template<typename T>
		struct FLTweenDestructor
		{
			void operator()(FLTween<T>* tween)
			{
				if (IsActiveTween(tween))
				{
					tween->onFinalize([tween]() { ReleaseTween(tween); });
				}
				else
				{
					ReleaseTween(tween);
				}
			}
		};
	}

	template<typename T>
	using TweenPtr = OwningPtr<FLTween<T>, internal::FLTweenDestructor<T>>;

	template<typename T>
	TweenPtr<T> MakeTween(T target)
	{
		FLTween<T>* tween = new FLTween<T>(target);
		internal::RegistTween(tween);
		return TweenPtr<T>{tween};
	}

	TweenPtr<Vector4f> MakePosTween(Transform* target);
	TweenPtr<Vector4f> MakeScaleTween(Transform* target);
	TweenPtr<Quaternion> MakeRotTween(Transform* target);
	//FLTween<Transform>* MakeTransformTween(Transform* target);

	template<typename T>
	void StartTween(TweenPtr<T>& tween)
	{
		StartTween(tween.get());
	}

	template<typename T>
	void StopTween(TweenPtr<T>& tween)
	{
		StopTween(tween.get());
	}

	template<typename T>
	void ReleaseTween(TweenPtr<T>& tween)
	{
		ReleaseTween(tween.get());
	}

	template<typename T>
	bool IsActiveTween(TweenPtr<T>& tween)
	{
		return IsActiveTween(tween.get());
	}
}
