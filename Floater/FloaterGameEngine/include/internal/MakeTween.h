#pragma once
#include "Scene.h"
#include "../../../FloaterUtil/include/FLTween.h"
#include "../../../FloaterRendererCommon/include/Transform.h"

namespace flt
{
	namespace internal
	{
		void RegistTween(IFLTween* tween);

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

	template<typename T>
	FLTween<T>* MakeTween(T target)
	{
		FLTween<T>* tween = new FLTween<T>(target);
		internal::RegistTween(tween);
		return tween;
	}

	template<>
	FLTween<flt::Transform&>* MakeTween(flt::Transform& target)
	{
		FLTween<Transform&>* tween = new FLTween<Transform&>(target, internal::TrLerpFunc);
		internal::RegistTween(tween);
		return tween;
	}

	template<>
	FLTween<flt::Quaternion&>* MakeTween(flt::Quaternion& target)
	{
		FLTween<Quaternion&>* tween = new FLTween<Quaternion&>(target, internal::QuatLerpFunc);
		internal::RegistTween(tween);
		return tween;
	}
}
