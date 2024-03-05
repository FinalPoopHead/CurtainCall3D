#include "Tween.h"
#include "TimeSystem.h"
#include "TweenTimer.h"
#include "assert.h"
#include <algorithm>
#include <math.h>
#include <iostream>

constexpr float PI = DirectX::XM_PI;

namespace Rocket
{
	Tween::Tween()
		: dataF(nullptr), data2V(nullptr), data3V(nullptr)
	{
		_timer = new Rocket::Core::TweenTimer();
	}

	Tween::~Tween()
	{
		delete _timer;
	}

	Tween& Tween::GetData(float* data)
	{
		this->dataF = data;
		return *this;
	}

	Tween& Tween::GetData(Vector2* data)
	{
		this->data2V = data;
		return *this;
	}

	Tween& Tween::GetData(Vector3* data)
	{
		this->data3V = data;
		return *this;
	}

	Tween& Tween::GetData(Quaternion* data)
	{
		this->dataQuat = data;
		return *this;
	}

	Tween& Tween::GetData(int* data)
	{
		this->dataI = data;
		return *this;
	}

	Tween& Tween::DoMove(const float& destination, const float duration)
	{
		_timer->Start();
		_timer->_duration = duration;

		float distance = destination - *this->dataF;
		float start = *this->dataF;
		_timer->_play = [=]()
		{
			*this->dataF = start + (distance*(_myEase(_timer->_time / duration)));
		};
		return *this;
	}

	Tween& Tween::DoMove(Vector2& destination, const float duration)
	{
		_timer->Start();
		_timer->_duration = duration;

		Vector2 distance;
		//distance._x = destination._x - this->data2V->_x;
		//distance._y = destination._y - this->data2V->_y;

		distance = destination - *this->data2V;

		Vector2 start = *this->data2V;

		_timer->_play = [=]()
		{
			this->data2V->x = start.x + (distance.x * (_myEase(_timer->_time / duration)));
			this->data2V->y = start.y + (distance.y * (_myEase(_timer->_time / duration)));
		};
		return *this;
	}

	Tween& Tween::DoMove(const Vector3& destination, const float duration)
	{
		_timer->Start();
		_timer->_duration = duration;

		Vector3 distance;
		distance.x = destination.x - this->data3V->x;
		distance.y = destination.y - this->data3V->y;
		distance.z = destination.z - this->data3V->z;

		Vector3 start = *this->data3V;

		_timer->_play = [=]()
		{
			this->data3V->x = start.x + (distance.x * (_myEase(_timer->_time / duration)));
			this->data3V->y = start.y + (distance.y * (_myEase(_timer->_time / duration)));
			this->data3V->z = start.z + (distance.z * (_myEase(_timer->_time / duration)));
		};
		return *this;
	}

	Tween& Tween::DoMove(const Quaternion& destination, const float duration)
	{
		_timer->Start();
		_timer->_duration = duration;

		Quaternion distance;
		distance.w = destination.w - dataQuat->w;
		distance.x = destination.x - dataQuat->x;
		distance.y = destination.y - dataQuat->y;
		distance.z = destination.z - dataQuat->z;

		Quaternion start = *this->dataQuat;

		_timer->_play = [=]()
		{
			dataQuat->w = start.w + (distance.w * (_myEase(_timer->_time / duration)));
			dataQuat->x = start.x + (distance.x * (_myEase(_timer->_time / duration)));
			dataQuat->y = start.y + (distance.y * (_myEase(_timer->_time / duration)));
			dataQuat->z = start.z + (distance.z * (_myEase(_timer->_time / duration)));
		};
		return *this;
	}

	Tween& Tween::DoMove(const int& destination, const float duration)
	{
		_timer->Start();
		_timer->_duration = duration;

		int distance = destination - *dataI;

		int start = *this->dataI;

		_timer->_play = [=]()
		{
			*dataI = start + (distance * (_myEase(_timer->_time / duration)));
		};

		return *this;
	}

	Tween& Tween::Delay(float delayTime)
	{
		float addDelay = 0.f - delayTime;
		_timer->_delayTime = delayTime;
		_timer->_time = addDelay;
		return *this;
	}

	Tween& Tween::OnComplete(std::function<void()> func)
	{
		onCompleteFunc = func;
		return *this;
	}

	std::function<float(float)> Tween::_easings[31] =
	{
		Linear,
		InBack,
		OutBack,
		InOutBack,
		InBounce,
		OutBounce,
		InOutBounce,
		InElastic,
		OutElastic,
		InOutElastic,
		InSine,
		OutSine,
		InOutSine,
		InExpo,
		OutExpo,
		InOutExpo,
		InCirc,
		OutCirc,
		InOutCirc,
		InQuad,
		OutQuad,
		InOutQuad,
		InCubic,
		OutCubic,
		InOutCubic,
		InQuart,
		OutQuart,
		InOutQuart,
		InQuint,
		OutQuint,
		InOutQuint
	};

	Tween& Tween::SetEase(eEasing type)
	{
		this->_myEase = _easings[type];
		return *this;
	}

	float Tween::Linear(float x)
	{
		return static_cast<float>(x);
	}

	float Tween::InBack(float x)
	{
		const float c1 = 1.70158f;
		const float c3 = c1 + 1;

		return static_cast<float>(c3 * x * x * x - c1 * x * x);
	}

	float Tween::OutBack(float x)
	{
		const float c1 = 1.70158f;
		const float c3 = c1 + 1;

		return static_cast<float>(1 + c3 * pow(x - 1, 3) + c1 * pow(x - 1, 2));
	}

