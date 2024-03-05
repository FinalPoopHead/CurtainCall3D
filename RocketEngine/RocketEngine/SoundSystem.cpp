#include "SoundSystem.h"
#include "Transform.h"

namespace Rocket::Core
{
	SoundSystem::SoundSystem()
		: _pSystem()
		, _pChannelGroup()
		, _pChannelMasterGroup()
		, _soundMap()
		, _soundCountMax(0)
		, _channelGroupMax(12)
		, _isCurrentPlay()
		, _objectSoundPos()
		, _objectSoundVel()
		, _listenerPos()
		, _listenerTransform()
	{
		_pChannelGroup.resize(_channelGroupMax);

		// FMOD::System �ʱ�ȭ
		FMOD::System_Create(&_pSystem);								// FMOD System ��ü ���� (���� 1���� ���)
		_pSystem->init(100, FMOD_INIT_NORMAL, NULL);	// FMOD System ��ü �ʱ�ȭ (�⺻ ����)
		_pSystem->getMasterChannelGroup(&_pChannelMasterGroup);		// FMOD System ä�� �׷� �ʱ�ȭ
		_pSystem->set3DSettings(1.0f, _distancefactor, 1.0f);		// 3D ���忡 ���� ũ��, �Ÿ� ��� ���� ����

		// FMOD::ChannelGroup �ʱ�ȭ
		for (int i = 0; i < (int)_channelGroupMax; i++)
		{
			// ���� Group �ʱ�ȭ
			_pChannelGroup[i] = 0;
			std::string temp = "GroupName" + std::to_string(i);
			_pSystem->createChannelGroup(temp.c_str(), &_pChannelGroup[i]);
		}
		for (int i = 0; i < (int)_channelGroupMax; i++)
		{
			// ���� Group, MasterGroup�� �߰�
			_pChannelMasterGroup->addGroup(_pChannelGroup[i]);
		}
	}

	void SoundSystem::Initialize()
	{
		_soundCountMax = _soundMap.size();
	}

	// System �� Sound �� ��� �� �� ���� ������ ��� �Ѵ�.
	void SoundSystem::Finalize()
	{
		// FMOD ���� ���� ����
		//std::vector<FMOD_VECTOR>().swap(_objectSoundVel);
		//std::vector<FMOD_VECTOR>().swap(_objectSoundPos);
		//std::vector<std::string>().swap(_soundPathList);

		// FMOD ���� (FMOD::System, FMOD::ChannelGroup�� �ݵ�� �����ؾ���)
		for (int i = 0; i < (int)_channelGroupMax; i++)
		{
			_pChannelGroup[i]->release();
		}

		_pChannelMasterGroup->release();

		_pSystem->close();
		_pSystem->release();
	}

	void SoundSystem::CreateSound(std::string soundPath, eSoundGroup soundGroup)
	{
		// Sound ��ü ���� �� Info ����
		RocketAudio newAudio;

		// Sound �߰�
		_pSystem->createSound(soundPath.c_str(), FMOD_LOOP_NORMAL | FMOD_DEFAULT, NULL, &(newAudio.sound));
		
		newAudio.path = soundPath;
		newAudio.group = soundGroup;
		newAudio.isPlaying = false;
		newAudio.channel = 0;
		newAudio.channel->setChannelGroup(_pChannelGroup[soundGroup]);
		newAudio.is3DSound = false;

		// sunordered_map�� Sound ��ü ����
		_soundMap.insert({ soundPath , newAudio });
	}

	void SoundSystem::CreateSound3D(std::string soundPath, eSoundGroup soundGroup
		, float minDistance, float maxDistance, Rocket::Transform* pos, float* vec)
	{
		// Sound ��ü ���� �� Info ����
		RocketAudio newAudio;

		// 3D Sound �߰�
		_pSystem->createSound(soundPath.c_str(), FMOD_3D, 0, &(newAudio.sound));

		newAudio.path = soundPath;
		newAudio.transform = pos;
		newAudio.group = soundGroup;
		newAudio.isPlaying = false;
		newAudio.channel = 0;
		newAudio.channel->setChannelGroup(_pChannelGroup[newAudio.group]);
		newAudio.is3DSound = true;

		// set3DMinMaxDistance (���� ũ��, ��û ���� ����)	// 3D sound �ִ� ���� ����
		newAudio.sound->set3DMinMaxDistance(
			minDistance * _distancefactor, maxDistance * _distancefactor);

		// sunordered_map�� Sound ��ü ����
		_soundMap.insert({ soundPath , newAudio });

		/// �̰� ���� ������ ���߿� �ʿ��ϸ� �߰�
		//_objectSoundVel[_currentSoundCount].x = vec[0];
		//_objectSoundVel[_currentSoundCount].y = vec[1];
		//_objectSoundVel[_currentSoundCount].z = vec[2];
	}

