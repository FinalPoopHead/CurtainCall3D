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
	/// Sound 관리, 재생에 관련된 Class.
	///
	/// - 해당 Class 사용시 유의사항 -
	/// 2D Sound와 3D Sound 는 따로 객체로 만들어서 관리할 것.
	/// 
	/// 최종 수정일 : 2023.08.02 Kim Do Woo.
	/// </summary>
	class SoundSystem : public Singleton<SoundSystem>
	{
		friend Singleton;
	private:
		SoundSystem();

		/// Sound 관련 초기화, 생성.
	public:
		/// SoundSystem 초기화.
		void Initialize();
		void Finalize();

		/// <summary>
		/// Sound 추가.
		/// 거리에 따른 Sound 조절시 CreateSound3D() 함수 사용할 것을 권함.
		/// </summary>
		/// <param name="soundName">Sound 파일 이름</param>
		/// <param name="soundGroup">Sound Group 지정</param>
		void CreateSound(std::string soundPath, eSoundGroup soundGroup);		// 사운드 추가

		/// <summary>
		/// 3D Sound 추가.
		/// 오브젝트에 3D Sound 를 붙일 경우 유용.
		/// </summary>
		/// <param name="soundName">Sound 파일 이름</param>
		/// <param name="soundGroup">Sound Group 지정</param>
		/// <param name="minDistance">Sound volume 크기</param>
		/// <param name="maxDistance">Object로부터 3D Sound 적용 범위</param>
		/// <param name="pos">Object 위치</param>
		/// <param name="vec">Object 속도</param>
		void CreateSound3D(std::string soundPath, eSoundGroup soundGroup
			, float minDistance, float maxDistance, Rocket::Transform* pos, float* vec);
	
	public:
		/// 사운드 재생 관련 함수.

		void PlayOnce(std::string soundPath);			// 한번 재생
		void RepeatPlayback(std::string soundPath);	// 반복 재생 시작
		void AllPlay();

		void PlayEnd(std::string soundPath);						// 재생 종료
		void PlayAllEnd();											// 재생 종료 All

		/// 3D 사운드 재생.
		// 도플러(Doppler) 효과 재생.
		void Play3DOnce(std::string soundPath);
		void Play3DRepeat(std::string soundPath);

		/// <summary>
		/// 모든 Sound 출력.
		/// 사용법 : Play 함수 한곳에 작성 뒤 밑에 이 함수를 사용.
		/// 사용 시 주의사항 : Doppler 효과 사용 시 이 함수를 반드시 사용할 것.
		/// </summary>
		void UpdateSound();			// 모든 Sound 재생 (3D Sound 있을 시 사용 금지)

		/// 사운드 설정 관련 함수.

		void SoundMute(std::string soundPath);					// OnOff 설정
		void SoundVolume(std::string soundPath, float volume);	// 봄륨 설정
		void SoundPaused(std::string soundPath);				// 일시정지 설정

		// Sound Group 소리 설정.
		void SoundGroupMuteAll();						// 모든 ChannelGroup 소리 설정
		void SoundGroupMute(std::string soundPath);		// 개별 ChannelGroup 소리 설정

		// Sound Group 소리 봄륨 설정.
		void SetAllGroupVolume(float volume);
		void SetGroupVolume(eSoundGroup group, float volume);	// 개별 ChannelGroup 봄륨 설정
		float GetGroupVolume(eSoundGroup group);

		// Sound Group 일시정지 설정.
		void SoundGroupPausedAll();
		void SoundGroupPaused(eSoundGroup soundGroupName);			// 개별 ChannelGroup 일시정지 설정


		/// 개별 Sound Pause 필요시 추가
		///// <summary>
		///// Sound 일시정지 상태를 설정한다.
		///// true : 재생 중지, false : 재생 활성화
		///// </summary>
		///// <param name="soundName">Sound 제목</param>
		///// <param name="pausedSetting">pause 상태</param>
		//void SetPaused(std::string soundPath, bool pausedSetting);		// Sound 일시정지 상태 설정
		//// 아직 안됨..
		//bool GetPaused(std::string soundPath);							// Sound 일시정지 상태 보내기

		bool IsSoundPlaying(std::string soundPath);

	public:
		void SetListenerTransform(Rocket::Transform* transform);

	public:
		std::unordered_map<std::string, RocketAudio>& GetSoundMap();

	private:
		/// FMOD 관련 변수들.
		FMOD::System*						_pSystem;				// FMOD 모든 리소스 재생 관리 객체
		FMOD::ChannelGroup*		 _pChannelMasterGroup;				// 오디오 채널 전체 그룹 (_pChannelGroup 묶음)
		std::vector<FMOD::ChannelGroup*>	_pChannelGroup;			// 오디오 채널 그룹

		std::unordered_map<std::string, RocketAudio> _soundMap;	// FMOD Sound 정보를 담고 있는 객체

		/// FMOD 제어에 필요한 변수들.
		unsigned int				_soundCountMax;				// 이 객체에 들어갈 Sound 의 최대 개수
		unsigned int				_channelGroupMax;			// 이 객체에서 생성할 ChannelGroup의 최대개수
		bool						_isCurrentPlay;				// 현재 재생중인가?

		/// FMOD 3D 제어에 필요한 변수들.
		const float					_distancefactor = 1.0f;		// 거리 계수
		FMOD_VECTOR					_objectSoundPos;			// Ojbect 3D Sound 위치
		FMOD_VECTOR					_objectSoundVel;			// Ojbect 3D Sound 속도
		FMOD_VECTOR					_listenerPos;				// listener의 위치 (FMOD_VECTOR)
		FMOD_VECTOR					_listenerForward;			// listener Forwrd
		FMOD_VECTOR					_listenerUp;				// listener Up
		Rocket::Transform*	_listenerTransform;			// listener의 Transform
	};
}
