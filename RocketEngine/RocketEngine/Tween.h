#pragma once
#include "DLLExporter.h"
#include <functional>
#include "MathHeader.h"

/*template class __declspec(dllexport) std::function<float(float)>;*/

namespace Rocket::Core
{
	class TweenTimer;
}

namespace Rocket
{
	enum eEasing
	{
		LINEAR,
		INBACK,
		OUTBACK,
		INOUTBACK,
		INBOUNCE,
		OUTBOUNCE,
		INOUTBOUNCE,
		INELASTIC,
		OUTELASTIC,
		INOUTELASTIC,
		INSINE,
		OUTSINE,
		INOUTSINE,
		INEXPO,
		OUTEXPO,
		INOUTEXPO,
		INCIRC,
		OUTCIRC,
		INOUTCIRC,
		INQUAD,
		OUTQUAD,
		INOUTQUAD,
		INCUBIC,
		OUTCUBIC,
		INOUTCUBIC,
		INQUART,
		OUTQUART,
		INOUTQUART,
		INQUINT,
		OUTQUINT,
		INOUTQUINT,

		LAST // 열거형의 끝
	};
}

namespace Rocket
{

	class ROCKET_API Tween
	{
	public:
		Tween();
		~Tween();
	public:
		/// save data
		int* dataI;
		float* dataF;
		Vector2* data2V;
		Vector3* data3V;
		Quaternion* dataQuat = nullptr;
		std::function<void()> onCompleteFunc = nullptr;

	public:
		/// Get data
		Tween& GetData(int* data);
		Tween& GetData(float* data);
		Tween& GetData(Vector2* data);
		Tween& GetData(Vector3* data);
		Tween& GetData(Quaternion* data);

		/// how to move

		// transform
		Tween& DoMove(const int& destination, const float duration);
		Tween& DoMove(const float& destination, const float duration);
		Tween& DoMove(Vector2& destination, const float duration);
		Tween& DoMove(const Vector3& destination, const float duration);
		Tween& DoMove(const Quaternion& destination, const float duration);

		// etc
		Tween& Delay(float delayTime);
		Tween& OnComplete(std::function<void()> func);

		/// easing functions

		Rocket::Core::TweenTimer* _timer;

		// easing data
		static std::function<float(float)> _easings[31];
		std::function<float(float)> _myEase;

		// easing 세팅 함수
		Tween& SetEase(eEasing type);

		// easing 수학 함수
		static float Linear(float x);

		static float InBack(float x);
		static float OutBack(float x);
		static float InOutBack(float x);

		static float InBounce(float x);
		static float OutBounce(float x);
		static float InOutBounce(float x);

		static float InElastic(float x);
		static float OutElastic(float x);
		static float InOutElastic(float x);

		static float InSine(float x);
		static float OutSine(float x);
		static float InOutSine(float x);

		static float InExpo(float x);
		static float OutExpo(float x);
		static float InOutExpo(float x);

		static float InCirc(float x);
		static float OutCirc(float x);
		static float InOutCirc(float x);
		// 제곱
		static float InQuad(float x);
		static float OutQuad(float x);
		static float InOutQuad(float x);
		// 세제곱
		static float InCubic(float x);
		static float OutCubic(float x);
		static float InOutCubic(float x);
		// 네제곱
		static float InQuart(float x);
		static float OutQuart(float x);
		static float InOutQuart(float x);
		// 다섯제곱
		static float InQuint(float x);
		static float OutQuint(float x);
		static float InOutQuint(float x);


		/// 연산자 오버로딩

	};
}
