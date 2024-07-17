#include "./include/MakeTween.h"
#include "./include/internal/GameEngine.h"


namespace
{
	flt::GameEngine* g_engine = flt::GameEngine::Instance();
}

void flt::internal::RegistTween(IFLTween* tween)
{
	g_engine->GetCurrentScene()->AddTween(tween);
}

void flt::internal::RegistPosTween(FLTween<Vector4f>* tween, Transform* tr)
{
	g_engine->GetCurrentScene()->AddPosTween(tween, tr);
}

void flt::internal::RegistScaleTween(FLTween<Vector4f>* tween, Transform* tr)
{
	g_engine->GetCurrentScene()->AddScaleTween(tween, tr);
}

void flt::internal::RegistRotTween(FLTween<Quaternion>* tween, Transform* tr)
{
	g_engine->GetCurrentScene()->AddRotTween(tween, tr);
}

flt::Transform flt::internal::TrLerpFunc(const Transform& a, const Transform& b, float t)
{
	flt::Transform result;
	flt::Vector4f pos = flt::Vector4f::Lerp(a.GetLocalPosition(), b.GetLocalPosition(), t);
	flt::Quaternion rot = flt::Quaternion::Slerp(a.GetLocalRotation(), b.GetLocalRotation(), t);
	flt::Vector4f scale = flt::Vector4f::Lerp(a.GetLocalScale(), b.GetLocalScale(), t);

	result.SetPosition(pos);
	result.SetRotation(rot);
	result.SetScale(scale);

	return result;
}

flt::Quaternion flt::internal::QuatLerpFunc(const Quaternion& a, const Quaternion& b, float t)
{
	flt::Quaternion result = flt::Quaternion::Slerp(a, b, t);
	return result;
}

flt::TweenPtr<flt::Vector4f> flt::MakePosTween(Transform* target)
{
	FLTween<flt::Vector4f>* tween = new FLTween<flt::Vector4f>(target->GetLocalPosition(), flt::defaultLerp<flt::Vector4f>);
	internal::RegistPosTween(tween, target);
	return flt::TweenPtr<flt::Vector4f>{tween};
}

flt::TweenPtr<flt::Vector4f> flt::MakeScaleTween(Transform* target)
{
	FLTween<flt::Vector4f>* tween = new FLTween<flt::Vector4f>(target->GetLocalScale(), flt::defaultLerp<flt::Vector4f>);
	internal::RegistScaleTween(tween, target);
	return flt::TweenPtr<flt::Vector4f>{tween};
}

flt::TweenPtr<flt::Quaternion> flt::MakeRotTween(Transform* target)
{
	FLTween<flt::Quaternion>* tween = new FLTween<flt::Quaternion>(target->GetLocalRotation(), internal::QuatLerpFunc);
	internal::RegistRotTween(tween, target);
	return flt::TweenPtr<flt::Quaternion>{tween};
}

void flt::ReleaseTween(IFLTween* tween)
{
	g_engine->GetCurrentScene()->ReleaseTween(tween);
}

bool flt::IsActiveTween(IFLTween* tween)
{
	return g_engine->GetCurrentScene()->IsActiveTween(tween);
}

void flt::StartTween(IFLTween* tween)
{
	g_engine->GetCurrentScene()->StartTween(tween);
}

void flt::StopTween(IFLTween* tween)
{
	g_engine->GetCurrentScene()->StopTween(tween);
}

//template<>
//flt::FLTween<flt::Transform*>* flt::MakeTween(flt::Transform* target)
//{
//	FLTween<Transform*>* tween = new FLTween<Transform*>(target, internal::TrLerpFunc);
//	internal::RegistTween(tween);
//	return tween;
//}
//
//template<>
//flt::FLTween<flt::Quaternion*>* flt::MakeTween(flt::Quaternion* target)
//{
//	FLTween<Quaternion*>* tween = new FLTween<Quaternion*>(target, internal::QuatLerpFunc);
//	internal::RegistTween(tween);
//	return tween;
//}
