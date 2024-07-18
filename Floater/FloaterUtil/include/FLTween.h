#pragma once
#include <vector>
#include <functional>
#include <type_traits>
#include <algorithm>
#include "FloaterMacro.h"


namespace flt
{
	template<typename T>
	using LerpFunction_t = T(*)(const T& a, const T& b, float t);

	template<typename T>
	T defaultLerp(const T& a, const T& b, float t)
	{
		return static_cast<T>(a + (b - a) * t);
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

		std::vector<std::function<void()>> onStart;
		std::vector<std::function<void(const T&)>> onStep;
		std::vector<std::function<void()>> onEnd;
	};

	class IFLTween
	{
	public:
		IFLTween();
		virtual ~IFLTween();

		virtual bool Update(float dt) = 0;
		virtual void ResetProgress() = 0;
		virtual bool IsFinished() const = 0;
	};

	template<typename T>
	class FLTween : public IFLTween
	{
		using ValueType = std::remove_pointer_t<std::remove_reference_t<T>>;

	public:
		FLTween(T target, LerpFunction_t<ValueType> lerp = flt::defaultLerp<ValueType>);
		FLTween(const FLTween<T>& other);

		virtual bool IsFinished() const override { return InFinishedinternal(); }

		virtual bool Update(float dt) override;
		virtual void ResetProgress() override { _current = 0; _elapsed = 0.0f; }

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

		FLTween<T>& onStart(std::function<void()> callback);
		FLTween<T>& onStep(std::function<void(const ValueType&)> callback);
		FLTween<T>& onEnd(std::function<void()> callback);

	private:
		bool InFinishedinternal() const { return _current >= _points.size() - 1; }

	private:
		float _elapsed;
		int _current;
		std::vector<FLTweenPoint<ValueType>> _points;
		LerpFunction_t<ValueType> _lerp;
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
		ASSERT(_points.size() > 1, "At least two points are required.");
		_points[_points.size() - 2].easing = easing;
		return *this;
	}

	template<typename T>
	FLTween<T>& flt::FLTween<T>::during(float duration)
	{
		ASSERT(_points.size() > 1, "At least two points are required.");
		_points[_points.size() - 2].duration = duration;
		return *this;
	}


	template<typename T>
	flt::FLTween<T>& flt::FLTween<T>::preDelay(float duration)
	{
		ASSERT(_points.size() > 1, "At least two points are required.");
		_points[_points.size() - 2].preDelay = duration;
		return *this;
	}

	template<typename T>
	flt::FLTween<T>& flt::FLTween<T>::postDelay(float duration)
	{
		ASSERT(_points.size() > 1, "At least two points are required.");
		_points[_points.size() - 2].postDelay = duration;
		return *this;
	}

	template<typename T>
	FLTween<T>& flt::FLTween<T>::onStart(std::function<void()> callback)
	{
		ASSERT(_points.size() > 1, "At least two points are required.");
		_points[_points.size() - 2].onStart.emplace_back(callback);
		return *this;
	}

	template<typename T>
	FLTween<T>& flt::FLTween<T>::onStep(std::function<void(const ValueType&)> callback)
	{
		ASSERT(_points.size() > 1, "At least two points are required.");
		_points[_points.size() - 2].onStep.emplace_back(callback);
		return *this;
	}

	template<typename T>
	FLTween<T>& flt::FLTween<T>::onEnd(std::function<void()> callback)
	{
		ASSERT(_points.size() > 1, "At least two points are required.");
		_points[_points.size() - 2].onEnd.emplace_back(callback);
		return *this;
	}

	template<typename T>
	bool flt::FLTween<T>::Update(float dt)
	{
		step(dt);

		return InFinishedinternal();
	}

	template<typename T>
	T flt::FLTween<T>::step(float dt)
	{
		ASSERT(_points.size() > 1, "At least two points are required.");

		//가장 마지막을 지난다면 마지막 값을 반환
		if (InFinishedinternal())
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
			for (int i = 0; i < _points[_current].onEnd.size(); ++i)
			{
				_points[_current].onEnd[i]();
			}
			_current++;

			// 가장 마지막을 지난다면 마지막 값을 반환
			if (InFinishedinternal())
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

			for (int i = 0; i < _points[_current].onStart.size(); ++i)
			{
				_points[_current].onStart[i]();
			}
			pointTime = _points[_current].preDelay + _points[_current].duration + _points[_current].postDelay;
		}

		float t = (_elapsed - _points[_current].preDelay) / _points[_current].duration;
		t = std::clamp(t, 0.0f, 1.0f);
		t = _points[_current].easing(t);

		ASSERT(_current + 1 < _points.size(), "Invalid index");

		if constexpr (std::is_pointer_v<T>)
		{
			*_target = _lerp(_points[_current].value, _points[_current + 1].value, t);
			for (int i = 0; i < _points[_current].onStep.size(); ++i)
			{
				_points[_current].onStep[i](*_target);
			}
		}
		else
		{
			_target = _lerp(_points[_current].value, _points[_current + 1].value, t);
			for (int i = 0; i < _points[_current].onStep.size(); ++i)
			{
				_points[_current].onStep[i](_target);
			}
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
		: FLTweenPoint(T{})
	{

	}

	template<typename T>
	flt::FLTweenPoint<T>::FLTweenPoint(const T& value)
		: value(value)
		, duration(0.0f), preDelay(0.0f), postDelay(0.0f)
		, easing([](float t) {return t; })
		, onStart(), onStep(), onEnd()
	{

	}

	template<typename T>
	flt::FLTween<T>::FLTween(T target, LerpFunction_t<ValueType> lerp) : _elapsed(0.0f), _current(0), _points(), _lerp(lerp), _target(target)
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
