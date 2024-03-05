#pragma once
#include <unordered_map>
#include <string>
#include "DLLExporter.h"
#include "Component.h"
#include "eSoundGroup.h"

namespace Rocket::Core
{
	class SoundSystem;
}

namespace Rocket
{
	struct AudioStatus
	{
		std::string path;
		bool isPlaying;
	};

	class GameObject;

	/// <summary>
	/// User가 접근 및 사용가능한 함수와 정보를 담은 class.
	/// 
	/// </summary>
	class ROCKET_API AudioClip final : public Component
	{
	public:
		AudioClip();

	protected:
		virtual void Start() override;

		/// 재생관련.
	public:
		// 2D Sound 함수.
		void PlayOnce(std::string audioName);		// 한번만 재생
		void PlayRepeat(std::string audioName);		// 반복 재생

		// 3D Sound 함수.
		void Play3DOnce(std::string audioName);		// 한번만 재생
		void Play3DRepeat(std::string audioName);	// 반복 재생

		// 공통 함수.
		void Allplay();								// 모든 Sound 재생
		void PlayEnd(std::string audioName);		// Sound 종료
		void PlayAllEnd();							// Sound All 종료

		/// Sound 설정
		// 개별 Sound
		void PlayMute(std::string audioName);
		void PlayPaused(std::string audioName);
		void SoundVolumeSet(std::string audioName);
		
		bool IsSoundPlaying(std::string audioName);

		// Group 관련 함수
		void PlayGroupMute(std::string audioName);			// Sound OnOff
		void PlayGroupAllMute();							// Sound All OnOff
		void PlayGroupPaused(eSoundGroup soundGroupName);	// Sound 일시정지
		void PlayGroupAllPaused();							// Sound All 일시정지
		void SoundGroupVolumeSet(eSoundGroup soundGroupName);	// Sound 봄륨 설정
		void SoundGroupAllVolumeSet();						// Sound All 봄륨 설정

		/// 오디오 추가 관련.
	public:
		// Audio 생성
		void AddAudio(std::string audioName, std::string audioPath, eSoundGroup group);		// 2D Sound 추가
		void Add3DAudio(std::string audioName, std::string audioPath, eSoundGroup group,
			float minDistance, float maxDistance);		// 3D Sound 추가 

		/// 오디오 제어에 필요한 변수들.
	public:
		float GetVolume();
		std::unordered_map<std::string, std::string>& GetAudios();

	private:
		Rocket::Core::SoundSystem& _soundSystem;							// FMOD System

		std::unordered_map<std::string, std::string> _audioMap;			// audio이름, audioPath
		float _volume = 1.f;													// Sound 봄륨 크기
	};
}

// Play(선택해서) -> 여러 사운드 받을 수 있어야 함
// 일시정지
// 정지
// 루프여부
// 종류설정 그룹짓기? -> 시스템에 등록할때
