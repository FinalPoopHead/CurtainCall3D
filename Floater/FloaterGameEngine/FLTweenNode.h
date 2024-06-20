#pragma once
#include <functional>


namespace flt
{
	class FLTweenNode;
	class FLTweenNode
	{
		friend class FLTween;
	public:
		FLTweenNode();
		FLTweenNode(std::function<float(float)> ease, float duration);
		~FLTweenNode() {}

		bool IsFinished();
		FLTweenNode* Next();
		void Append(FLTweenNode* next);

		void SetTarget(float* target);
		void SetDelay(float delaySecond);
		void SetEase(std::function<float(float)> ease);
		void SetDuration(float duration);

		void SetOnComplete(std::function<void()> onComplete);
		void SetOnStart(std::function<void()> onStart);
		void SetOnPlay(std::function<void()> onPlay);
		void SetOnUpdate(std::function<void(float)> onUpdate);
		void SetOnPause(std::function<void()> onPause);



	private:
		void Update(float deltaSeconds);

	private:
		std::function<float(float)> _ease;
		float* _target;
		float _delay;
		float _duration;
		float _elapsed;
		float _from;
		float _to;

		bool _isFinished;

		std::function<void()> _onComplete;
		std::function<void()> _onStart;
		std::function<void()> _onPlay;
		std::function<void(float)> _onUpdate;
		std::function<void()> _onPause;

		FLTweenNode* _next;
	};
}
