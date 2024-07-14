#pragma once
#include <vector>
#include <functional>
#include <type_traits>
#include <algorithm>
#include "FloaterMacro.h"


namespace flt
{
	template<typename T>
	using LerpFunction = T(*)(const T& a, const T& b, float t);

	template<typename T>
	T defaultLerp(const T& a, const T& b, float t)
	{
		return a + t * (b - a);
	}

	template<typename T>
	struct FLTweenPoint
	{
		FLTweenPoint();
		FLTweenPoint(const T&);

		T value;
		float duration;
		float preDelay;
		float postDelay;
		std::function<float(float)> easing;

		std::function<void()> onStart;
		std::function<void(const T&)> onStep;
		std::function<void()> onEnd;
	};

	class IFLTween
	{
	public:
		virtual bool Update(float dt) = 0;
	};

	template<typename T>
	class FLTween : public IFLTween
	{
		using ValueType = std::remove_pointer_t<std::remove_reference_t<T>>;

	public:
		FLTween(T target, LerpFunction<ValueType> lerp);
		FLTween(const FLTween& other);

		virtual bool Update(float dt) override;
		T step(float dt);
		T seek(float dt);
		T peek() const;

		//FLTween& onStep(std::function<bool(FLTween&)> callback);
		//FLTween& onStep(std::function<bool(FLTween&, T)> callback);
		//FLTween& onStep(std::function<bool(T)> callback);
		//FLTween& onStep(std::function<bool()> callback);

		FLTween<T>& from(const ValueType& value);
		FLTween<T>& to(const ValueType& value);
		FLTween<T>& during(float duration);
		FLTween<T>& preDelay(float duration);
		FLTween<T>& postDelay(float duration);
		FLTween<T>& easing(std::function<float(float)> easing);

	private:
		float _elapsed;
		int _current;
		std::vector<FLTweenPoint<ValueType>> _points;
		LerpFunction<ValueType> _lerp;
		T _target;
	};

	namespace tween
	{
		template<typename T>
		FLTween<T> from(const T& value)
		{
			FLTween<T> tween{ value, defaultLerp<std::remove_pointer_t<std::remove_cvref_t<T>>> };
			return tween;
		}
	}

	template<typename T>
	T flt::FLTween<T>::seek(float dt)
	{
		ASSERT(false, "Not implemented");
		return T{};
	}

	template<typename T>
	T flt::FLTween<T>::peek() const
	{
		ASSERT(false, "Not implemented");
		return T{};
	}

	template<typename T>
	FLTween<T>& flt::FLTween<T>::easing(std::function<float(float)> easing)
	{
		_points[_points.size() - 2].easing = easing;
		return *this;
	}

	template<typename T>
	FLTween<T>& flt::FLTween<T>::during(float duration)
	{
		_points[_points.size() - 2].duration = duration;
		return *this;
	}


	template<typename T>
	flt::FLTween<T>& flt::FLTween<T>::preDelay(float duration)
	{
		_points[_points.size() - 2].preDelay = duration;
		return *this;
	}

	template<typename T>
	flt::FLTween<T>& flt::FLTween<T>::postDelay(float duration)
	{
		_points[_points.size() - 2].postDelay = duration;
		return *this;
	}

	template<typename T>
	bool flt::FLTween<T>::Update(float dt)
	{
		step(dt);

		return _current >= _points.size();
	}

	template<typename T>
	T flt::FLTween<T>::step(float dt)
	{
		ASSERT(_points.size() >= 2, "At least two points are required.");

		//가장 마지막을 지난다면 마지막 값을 반환
		if (_current >= _points.size())
		{
			if constexpr (std::is_pointer_v<T>)
			{
				*_target = _points.back().value;
			}
			else
			{
				_target = _points.back().value;
			}

			return _target;
		}

		_elapsed += dt;

		float pointTime = _points[_current].preDelay + _points[_current].duration + _points[_current].postDelay;
		while (_elapsed >= pointTime)
		{
			_elapsed -= pointTime;
			_points[_current].onEnd();
			_current++;

			// 가장 마지막을 지난다면 마지막 값을 반환
			if (_current >= _points.size())
			{
				if constexpr (std::is_pointer_v<T>)
				{
					*_target = _points.back().value;
				}
				else
				{
					_target = _points.back().value;
				}

				return _target;
			}

			_points[_current].onStart();
			pointTime = _points[_current].preDelay + _points[_current].duration + _points[_current].postDelay;
		}

		float t = (_elapsed - _points[_current].preDelay) / _points[_current].duration;
		t = std::clamp(t, 0.0f, 1.0f);
		t = _points[_current].easing(t);

		if constexpr (std::is_pointer_v<T>)
		{
			*_target = _lerp(_points[_current].value, _points[_current + 1].value, t);
			_points[_current].onStep(*_target);
		}
		else
		{
			_target = _lerp(_points[_current].value, _points[_current + 1].value, t);
			_points[_current].onStep(_target);
		}

		return _target;
	}

	template<typename T>
	FLTween<T>& flt::FLTween<T>::from(const ValueType& value)
	{
		_points[0].value = value;
		return *this;
	}


	template<typename T>
	FLTween<T>& FLTween<T>::to(const ValueType& value)
	{
		_points.push_back(FLTweenPoint<ValueType>(value));
		return *this;
	}

	//template<typename T, auto Lerp>
	//flt::FLTween<T, Lerp> flt::FLTween<T, Lerp>::from(const T& value)
	//{
	//	FLTween<T> tween{ value };
	//	if constexpr (std::is_pointer_v<T>)
	//	{
	//		tween._points.emplace_back(*value);
	//	}
	//	else
	//	{
	//		tween._points.emplace_back(value);
	//	}
	//	return tween;
	//}

	template<typename T>
	flt::FLTweenPoint<T>::FLTweenPoint()
		: value()
		, duration(0.0f), preDelay(0.0f), postDelay(0.0f)
		, easing([](float t) {return t; })
		, onStart([]() {}), onStep([](const T&) {}), onEnd([]() {})
	{

	}

	template<typename T>
	flt::FLTweenPoint<T>::FLTweenPoint(const T& value)
		: value(value)
		, duration(0.0f), preDelay(0.0f), postDelay(0.0f)
		, easing([](float t) {return t; })
		, onStart([]() {}), onStep([](const T&) {}), onEnd([]() {})
	{

	}


	template<typename T>
	flt::FLTween<T>::FLTween(T target, LerpFunction<ValueType> lerp) : _elapsed(0.0f), _current(0), _points(), _lerp(lerp), _target(target)
	{
		if constexpr (std::is_pointer_v<T>)
		{
			to(*target);
		}
		else
		{
			to(target);
		}
	}


	template<typename T>
	flt::FLTween<T>::FLTween(const FLTween& other) : _elapsed(other._elapsed), _current(other._current), _points(other._points), _lerp(other._lerp), _target(other._target)
	{

	}

	//template <typename T, auto Lerp>
	//FLTween<T, Lerp>(const FLTween<T, Lerp>& other) -> FLTween<T, Lerp>;
}
