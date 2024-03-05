#pragma once
#include "eSoundGroup.h"
#include <string>

#include <fmod.hpp>

namespace Rocket
{
	class Transform;
}

namespace Rocket::Core
{
	/// <summary>
	/// Sound 객체에 필요한 정보.
	/// </summary>
	struct RocketAudio
	{
		std::string path;				// Sound 경로
		Rocket::Transform* transform;	// Sound 좌표
		FMOD::Sound* sound;					// Sound 컨테이너
		FMOD::Channel* channel;				// Sound 채널주소
		eSoundGroup group;					// Sound 해당 그룹
		bool isPlaying;						// Sound 재생 중?

		// 23.08.15 오수안 시리얼라이즈를 위한 변수 추가

		bool is3DSound;
	};
}