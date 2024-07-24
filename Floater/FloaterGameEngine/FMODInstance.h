#pragma once
#include <vector>


namespace FMOD
{
	class System;
	class Sound;
	class ChannelControl;
	class Channel;
	class ChannelGroup;
	class SoundGroup;
	class DSP;
	class DSPConnection;
	class Geometry;
	class Reverb3D;
}

struct FMOD_CREATESOUNDEXINFO;

namespace flt
{
	class Sound;

	class FMODInstance
	{
	public:
		FMODInstance();
		void Initialize(int fmodDriverIndex);
		void Finalize();

		void Update();

		bool CreateSound(void* buff, FMOD_CREATESOUNDEXINFO* exinfo, FMOD::Sound** sound);
		bool CreateSound(const char* name, FMOD::Sound** sound);
		void Play(Sound* sound, bool isLoop = false);
		bool isPlay(Sound* sound);
		void Pause(Sound* sound);
		void Stop(Sound* sound);

		int GetDriverIndex();
	private:
		FMOD::System* _system;
		std::vector<FMOD::ChannelGroup*> _channelGroups;

		inline static constexpr float DISTANCEFACTOR = 1.0f;
		inline static constexpr int   INTERFACE_UPDATETIME = 50; // ms
	};

}