	void SoundSystem::PlayOnce(std::string soundPath)
	{
		for (int i = 0; i < (int)_soundCountMax; i++)
		{
			if (_soundMap.at(soundPath).path == soundPath)
			{
				_soundMap.at(soundPath).sound->setMode(FMOD_LOOP_OFF);
				_pSystem->playSound(_soundMap.at(soundPath).sound, 0, true, &_soundMap.at(soundPath).channel);
				_soundMap.at(soundPath).channel->setPaused(false);
				_soundMap.at(soundPath).isPlaying =
					_soundMap.at(soundPath).channel->getCurrentSound(&_soundMap.at(soundPath).sound);
				break;
			}
		}
	}

	void SoundSystem::RepeatPlayback(std::string soundPath)
	{
		for (int i = 0; i < (int)_soundCountMax; i++)
		{
			if (_soundMap.at(soundPath).path == soundPath)
			{
				_soundMap.at(soundPath).sound->setMode(FMOD_LOOP_NORMAL);
				_pSystem->playSound(_soundMap.at(soundPath).sound, 0, true, &_soundMap.at(soundPath).channel);
				_soundMap.at(soundPath).channel->setPaused(false);
				_soundMap.at(soundPath).isPlaying =
					_soundMap.at(soundPath).channel->getCurrentSound(&_soundMap.at(soundPath).sound);
				break;
			}
		}
	}

	void SoundSystem::AllPlay()
	{
		// ��� Sound ���
		_pSystem->update();
	}

	void SoundSystem::PlayEnd(std::string soundPath)
	{
		// ����ó��
		if (_soundMap.at(soundPath).channel->getCurrentSound(&_soundMap.at(soundPath).sound) == false)
		{
			return;
		}
		_soundMap.at(soundPath).sound->setMode(FMOD_LOOP_OFF);
		_soundMap.at(soundPath).channel->stop();
	}

	void SoundSystem::PlayAllEnd()
	{
		for (auto& elem : _soundMap)
		{
			elem.second.channel->stop();
		}
	}

	void SoundSystem::Play3DOnce(std::string soundPath)
	{
		// �ʿ��ϸ� Ȱ��ȭ, ��������� ����
		//_soundMap.at(soundPath)->position;

		for (int i = 0; i < (int)_soundCountMax; i++)
		{
			if (_soundMap.at(soundPath).path == soundPath)
			{
				/// Sound ����
				_soundMap.at(soundPath).sound->setMode(FMOD_LOOP_OFF);
				_pSystem->playSound(_soundMap.at(soundPath).sound, 0, true, &_soundMap.at(soundPath).channel);

				// Object Pos Update
				_objectSoundPos.x = _soundMap.at(soundPath).transform->GetPosition().x;
				_objectSoundPos.y = _soundMap.at(soundPath).transform->GetPosition().y;
				_objectSoundPos.z = _soundMap.at(soundPath).transform->GetPosition().z;

				// ���� �Ⱦ��̴� set3DListenerAttributes() �Լ��� forward, up, vec �ʱ�ȭ.
				// ���� �� �ΰ��� ����Ѵٸ� ��� ������ ����.
				FMOD_VECTOR vec = { 0.0f, 0.0f, 0.0f };

				// Object Pos, vec ���� (vec�� �ϴ� �ӽ÷� 0 ���� �ʿ��ϸ� ä���ֱ�)
				_soundMap.at(soundPath).channel->set3DAttributes(&_objectSoundPos, 0);	// �д�, ���� �� ���÷��� �����ϴµ� ���Ǵ� ��ġ�� �ӵ��� ����
				_soundMap.at(soundPath).channel->setPaused(false);	// �Ͻ� ���� ���¸� �����Ѵ�. // true : ��� ����, false : ��� Ȱ��ȭ

				// Player�� Object ���� ��ȣ�ۿ�.
				_pSystem->set3DListenerAttributes(0, &_listenerPos, &vec, &_listenerForward, &_listenerUp);

				// ��������� �Ǵ�
				_soundMap.at(soundPath).isPlaying =
					_soundMap.at(soundPath).channel->getCurrentSound(&_soundMap.at(soundPath).sound);
				break;
			}
		}
	}

	void SoundSystem::Play3DRepeat(std::string soundPath)
	{
		for (int i = 0; i < (int)_soundCountMax; i++)
		{
			if (_soundMap.at(soundPath).path == soundPath)
			{
				/// Sound ����
				_soundMap.at(soundPath).sound->setMode(FMOD_LOOP_NORMAL);
				_pSystem->playSound(_soundMap.at(soundPath).sound, 0, true, &_soundMap.at(soundPath).channel);

				// Object Pos Update
				_objectSoundPos.x = _soundMap.at(soundPath).transform->GetPosition().x;
				_objectSoundPos.y = _soundMap.at(soundPath).transform->GetPosition().y;
				_objectSoundPos.z = _soundMap.at(soundPath).transform->GetPosition().z;

				// ���� �Ⱦ��̴� set3DListenerAttributes() �Լ��� forward, up, vec �ʱ�ȭ.
				// ���� �� �ΰ��� ����Ѵٸ� ��� ������ ����.
				FMOD_VECTOR vec = { 0.0f, 0.0f, 0.0f };

				// Object Pos, vec ���� (vec�� �ϴ� �ӽ÷� 0 ���� �ʿ��ϸ� ä���ֱ�)
				_soundMap.at(soundPath).channel->set3DAttributes(&_objectSoundPos, 0);	// �д�, ���� �� ���÷��� �����ϴµ� ���Ǵ� ��ġ�� �ӵ��� ����
				_soundMap.at(soundPath).channel->setPaused(false);	// �Ͻ� ���� ���¸� �����Ѵ�. // true : ��� ����, false : ��� Ȱ��ȭ

				// Player�� Object ���� ��ȣ�ۿ�.
				_pSystem->set3DListenerAttributes(0, &_listenerPos, &vec, &_listenerForward, &_listenerUp);

				// ��������� �Ǵ�
				_soundMap.at(soundPath).isPlaying =
					_soundMap.at(soundPath).channel->getCurrentSound(&_soundMap.at(soundPath).sound);
				break;
			}
		}
	}

