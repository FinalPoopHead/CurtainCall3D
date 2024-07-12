#pragma once
#include <vector>
#include <functional>


namespace flt
{
	template<typename T>
	class FLTween;

	namespace tween
	{
		template<typename T>
		FLTween<T> from(const T& value);
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

	template<typename T>
	class FLTween
	{
		//template<typename U>
		//friend FLTween<U> tween::from(const U& value);

		friend FLTween<T> tween::from<T>(const T& value);
	public:
		FLTween();
		FLTween(const FLTween& other);

		T step(float dt);
		T seek(float dt);
		T peek() const;

		//FLTween& onStep(std::function<bool(FLTween&)> callback);
		//FLTween& onStep(std::function<bool(FLTween&, T)> callback);
		//FLTween& onStep(std::function<bool(T)> callback);
		//FLTween& onStep(std::function<bool()> callback);

		static FLTween<T> from(T value);

		FLTween& to(const T& value);
		FLTween& during(float duration);
		FLTween& easing(std::function<float(float)> easing);

	private:
		float _elapsed;
		int _current;
		std::vector<FLTweenPoint<T>> _points;
	};

	template<typename T>
	FLTween<T> tween::from(const T& value)
	{
		FLTween<T> tween;
		tween._points.emplace_back(value);
		return tween;
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
		float t = _points[_current].easing(_elapsed / _points[_current].duration);
		return _points[_current].value * (1.0f - t) + _points[_current + 1].value * t;
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
	T flt::FLTween<T>::step(float dt)
	{
		ASSERT(_points.size() >= 2, "At least two points are required.");

		if (_current >= _points.size())
		{
			return _points.back().value;
		}

		_elapsed += dt;

		while (_elapsed >= _points[_current].duration)
		{
			_elapsed -= _points[_current].duration;
			_current++;
			if (_current >= _points.size())
			{
				return _points.back().value;
			}
		}

		float t = _points[_current].easing(_elapsed / _points[_current].duration);

		return _points[_current].value * (1.0f - t) + _points[_current + 1].value * t;
	}

	template<typename T>
	FLTween<T>& FLTween<T>::to(const T& value)
	{
		_points.push_back(FLTweenPoint<T>(value));
		return *this;
	}

	template<typename T>
	flt::FLTween<T> flt::FLTween<T>::from(T value)
	{
		FLTween<T> tween;
		tween._points.push_back(FLTweenPoint<T>(value));
		return tween;
	}

	template<typename T>
	flt::FLTweenPoint<T>::FLTweenPoint() : value(), duration(0.0f), easing([](float t) {return t; })
	{

	}

	template<typename T>
	flt::FLTweenPoint<T>::FLTweenPoint(const T& value) : value(value), duration(0.0f), easing([](float t) {return t; })
	{

	}


	template<typename T>
	flt::FLTween<T>::FLTween() : _elapsed(0.0f), _current(0), _points()
	{

	}


	template<typename T>
	flt::FLTween<T>::FLTween(const FLTween& other) : _elapsed(other._elapsed), _current(other._current), _points(other._points)
	{

	}
}
