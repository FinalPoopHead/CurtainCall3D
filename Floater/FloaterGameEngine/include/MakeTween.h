#pragma once
#include "./internal/Scene.h"
#include "../../FloaterUtil/include/FLTween.h"
#include "../../FloaterUtil/include/OwningPtr.h"
#include "../../FloaterRendererCommon/include/Transform.h"
#include "../../FloaterMath/include/Ease.h"
#include <type_traits>

namespace flt
{
	namespace internal
	{
		void RegistTween(IFLTween* tween);
		void RegistPosTween(FLTween<Vector4f>* tween, Transform* tr);
		void RegistScaleTween(FLTween<Vector4f>* tween, Transform* tr);
		void RegistRotTween(FLTween<Quaternion>* tween, Transform* tr);

		flt::Transform TrLerpFunc (const Transform& a, const Transform& b, float t);
		flt::Quaternion QuatLerpFunc(const Quaternion& a, const Quaternion& b, float t);
	}

	//template<typename T>
	//FLTween<T>* MakeTween()
	//{
	//	FLTween<T>* tween = new FLTween<T>();
	//	RegistTween(tween);
	//	return tween;
	//}

	FLTween<Vector4f>* MakePosTween(Transform* target);
	FLTween<Vector4f>* MakeScaleTween(Transform* target);
	FLTween<Quaternion>* MakeRotTween(Transform* target);
	//FLTween<Transform>* MakeTransformTween(Transform* target);

	void ReleaseTween(IFLTween* tween);

	void StartTween(IFLTween* tween);
	void StopTween(IFLTween* tween);

	//struct TweenDestructor
	//{
	//	constexpr void operator()(IFLTween* ptr) const noexcept
	//	{
	//		ReleaseTween(ptr);
	//	}
	//};


	template<typename T>
	FLTween<T>* MakeTween(T target)
	{
		FLTween<T>* tween = new FLTween<T>(target, flt::defaultLerp<std::remove_pointer_t<std::remove_reference_t<T>>>);
		internal::RegistTween(tween);
		return tween;
	}

	//template<>
	//FLTween<flt::Transform*>* MakeTween<flt::Transform*>(flt::Transform* target);

	//template<>
	//FLTween<flt::Quaternion*>* MakeTween<flt::Quaternion*>(flt::Quaternion* target);
}
