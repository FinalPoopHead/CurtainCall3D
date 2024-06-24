#pragma once
#include <functional>
#include "../../FloaterRendererCommon/include/Transform.h"


namespace flt
{
	class FLTweenNode;
	class FLTweenNode
	{
		friend class FLTween;
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
}
