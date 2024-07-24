#pragma once
#include "../FloaterUtil/include/FloaterType.h"
#include "FMODInstance.h"
#include <vector>
#include <string>


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
	class FMODInstance;

	class SoundEngine
	{
		struct SoundDriver
		{
			enum class Mode
			{
				DEFAULT,
				RAW,
				MONO,
				STEREO,
				QUAD,
				SURROUND,
				SUR5POINT1,
				SUR7POINT1,
				SUR7POINT1POINT4,

				MAX,
				FORCEINT = 65536
			};

			std::wstring name;
			int fmodDriverIndex;
			int systemRate;
			int speakerModeChannels;
			Mode mode;
		};
	public:
		SoundEngine();
		void Initialize();
		void Finalize();

		void Update();

		//bool CreateSound(void* buff, FMOD_CREATESOUNDEXINFO* exinfo, FMOD::Sound** sound, uint32 instanceIndex = 0);
		bool CreateSound(const char* name, FMOD::Sound** sound, uint32 instanceIndex = 0);
		void Play(Sound* sound, bool isLoop = false, uint32 instanceIndex = 0);
		bool isPlay(Sound* sound, uint32 instanceIndex = 0);
		void Pause(Sound* sound, uint32 instanceIndex = 0);
		void Stop(Sound* sound, uint32 instanceIndex = 0);

		uint32 GetCurrOutputDriverNum() const;

	private:

	private:
		std::vector<FMODInstance> _instances;
		std::vector<SoundDriver> _drivers;
	};
}


