#pragma once

namespace flt
{
	struct AnimState
	{
		AnimState() :
			index(-1),
			isLoop(false),
			isPlaying(false),
			frameTime(0.0f) {}

		int index;
		bool isLoop;
		bool isPlaying;
		float frameTime;
	};
}
