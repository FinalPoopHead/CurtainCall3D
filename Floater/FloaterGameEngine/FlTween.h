#pragma once
#include <functional>


namespace flt
{
	class FlTween
	{
	public:
		FlTween(std::function<float(float)> ease, float duration);
		~FlTween();

		void update(float deltaSeconds);
		float getProgress() const;
		bool isFinished() const;

	private:
		std::function<float(float)> _ease;
		float _duration;
		float _elapsed;
		bool _isFinished;
	};
}
