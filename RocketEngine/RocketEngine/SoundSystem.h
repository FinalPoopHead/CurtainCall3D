#pragma once
#include "Singleton.h"
#include <vector>
#include <unordered_map>
#include <string>
#include "RocketAudio.h"

#include <fmod.hpp>
#include "eSoundGroup.h"

#include "MathHeader.h" 

namespace Rocket
{
	class Transform;
}

namespace Rocket::Core
{
	/// <summary>
	/// Sound ����, ����� ���õ� Class.
	///
	/// - �ش� Class ���� ���ǻ��� -
	/// 2D Sound�� 3D Sound �� ���� ��ü�� ���� ������ ��.
	/// 
	/// ���� ������ : 2023.08.02 Kim Do Woo.
	/// </summary>
	class SoundSystem : public Singleton<SoundSystem>
	{
		friend Singleton;
	private:
		SoundSystem();

		/// Sound ���� �ʱ�ȭ, ����.
	public:
		/// SoundSystem �ʱ�ȭ.
		void Initialize();
		void Finalize();

		/// <summary>
		/// Sound �߰�.
		/// �Ÿ��� ���� Sound ������ CreateSound3D() �Լ� ����� ���� ����.
		/// </summary>
		/// <param name="soundName">Sound ���� �̸�</param>
		/// <param name="soundGroup">Sound Group ����</param>
		void CreateSound(std::string soundPath, eSoundGroup soundGroup);		// ���� �߰�

		/// <summary>
		/// 3D Sound �߰�.
		/// ������Ʈ�� 3D Sound �� ���� ��� ����.
		/// </summary>
		/// <param name="soundName">Sound ���� �̸�</param>
		/// <param name="soundGroup">Sound Group ����</param>
		/// <param name="minDistance">Sound volume ũ��</param>
		/// <param name="maxDistance">Object�κ��� 3D Sound ���� ����</param>
		/// <param name="pos">Object ��ġ</param>
		/// <param name="vec">Object �ӵ�</param>
		void CreateSound3D(std::string soundPath, eSoundGroup soundGroup
			, float minDistance, float maxDistance, Rocket::Transform* pos, float* vec);
	
	public:
		/// ���� ��� ���� �Լ�.

		void PlayOnce(std::string soundPath);			// �ѹ� ���
		void RepeatPlayback(std::string soundPath);	// �ݺ� ��� ����
		void AllPlay();

		void PlayEnd(std::string soundPath);						// ��� ����
		void PlayAllEnd();											// ��� ���� All

		/// 3D ���� ���.
		// ���÷�(Doppler) ȿ�� ���.
		void Play3DOnce(std::string soundPath);
		void Play3DRepeat(std::string soundPath);

		/// <summary>
		/// ��� Sound ���.
		/// ���� : Play �Լ� �Ѱ��� �ۼ� �� �ؿ� �� �Լ��� ���.
		/// ��� �� ���ǻ��� : Doppler ȿ�� ��� �� �� �Լ��� �ݵ�� ����� ��.
		/// </summary>
		void UpdateSound();			// ��� Sound ��� (3D Sound ���� �� ��� ����)

		/// ���� ���� ���� �Լ�.

		void SoundMute(std::string soundPath);					// OnOff ����
		void SoundVolume(std::string soundPath, float volume);	// ���� ����
		void SoundPaused(std::string soundPath);				// �Ͻ����� ����

		// Sound Group �Ҹ� ����.
		void SoundGroupMuteAll();						// ��� ChannelGroup �Ҹ� ����
		void SoundGroupMute(std::string soundPath);		// ���� ChannelGroup �Ҹ� ����

		// Sound Group �Ҹ� ���� ����.
		void SetAllGroupVolume(float volume);
		void SetGroupVolume(eSoundGroup group, float volume);	// ���� ChannelGroup ���� ����
		float GetGroupVolume(eSoundGroup group);

		// Sound Group �Ͻ����� ����.
		void SoundGroupPausedAll();
		void SoundGroupPaused(eSoundGroup soundGroupName);			// ���� ChannelGroup �Ͻ����� ����


		/// ���� Sound Pause �ʿ�� �߰�
		///// <summary>
		///// Sound �Ͻ����� ���¸� �����Ѵ�.
		///// true : ��� ����, false : ��� Ȱ��ȭ
		///// </summary>
		///// <param name="soundName">Sound ����</param>
		///// <param name="pausedSetting">pause ����</param>
		//void SetPaused(std::string soundPath, bool pausedSetting);		// Sound �Ͻ����� ���� ����
		//// ���� �ȵ�..
		//bool GetPaused(std::string soundPath);							// Sound �Ͻ����� ���� ������

		bool IsSoundPlaying(std::string soundPath);

	public:
		void SetListenerTransform(Rocket::Transform* transform);

	public:
		std::unordered_map<std::string, RocketAudio>& GetSoundMap();

	private:
		/// FMOD ���� ������.
		FMOD::System*						_pSystem;				// FMOD ��� ���ҽ� ��� ���� ��ü
		FMOD::ChannelGroup*		 _pChannelMasterGroup;				// ����� ä�� ��ü �׷� (_pChannelGroup ����)
		std::vector<FMOD::ChannelGroup*>	_pChannelGroup;			// ����� ä�� �׷�

		std::unordered_map<std::string, RocketAudio> _soundMap;	// FMOD Sound ������ ��� �ִ� ��ü

		/// FMOD ��� �ʿ��� ������.
		unsigned int				_soundCountMax;				// �� ��ü�� �� Sound �� �ִ� ����
		unsigned int				_channelGroupMax;			// �� ��ü���� ������ ChannelGroup�� �ִ밳��
		bool						_isCurrentPlay;				// ���� ������ΰ�?

		/// FMOD 3D ��� �ʿ��� ������.
		const float					_distancefactor = 1.0f;		// �Ÿ� ���
		FMOD_VECTOR					_objectSoundPos;			// Ojbect 3D Sound ��ġ
		FMOD_VECTOR					_objectSoundVel;			// Ojbect 3D Sound �ӵ�
		FMOD_VECTOR					_listenerPos;				// listener�� ��ġ (FMOD_VECTOR)
		FMOD_VECTOR					_listenerForward;			// listener Forwrd
		FMOD_VECTOR					_listenerUp;				// listener Up
		Rocket::Transform*	_listenerTransform;			// listener�� Transform
	};
}
