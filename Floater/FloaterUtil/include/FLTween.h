#pragma once
#include <vector>
#include <functional>
#include <type_traits>
#include "FloaterMacro.h"


namespace flt
{
	template<typename T>
	using LerpFunction = std::function<T(const T&, const T&, float)>;

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
		std::function<float(float)> easing;
	};

	template<typename T, LerpFunction Lerp = defaultLerp<T>>
	class FLTween
	{
		using ValueType = std::remove_pointer_t<std::remove_reference_t<T>>;

	public:
		FLTween(T target);
		FLTween(const FLTween& other);

		T step(float dt);
		T seek(float dt);
		T peek() const;

		//FLTween& onStep(std::function<bool(FLTween&)> callback);
		//FLTween& onStep(std::function<bool(FLTween&, T)> callback);
		//FLTween& onStep(std::function<bool(T)> callback);
		//FLTween& onStep(std::function<bool()> callback);

		//static FLTween<T> from(const T& value);

		FLTween& to(const ValueType& value);
		FLTween& during(float duration);
		FLTween& easing(std::function<float(float)> easing);

	private:
		float _elapsed;
		int _current;
		std::vector<FLTweenPoint<ValueType>> _points;
		T _target;
	};

	namespace tween
	{
		template<typename T, LerpFunction Lerp = defaultLerp<std::remove_pointer_t<std::remove_reference_t<T>>>>
		FLTween<T, Lerp> from(const T& value)
		{
			FLTween<T> tween{ value };
			if constexpr (std::is_pointer_v<T>)
			{
				tween.to(*value);
			}
			else
			{
				tween.to(value);
			}
			return tween;
		}
	}


	template<typename T, LerpFunction Lerp>
	T flt::FLTween<T, Lerp>::seek(float dt)
	{
		ASSERT(false, "Not implemented");
		return T{};
	}

	template<typename T, LerpFunction Lerp>
	T flt::FLTween<T, Lerp>::peek() const
	{
		float t = _points[_current].easing(_elapsed / _points[_current].duration);
		return _points[_current].value * (1.0f - t) + _points[_current + 1].value * t;
	}

	template<typename T, LerpFunction Lerp>
	FLTween<T, Lerp>& flt::FLTween<T, Lerp>::easing(std::function<float(float)> easing)
	{
		_points[_points.size() - 2].easing = easing;
		return *this;
	}

	template<typename T, LerpFunction Lerp>
	FLTween<T, Lerp>& flt::FLTween<T, Lerp>::during(float duration)
	{
		_points[_points.size() - 2].duration = duration;
		return *this;
	}

	template<typename T, LerpFunction Lerp>
	T flt::FLTween<T, Lerp>::step(float dt)
	{
		ASSERT(_points.size() >= 2, "At least two points are required.");

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

		while (_elapsed >= _points[_current].duration)
		{
			_elapsed -= _points[_current].duration;
			_current++;
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
		}

		float t = _points[_current].easing(_elapsed / _points[_current].duration);

		if constexpr (std::is_pointer_v<T>)
		{
			*_target = _points[_current].value * (1.0f - t) + _points[_current + 1].value * t;
		}
		else
		{
			_target = _points[_current].value * (1.0f - t) + _points[_current + 1].value * t;
		}

		return _target;
	}

	template<typename T, LerpFunction Lerp>
	FLTween<T, Lerp>& FLTween<T, Lerp>::to(const ValueType& value)
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
	flt::FLTweenPoint<T>::FLTweenPoint() : value(), duration(0.0f), easing([](float t) {return t; })
	{

	}

	template<typename T>
	flt::FLTweenPoint<T>::FLTweenPoint(const T& value) : value(value), duration(0.0f), easing([](float t) {return t; })
	{

	}


	template<typename T, LerpFunction Lerp>
	flt::FLTween<T, Lerp>::FLTween(T target) : _elapsed(0.0f), _current(0), _points(), _target(target)
	{

	}


	template<typename T, LerpFunction Lerp>
	flt::FLTween<T, Lerp>::FLTween(const FLTween& other) : _elapsed(other._elapsed), _current(other._current), _points(other._points), _target(other._target)
	{

	}

	//template <typename T, auto Lerp>
	//FLTween<T, Lerp>(const FLTween<T, Lerp>& other) -> FLTween<T, Lerp>;
}
