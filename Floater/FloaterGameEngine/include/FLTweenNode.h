#pragma once
#include <functional>
#include "../../FloaterRendererCommon/include/Transform.h"


namespace flt
{
	class FLTweenNode;
	class FLTweenNode
	{
	private:
		FLTweenNode();
		FLTweenNode(std::function<float(float)> ease, float duration);
		~FLTweenNode() {}

	public:
		bool IsFinished();
		FLTweenNode* Next();
		void Append(FLTweenNode* next);

		void SetDelay(float delaySecond);
		void SetEase(std::function<float(float)> ease);
		void SetDuration(float duration);

		void SetOnComplete(std::function<void()> onComplete);
		void SetOnStart(std::function<void()> onStart);
		void SetOnPlay(std::function<void()> onPlay);
		void SetOnUpdate(std::function<void(float)> onUpdate);
		void SetOnPause(std::function<void()> onPause);

	protected:
		virtual void Update(float deltaSeconds) = 0;
		std::function<float(float)> _ease;
		float _delay;
		float _duration;
		float _elapsed;
		bool _isFinished;

		std::function<void()> _onComplete;
		std::function<void()> _onStart;
		std::function<void()> _onPlay;
		std::function<void(float)> _onUpdate;
		std::function<void()> _onPause;

		FLTweenNode* _next;
	};

	template<typename T>
	class FLTweenNodeT : public FLTweenNode
	{
	public:
		virtual void Update(float deltaSeconds) override
		{
			_elapsed += deltaSeconds;
			if (_elapsed < _delay)
			{
				return;
			}

			if (_elapsed > _delay + _duration)
			{
				_isFinished = true;
				if (_onComplete)
				{
					_onComplete();
				}
				return;
			}

			float t = (_elapsed - _delay) / _duration;

			if (t > 0.0f)
			{
				//lerp
				*_to = _from + (_to - _from) * _ease(t);
				if (_onUpdate)
				{
					_onUpdate(deltaSeconds);
				}
			}
			else if (t > 1.0f)
			{
				*_target = _from + (_to - _from) * _ease(1.0f);
				if (_onUpdate)
				{
					_onUpdate(deltaSeconds);
				}

				_isFinished = true;
				if (_onComplete)
				{
					_onComplete();
				}
			}
		}

		void Setfrom(T from)
		{
			_from = from;
		}

		void SetTo(T to)
		{
			_to = to;
		}

		void SetTarget(T* target)
		{
			_target = target;
		}

	private:
		T _from;
		T _to;
		T* _target;
	};

	template<>
	class FLTweenNodeT<Transform> : public FLTweenNode
	{
		friend class FLTween;
	public:
		virtual void Update(float deltaSeconds) override
		{
			_elapsed += deltaSeconds;
			if (_elapsed < _delay)
			{
				return;
			}

			if (_elapsed > _delay + _duration)
			{
				_isFinished = true;
				if (_onComplete)
				{
					_onComplete();
				}
				return;
			}

			float t = (_elapsed - _delay) / _duration;

			if (t > 0.0f)
			{
				//lerp
				*_to = _from + (_to - _from) * _ease(t);
				if (_onUpdate)
				{
					_onUpdate(deltaSeconds);
				}
			}
			else if (t > 1.0f)
			{
				*_to = _from + (_to - _from) * _ease(1.0f);
				if (_onUpdate)
				{
					_onUpdate(deltaSeconds);
				}

				_isFinished = true;
				if (_onComplete)
				{
					_onComplete();
				}
			}
		}

		void Setfrom(Transform from)
		{
			_from = from;
		}
		void SetTarget(Transform* target)
		{
			_to = target;
		}

	private:
		Transform _from;
		Transform* _to;
	};
}
