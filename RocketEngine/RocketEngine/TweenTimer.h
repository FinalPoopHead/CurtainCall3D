#pragma once
#include <functional>

namespace Rocket::Core
{
	class TweenTimer
	{
	public:
		TweenTimer();
		~TweenTimer();

	public:
		void Start();
		void Update();

	public:
		// object has each thier timer
		float _time;
		float _delayTime;
		float _duration;
		std::function<void()> _play;
	
		// delayed Active
		bool _isActive;
	};
}
