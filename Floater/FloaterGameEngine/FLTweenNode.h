#pragma once
#include <functional>


namespace flt
{
	class FLTweenNode
	{
		friend class FLTween;
	public:
		FLTweenNode(std::function<float(float)> ease, float duration);
		~FLTweenNode() {}

		bool IsFinished();
		FLTweenNode* Next();
		void Append(FLTweenNode* next);

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
		float _duration;
		float _elapsed;
		bool _isFinished;

		FLTweenNode* _next;
	};
}
