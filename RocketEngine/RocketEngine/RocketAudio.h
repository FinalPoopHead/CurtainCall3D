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
	/// Sound ��ü�� �ʿ��� ����.
	/// </summary>
	struct RocketAudio
	{
		std::string path;				// Sound ���
		Rocket::Transform* transform;	// Sound ��ǥ
		FMOD::Sound* sound;					// Sound �����̳�
		FMOD::Channel* channel;				// Sound ä���ּ�
		eSoundGroup group;					// Sound �ش� �׷�
		bool isPlaying;						// Sound ��� ��?

		// 23.08.15 ������ �ø������� ���� ���� �߰�

		bool is3DSound;
	};
}