#include "SoundEngine.h"
#include "../FloaterUtil/include/FloaterMacro.h"

#include "../FloaterUtil/include/ConvString.h"
#include "FMODInstance.h"
#include "../External/include/fmod/fmod.hpp"
#include "../External/include/fmod/fmod_errors.h"
#include "Sound.h"

#pragma comment(lib, "../External/lib/x64/release/fmod_vc.lib")
#pragma comment(lib, "../External/lib/x64/release/fmodL_vc.lib")

flt::SoundEngine::SoundEngine()
{

}

void flt::SoundEngine::Initialize()
{
	FMOD::System* system;
	FMOD_RESULT result;
	result = FMOD::System_Create(&system);
	ASSERT(result == FMOD_OK, "");

	result = system->init(512, FMOD_INIT_NORMAL, 0);
	ASSERT(result == FMOD_OK, "");
	
	///기본적으로 디폴트 장치를 0번에 세팅해주는 코드.
	//int defaultDriverIndex = -1;
	//system->getDriver(&defaultDriverIndex);
	//ASSERT(defaultDriverIndex != -1, "Failed to get default driver index");
	//_instances.emplace_back();
	//_instances.back().Initialize(defaultDriverIndex);

	//여러 장치 사용하는 테스트.
	int numDrivers = 0;
	system->getNumDrivers(&numDrivers);

	for (int i = 0; i < numDrivers; ++i)
	{
		//if (i == defaultDriverIndex)
		//{
		//	continue;
		//}

		FMOD_GUID guid{};
		constexpr uint32 nameLen = 256;
		char name[nameLen];
		int systemRate = -1;
		FMOD_SPEAKERMODE speakerMode{};
		int speakerModeChannels = -1;

		system->getDriverInfo(i, name, nameLen, &guid, &systemRate, &speakerMode, &speakerModeChannels);

		constexpr std::string_view highDefinition = "High Definition Audio Device";
		constexpr std::string_view usbAudio = "USB Audio Device";
		std::string_view driverName = name;
		std::wstring wName;
		if (driverName.find(highDefinition) != std::string::npos)
		{
			wName = L"High Definition Audio Device";
		}
		else if (driverName.find(usbAudio) != std::string::npos)
		{
			// 헤드셋만 만들어보자.
			wName = L"USB Audio Device";
		}
		else
		{
			continue;
		}
		_drivers.emplace_back(SoundDriver{ wName, i, systemRate, speakerModeChannels, (SoundDriver::Mode)speakerMode });

		//그냥 모든 드라이버를 만들어보자.
		_instances.emplace_back();
		_instances.back().Initialize(i);
	}

	for(auto& instance : _instances)
	{
		int index = instance.GetDriverIndex();
	}
	
	system->release();
}

void flt::SoundEngine::Finalize()
{
	for (auto& instance : _instances)
	{
		instance.Finalize();
		//delete instance;
	}
}

void flt::SoundEngine::Update()
{
	for (auto& instance : _instances)
	{
		instance.Update();
	}
}

//bool flt::SoundEngine::CreateSound(void* buff, FMOD_CREATESOUNDEXINFO* exinfo, FMOD::Sound** sound, uint32 instanceIndex /*= 0*/)
//{
//	return _instances[instanceIndex].CreateSound(buff, exinfo, sound);
//}

bool flt::SoundEngine::CreateSound(const char* name, FMOD::Sound** sound, uint32 instanceIndex /*= 0*/)
{
	//bool ret = true;
	//for (auto& instance : _instances)
	//{
	//	ret &= instance.CreateSound(name, sound);
	//	ASSERT(ret, "Failed");
	//}

	//return ret;
	return _instances[instanceIndex].CreateSound(name, sound);
}

void flt::SoundEngine::Play(flt::Sound* sound, bool isLoop/*= false*/, uint32 instanceIndex /*= 0*/)
{
	//for (auto& instance : _instances)
	//{
	//	instance.Play(sound, isLoop);
	//}

	_instances[instanceIndex].Play(sound, isLoop);
}

bool flt::SoundEngine::isPlay(Sound* sound, uint32 instanceIndex /*= 0*/)
{
	//bool ret = true;
	//for (auto& instance : _instances)
	//{
	//	ret &= instance.isPlay(sound);
	//	ASSERT(ret, "Failed");
	//}

	//return ret;
	return _instances[instanceIndex].isPlay(sound);
}

void flt::SoundEngine::Pause(Sound* sound, uint32 instanceIndex /*= 0*/)
{
	//for (auto& instance : _instances)
	//{
	//	instance.Pause(sound);
	//}

	_instances[instanceIndex].Pause(sound);
}

void flt::SoundEngine::Stop(flt::Sound* sound, uint32 instanceIndex /*= 0*/)
{
	//for (auto& instance : _instances)
	//{
	//	instance.Stop(sound);
	//}

	_instances[instanceIndex].Stop(sound);
}

uint32 flt::SoundEngine::GetCurrOutputDriverNum() const
{
	return (uint32)_instances.size();
}
