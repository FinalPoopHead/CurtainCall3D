/// 클라이언트에게 엔진의 기능을 제공하기 위해 한번 랩핑하여 export 하는 코드.
/// 클라이언트에게 Open하고 싶은 부분만 제공할 수 있도록 했다.
///
/// 23.07.07 강석원 인재원.

#pragma once
#include <string>
#include "DLLExporter.h"
#include "MathHeader.h"

// 여기에 클라이언트에서 쓸 클래스들 참조해주고 (상속을위한?)
/// 엔진 생성 팩토리
#include "EngineProcess.h"

/// 객체 생성 팩토리.
#include "GameObject.h"
#include "Scene.h"

/// 기능 제공.
// 기능제공은 cpp에서 래핑.
#include "eSoundGroup.h"

/// 그래픽스 enum.
#include "../RocketCommon/GraphicsEnum.h"

/// 클래스 export.
// 대체로 컴포넌트.
#include "Component.h"
#include "Camera.h"
#include "MeshRenderer.h"
#include "DynamicModelRenderer.h"
#include "Transform.h"
#include "AnimationController.h"
#include "Animator.h"
#include "State.h"
#include "Tween.h"
#include "DynamicCollider.h"
#include "BoxCollider.h"
#include "SphereCollider.h"
#include "CapsuleCollider.h"
#include "PlaneCollider.h"
#include "StaticBoxCollider.h"
#include "TextBox.h"
#include "SpriteRenderer.h"
#include "Button.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "AudioClip.h"

namespace Rocket
{
	/// 엔진 생성 팩토리.
	ROCKET_API EngineProcess* CreateLauncher();
	ROCKET_API void ReleaseLauncher(IEngineProcess* instance);

	/// 에디터 정보 로드.
	ROCKET_API void LoadSceneFromJSON(std::string sceneDataPath);

	/// 객체 생성 팩토리.
	ROCKET_API GameObject* CreateObject(const std::string& objName);
	ROCKET_API GameObject* CreateStaticObject(const std::string& objName);
	ROCKET_API GameObject* CreateModelObject(const std::string& fileName);
	ROCKET_API Scene* CreateScene(const std::string& sceneName, bool withNoCam = false);
	ROCKET_API Tween* CreateTween();
	ROCKET_API GameObject* CreateButton(const std::string& objName);
	ROCKET_API GameObject* CreateImageBox(const std::string& objName);
	ROCKET_API GameObject* CreateTextBox(const std::string& objName);

	/// 객체 반환. (객체의 주소 반환)
	ROCKET_API Camera* GetMainCamera();

	/// 메인 카메라 변경.
	ROCKET_API void SetMainCamera(Camera* camera);

	/// 기능 제공.
	ROCKET_API void RocketDestroyWindow();						// 윈도우 종료.
	ROCKET_API bool LoadScene(const std::string& sceneName);
	ROCKET_API float GetDeltaTime();
	ROCKET_API bool GetKeyDown(int keyCode);
	ROCKET_API bool GetKeyUp(int keyCode);
	ROCKET_API bool GetKey(int keyCode);
	ROCKET_API Vector2 GetMousePosition();
	ROCKET_API Vector2 GetMouseDelta();
	ROCKET_API Vector2 MouseDeltaSmooth();
	ROCKET_API void AddStaticComponent(Component* component);
	ROCKET_API float GetMouseSensitivity();
	ROCKET_API void SetMouseSensitivity(float sensitivity);
	ROCKET_API GameObject* FindObjectByName(std::string name);

	/// 물리 기능 제공.
	ROCKET_API Collider* ShootRay(Vector3 origin, Vector3 direction, float length = 100.0f, int* type = nullptr);

	/// 사운드 관련.
	ROCKET_API float GetGroupVolume(eSoundGroup group);
	ROCKET_API void SetGroupVolume(eSoundGroup group, float volume);
	ROCKET_API void AddAudio(const std::string& audioPath, eSoundGroup group);
	ROCKET_API void PlayAudioOnce(const std::string& audioPath, float volume);
	ROCKET_API void PlayAudioOnceNested(const std::string& audioPath, float volume);
	ROCKET_API void PlayAudioRepeat(const std::string& audioPath, float volume);
	ROCKET_API void PlayEnd(std::string soundPath);
	ROCKET_API void PlayAllEnd();

	/// 디버그 관련.
	//ROCKET_API void ToggleDebugMode();
	ROCKET_API void DrawDebugText(Vector2 centerPos, std::string content, float size);
	ROCKET_API void DrawDebugBox(Matrix worldTM, Vector3 widthHeightDepth = { 1.0f,1.0f,1.0f }, bool isWire = true, Vector4 color = { 1.0f,0.0f,0.0f,1.0f });
	ROCKET_API void DrawDebugLine(Vector3 beginPoint, Vector3 endPoint, Vector4 color = { 1.0f,0.0f,0.0f,1.0f });
	ROCKET_API void DrawDebug2DBox(Vector2 LT, Vector2 RB, Vector4 color = { 1.0f,0.0f,0.0f,1.0f });


}
