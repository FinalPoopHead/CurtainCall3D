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

	class SoundEngine
	{
	public:
		SoundEngine();
		void Initialize();
		void Finalize();

		bool CreateSound(void* buff, FMOD_CREATESOUNDEXINFO* exinfo, FMOD::Sound** sound);
		bool CreateSound(const char* name, FMOD::Sound** sound);
		void Play(Sound* sound);

	private:
		FMOD::System* _system;
		std::vector<FMOD::ChannelGroup*> _channelGroups;

		const float DISTANCEFACTOR = 1.0f;
		const int   INTERFACE_UPDATETIME = 50; // ms
	};
}


