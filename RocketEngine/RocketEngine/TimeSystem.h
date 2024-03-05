#pragma once
#include "Singleton.h"
#include <windows.h>

/// <summary>
/// 프레임 관리를 위해 제작한 클래스
/// 
/// 22.12.18 강석원 집
/// </summary>
namespace Rocket::Core
{
	class TimeSystem : public Singleton<TimeSystem>
	{
		friend Singleton;
	private:
		TimeSystem();

	public:
		void Initialize();
		void Finalize();
		void MeasureTime();

		static float GetDeltaTime() { return _deltaTime; }
		static int GetFrameRate() { return _frameRate; }

		void AccumulateFrame(float deltaTime);

		LARGE_INTEGER _frequency;
		LARGE_INTEGER _startTick;
		LARGE_INTEGER _endTick;

		float _elapsedTick;
		static float _deltaTime;
		static int _frameRate;
	};
}