	float Tween::InOutBack(float x)
	{
		const float c1 = 1.70158f;
		const float c2 = c1 * 1.525f;

		return x < 0.5
			? static_cast<float>((pow(2 * x, 2) * ((c2 + 1) * 2 * x - c2)) / 2)
			: static_cast<float>((2 * x - 2, 2) * ((c2 + 1) * (x * 2 - 2) + c2) + 2) / 2;
	}

	float Tween::InBounce(float x)
	{
		return static_cast<float>(1 - OutBounce(1 - x));
	}

	float Tween::OutBounce(float x)
	{
		const float n1 = 7.5625f;
		const float d1 = 2.75f;

		if (x < 1 / d1) {
			return static_cast<float>(n1 * x * x);
		}
		else if (x < 2 / d1) {
			return static_cast<float>(n1 * (x -= 1.5f / d1) * x + 0.75f);
		}
		else if (x < 2.5 / d1) {
			return static_cast<float>(n1 * (x -= 2.25f / d1) * x + 0.9375f);
		}
		else {
			return static_cast<float>(n1 * (x -= 2.625f / d1) * x + 0.984375f);
		}
	}

	float Tween::InOutBounce(float x)
	{
		return x < 0.5 ?
			static_cast<float>((1 - OutBounce(1 - 2 * x)) / 2) :
			static_cast<float>((1 + OutBounce(2 * x - 1)) / 2);
	}

	float Tween::InElastic(float x)
	{
		const float c4 = (float)((2 * PI) / 3);
		return x == 0 ? 0 : x == 1 ? 1 : static_cast<float>((-pow(2, 10 * x - 10) * sin((x * 10 - 10.75) * c4)));
	}

	float Tween::OutElastic(float x)
	{
		const float c4 = (float)((2 * PI) / 3);
		return x == 0 ? 0 : x == 1 ? 1 : static_cast<float>((pow(2, -10 * x) * sin((x * 10 - 0.75) * c4) + 1));
	}

	float Tween::InOutElastic(float x)
	{
		const float c5 = (float)((2 * PI) / 4.5);
		return x == 0 ? 0 : x == 1 ? 1 : x < 0.5 ? static_cast<float>(-(pow(2, 20 * x - 10) * sin((20 * x - 11.125) * c5)) / 2) : (float)((pow(2, -20 * x + 10) * sin((20 * x - 11.125) * c5)) / 2 + 1);
	}

	float Tween::InSine(float x)
	{
		return static_cast<float>(1 - cos(x * PI) / 2);
	}

	float Tween::OutSine(float x)
	{
		return static_cast<float>(sin((x * PI) / 2));
	}

	float Tween::InOutSine(float x)
	{
		return static_cast<float>(-(cos(PI * x) - 1) / 2);
	}

	float Tween::InExpo(float x)
	{
		return x == 0 ? 0 : static_cast<float>(pow(2, 10 * x - 10));
	}

	float Tween::OutExpo(float x)
	{
		return x == 1 ? 1 : static_cast<float>(1 - pow(2, -10 * x));
	}

	float Tween::InOutExpo(float x)
	{
		return x == 0 ? 0 : x == 1 ? 1 : x < 0.5 ?
			static_cast<float>(pow(2, 20 * x - 10) / 2) :
			static_cast<float>((2 - pow(2, -20 * x + 10)) / 2);
	}

	float Tween::InCirc(float x)
	{
		return static_cast<float>(1 - sqrt(1 - pow(x, 2)));
	}

	float Tween::OutCirc(float x)
	{
		return static_cast<float>(sqrt(1 - pow(x - 1, 2)));
	}

	float Tween::InOutCirc(float x)
	{
		return x < 0.5 ? 16 * x * x * x * x * x : static_cast<float>(1 - pow(-2 * x + 2, 5) / 2);
	}

	float Tween::InQuad(float x)
	{
		return x * x;
	}

	float Tween::OutQuad(float x)
	{
		return 1 - (1 - x) * (1 - x);
	}

	float Tween::InOutQuad(float x)
	{
		return x < 0.5 ? 2 * x * x : static_cast<float>(1 - pow(-2 * x + 2, 2) / 2);
	}

	float Tween::InCubic(float x)
	{
		return x * x * x;
	}

	float Tween::OutCubic(float x)
	{
		return static_cast<float>(1 - pow(1 - x, 3));
	}

	float Tween::InOutCubic(float x)
	{
		return x < 0.5 ? 4 * x * x * x : static_cast<float>(1 - pow(-2 * x + 2, 3) / 2);
	}

	float Tween::InQuart(float x)
	{
		return x * x * x * x;
	}

	float Tween::OutQuart(float x)
	{
		return static_cast<float>(1 - pow(1 - x, 4));
	}

	float Tween::InOutQuart(float x)
	{
		return x < 0.5 ? 8 * x * x * x * x : static_cast<float>(1 - pow(-2 * x + 2, 4) / 2);
	}

	float Tween::InQuint(float x)
	{
		return x * x * x * x * x;
	}

	float Tween::OutQuint(float x)
	{
		return  static_cast<float>(1 - pow(1 - x, 5));
	}

	float Tween::InOutQuint(float x)
	{
		return x < 0.5 ? 16 * x * x * x * x * x : static_cast<float>(1 - pow(-2 * x + 2, 5) / 2);
	}
}
