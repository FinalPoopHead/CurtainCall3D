#include "AudioClip.h"
#include "SoundSystem.h"
#include "GameObject.h"
#include "Transform.h"

namespace Rocket
{
	AudioClip::AudioClip()
		: _soundSystem(Rocket::Core::SoundSystem::Instance()),
		_audioMap(NULL),
		_volume()
	{

	}

	void AudioClip::Start()
	{
		_volume = 1.0f;
		//PlayAllEnd();
	}

	void AudioClip::PlayOnce(std::string audioName)
	{
		if (_audioMap.find(audioName) == _audioMap.end())
		{
			return;
		}

		_soundSystem.PlayOnce(_audioMap.at(audioName));
	}

	void AudioClip::PlayRepeat(std::string audioName)
	{
		if (_audioMap.find(audioName) == _audioMap.end())
		{
			return;
		}

		_soundSystem.RepeatPlayback(_audioMap.at(audioName));
	}

	void AudioClip::Play3DOnce(std::string audioName)
	{
		if (_audioMap.find(audioName) == _audioMap.end())
		{
			return;
		}

		_soundSystem.Play3DOnce(_audioMap.at(audioName));

		/// TEST¿ë
		//Vector3 palyerPos;
		//palyerPos.x = 0.0f;
		//palyerPos.y = 0.0f;
		//palyerPos.z = 0.0f;
		//_soundSystem.Play3DOnce(_audioMap.at(audioName), _volume, &palyerPos);
	}

	void AudioClip::Play3DRepeat(std::string audioName)
	{
		if (_audioMap.find(audioName) == _audioMap.end())
		{
			return;
		}
		
		_soundSystem.Play3DRepeat(_audioMap.at(audioName));

		/// TEST¿ë
		//Vector3 palyerPos;
		//palyerPos.x = 0.0f;
		//palyerPos.y = 0.0f;
		//palyerPos.z = 0.0f;
		//_soundSystem.Play3DRepeat(_audioMap.at(audioName), _volume, &palyerPos);
	}

	void AudioClip::Allplay()
	{
		_soundSystem.AllPlay();
	}

	void AudioClip::PlayEnd(std::string audioName)
	{
		_soundSystem.PlayEnd(audioName);
	}

	void AudioClip::PlayAllEnd()
	{
		for (auto& iter : _audioMap)
		{
			_soundSystem.PlayEnd(iter.second);
		}
	}

	void AudioClip::PlayMute(std::string audioName)
	{
		if (_audioMap.find(audioName) == _audioMap.end())
		{
			return;
		}

		_soundSystem.SoundMute(audioName);
	}

	void AudioClip::PlayPaused(std::string audioName)
	{
		if (_audioMap.find(audioName) == _audioMap.end())
		{
			return;
		}

		_soundSystem.SoundPaused(audioName);
	}

	void AudioClip::SoundVolumeSet(std::string audioName)
	{
		if (_audioMap.find(audioName) == _audioMap.end())
		{
			return;
		}

		_soundSystem.SoundVolume(audioName, _volume);
	}

	bool AudioClip::IsSoundPlaying(std::string audioName)
	{
		return _soundSystem.IsSoundPlaying(_audioMap.at(audioName));
	}

	void AudioClip::PlayGroupMute(std::string audioName)
	{
		_soundSystem.SoundGroupMute(audioName);
	}

	void AudioClip::PlayGroupAllMute()
	{
		_soundSystem.SoundGroupMuteAll();
	}

	void AudioClip::PlayGroupPaused(eSoundGroup soundGroupName)
	{
		_soundSystem.SoundGroupPaused(soundGroupName);
	}

	void AudioClip::PlayGroupAllPaused()
	{
		_soundSystem.SoundGroupPausedAll();
	}

	void AudioClip::SoundGroupVolumeSet(eSoundGroup soundGroupName)
	{
		_soundSystem.SetGroupVolume(soundGroupName, _volume);
	}

	void AudioClip::SoundGroupAllVolumeSet()
	{
		_soundSystem.SetAllGroupVolume(_volume);
	}

	void AudioClip::AddAudio(std::string audioName, std::string audioPath, eSoundGroup group)
	{
		_audioMap.insert({ audioName, audioPath });
		_soundSystem.CreateSound(audioPath, group);
	}

	void AudioClip::Add3DAudio(std::string audioName, std::string audioPath, eSoundGroup group,
		float minDistance, float maxDistance)
	{
		_audioMap.insert({audioName, audioPath});
		_volume = minDistance;
		
		_soundSystem.CreateSound3D(audioPath, group, minDistance, maxDistance,
			&(gameObject->transform), 0);

		/// TEST¿ë
		//Vector3* objectPos = new Vector3();
		//objectPos->x = 10.0f;
		//objectPos->y = 0.0f;
		//objectPos->z = 0.0f;
		//_soundSystem.CreateSound3D(audioPath, group, minDistance, maxDistance,
		//	objectPos, 0);
	}

	float AudioClip::GetVolume()
	{
		return _volume;
	}

	std::unordered_map<std::string, std::string>& AudioClip::GetAudios()
	{
		return _audioMap;
	}

}