	void SoundSystem::UpdateSound()
	{
		if (!_listenerTransform)
		{
			return;
		}

		Vector3 pos = _listenerTransform->GetPosition();
		_listenerPos.x = pos.x;
		_listenerPos.y = pos.y;
		_listenerPos.z = pos.z;

		Vector3 forward = _listenerTransform->GetForward();
		_listenerForward.x = forward.x;
		_listenerForward.y = forward.y;
		_listenerForward.z = forward.z;

		Vector3 up = _listenerTransform->GetUp();
		_listenerUp.x = up.x;
		_listenerUp.y = up.y;
		_listenerUp.z = up.z;

		// ��� Sound ���
		_pSystem->update();
	}

	void SoundSystem::SoundMute(std::string soundPath)
	{
		bool mute = true;
		_soundMap.at(soundPath).channel->getMute(&mute);
		_soundMap.at(soundPath).channel->setMute(!mute);
	}

	void SoundSystem::SoundVolume(std::string soundPath, float volume)
	{
		_soundMap.at(soundPath).channel->setVolume(volume);
	}

	void SoundSystem::SoundPaused(std::string soundPath)
	{
		bool paused = true;
		_soundMap.at(soundPath).channel->getPaused(&paused);
		_soundMap.at(soundPath).channel->setPaused(!paused);
	}

	void SoundSystem::SoundGroupMuteAll()
	{
		bool mute = true;
		_pChannelMasterGroup->getMute(&mute);
		_pChannelMasterGroup->setMute(!mute);
	}

	void SoundSystem::SoundGroupMute(std::string soundPath)
	{
		bool mute = true;
		for (int i = 0; i < (int)_soundCountMax; i++)
		{
			if (_soundMap.at(soundPath).path == soundPath)
			{
				_pChannelGroup[i]->getMute(&mute);
				_pChannelGroup[i]->setMute(!mute);
				break;
			}
		}
	}

	void SoundSystem::SetAllGroupVolume(float volume)
	{
		_pChannelMasterGroup->setVolume(volume);
	}

	void SoundSystem::SetGroupVolume(eSoundGroup group, float volume)
	{
		//_pChannelGroup[group]->setVolume(volume);

		for (auto iter = _soundMap.begin(); iter != _soundMap.end(); iter++)
		{
			if (iter->second.group == group)
			{
				iter->second.channel->setVolume(volume);
			}
		}
		_pChannelGroup[group]->setVolume(volume);
	}

	float SoundSystem::GetGroupVolume(eSoundGroup group)
	{
		float temp = 0.0f;
		auto result = _pChannelGroup[group]->getVolume(&temp);
		if (result != FMOD_OK)
		{
			temp = 10.0f;
		}
		return temp;
	}

	void SoundSystem::SoundGroupPausedAll()
	{
		bool paused;
		_pChannelMasterGroup->getPaused(&paused);
		_pChannelMasterGroup->setPaused(!paused);
	}

	void SoundSystem::SoundGroupPaused(eSoundGroup soundGroupName)
	{
		bool paused;
		_pChannelGroup[soundGroupName]->getPaused(&paused);
		_pChannelGroup[soundGroupName]->setPaused(!paused);
	}

	bool SoundSystem::IsSoundPlaying(std::string soundPath)
	{
		bool tempIsPlay;
		_soundMap.at(soundPath).channel->isPlaying(&tempIsPlay);

		return tempIsPlay;
	}

	void SoundSystem::SetListenerTransform(Rocket::Transform* transform)
	{
		_listenerTransform = transform;
	}

	std::unordered_map<std::string, RocketAudio>& SoundSystem::GetSoundMap()
	{
		return _soundMap;
	}

	//void SoundSystem::SetPaused(std::string soundPath, bool pausedSetting)
	//{

	//	for (int i = 0; i < _soundCountMax; i++)
	//	{
	//		if (_soundPathList[i] == soundPath)
	//		{
	//			_pChannel[i]->setPaused(pausedSetting);
	//			break;
	//		}
	//	}
	//}

	//bool SoundSystem::GetPaused(std::string soundPath)
	//{
	//	for (int i = 0; i < _soundCountMax; i++)
	//	{
	//		if (_soundPathList[i] == soundPath)
	//		{
	//			_paused = _pChannel[i]->getPaused(&_paused);
	//			return _paused;
	//		}
	//	}
	//}

}
