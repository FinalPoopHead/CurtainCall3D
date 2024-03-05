#include "RocketAPI.h"

#include <windows.h>

#include "ObjectSystem.h"
#include "SceneSystem.h"
#include "TimeSystem.h"
#include "TweenSystem.h"
#include "InputSystem.h"
#include "GraphicsSystem.h"
#include "DebugSystem.h"
#include "PhysicsSystem.h"
#include "DataSystem.h"
#include "SoundSystem.h"

constexpr const char* DEFAULT_IMAGE_PATH = "..\\GraphicsResources\\Images\\luffy.jpg";

namespace Rocket
{

	EngineProcess* CreateLauncher()
	{
		return new EngineProcess();
	}

	void ReleaseLauncher(IEngineProcess* instance)
	{
		delete instance;
	}

	void LoadSceneFromJSON(std::string sceneDataPath)
	{
		//Rocket::Core::DataSystem::Instance().SetAllDataToScene(sceneDataPath);
	}

	GameObject* CreateObject(const std::string& objName)
	{
		return Rocket::Core::SceneSystem::Instance().GetCurrentScene()->CreateObject(objName);
	}

	GameObject* CreateStaticObject(const std::string& objName)
	{
		return Rocket::Core::ObjectSystem::Instance().CreateStaticObject(objName);

	}

	ROCKET_API Scene* CreateScene(const std::string& sceneName, bool withNoCam /*= false*/)
	{
		return Rocket::Core::SceneSystem::Instance().CreateScene(sceneName, withNoCam);
	}

	Tween* CreateTween()
	{
		return &(Rocket::Core::TweenSystem::Instance().CreateTween());
	}

	GameObject* CreateButton(const std::string& objName)
	{
		GameObject* button = Rocket::Core::SceneSystem::Instance().GetCurrentScene()->CreateObject(objName);
		button->AddComponent<Button>();
		SpriteRenderer* buttonRenderer = button->AddComponent<SpriteRenderer>();
		buttonRenderer->SetPath(DEFAULT_IMAGE_PATH);
		return button;
	}

	GameObject* CreateImageBox(const std::string& objName)
	{
		GameObject* imageBox = Rocket::Core::SceneSystem::Instance().GetCurrentScene()->CreateObject(objName);
		SpriteRenderer* imageRenderer = imageBox->AddComponent<SpriteRenderer>();
		imageRenderer->SetPath(DEFAULT_IMAGE_PATH);
		return imageBox;
	}

	GameObject* CreateTextBox(const std::string& objName)
	{
		GameObject* textBox = Rocket::Core::SceneSystem::Instance().GetCurrentScene()->CreateObject(objName);
		SpriteRenderer* imageRenderer = textBox->AddComponent<SpriteRenderer>();
		TextBox* textRenderer = textBox->AddComponent<TextBox>();

		imageRenderer->SetPath(DEFAULT_IMAGE_PATH);
		textRenderer->SetText("기본 텍스트 입니다.");
		textRenderer->SetSortOrder(1);

		return textBox;
	}

	Camera* GetMainCamera()
	{
		return Rocket::Core::SceneSystem::Instance().GetCurrentScene()->GetMainCamera();
	}

	void SetMainCamera(Camera* camera)
	{
		Rocket::Core::SceneSystem::Instance().GetCurrentScene()->SetMainCamera(camera);
	}

	void RocketDestroyWindow()
	{
		Rocket::Core::GraphicsSystem::Instance().DestroyWindow();
	}

	bool LoadScene(const std::string& sceneName)
	{
		return Rocket::Core::SceneSystem::Instance().LoadScene(sceneName);
	}

	float GetDeltaTime()
	{
		return Rocket::Core::TimeSystem::GetDeltaTime();
	}

	bool GetKeyDown(int keyCode)
	{
		return Rocket::Core::InputSystem::Instance().GetKeyDown(keyCode);
	}

	bool GetKeyUp(int keyCode)
	{
		return Rocket::Core::InputSystem::Instance().GetKeyUp(keyCode);
	}

	bool GetKey(int keyCode)
	{
		return Rocket::Core::InputSystem::Instance().GetKey(keyCode);
	}

	Vector2 GetMousePosition()
	{
		return Rocket::Core::InputSystem::Instance().GetMousePosition();
	}

	Vector2 GetMouseDelta()
	{
		return Rocket::Core::InputSystem::Instance().GetMouseDelta();
	}

	ROCKET_API Vector2 MouseDeltaSmooth()
	{
		return Rocket::Core::InputSystem::Instance().MouseDeltaSmooth();
	}

	void AddStaticComponent(Component* component)
	{
		Rocket::Core::ObjectSystem::Instance().AddStaticComponent(component);
	}

	float GetMouseSensitivity()
	{
		return Rocket::Core::InputSystem::Instance().GetMouseSensitivity();
	}

	void SetMouseSensitivity(float sensitivity)
	{
		Rocket::Core::InputSystem::Instance().SetMouseSensitivity(sensitivity);
	}

	GameObject* FindObjectByName(std::string name)
	{
		return Rocket::Core::SceneSystem::Instance().GetCurrentScene()->FindObjectByName(name);
	}

	ROCKET_API Collider* ShootRay(Vector3 origin, Vector3 direction, float length /*= 100.0f*/, int* type /*= nullptr*/)
	{
		// type이 1이면 rigidStatic, 2면 rigidDynamic
		return Rocket::Core::PhysicsSystem::Instance().RayCast(origin, direction, length, type);
	}

// 	void ToggleDebugMode()
// 	{
// 		Rocket::Core::DebugSystem& instance = Rocket::Core::DebugSystem::Instance();
// 		instance.SetDebugMode(!instance.IsDebugMode());
// 	}

	float GetGroupVolume(eSoundGroup group)
	{
		return Rocket::Core::SoundSystem::Instance().GetGroupVolume(group);
	}

	void SetGroupVolume(eSoundGroup group, float volume)
	{
		Rocket::Core::SoundSystem::Instance().SetGroupVolume(group, volume);
	}

	void AddAudio(const std::string& audioPath, eSoundGroup group)
	{
		Rocket::Core::SoundSystem::Instance().CreateSound(audioPath, group);
	}

	void PlayAudioOnce(const std::string& audioPath, float volume)
	{
		if (Rocket::Core::SoundSystem::Instance().IsSoundPlaying(audioPath))
		{
			return;
		}

		Rocket::Core::SoundSystem::Instance().PlayOnce(audioPath);
	}

	ROCKET_API void PlayAudioOnceNested(const std::string& audioPath, float volume)
	{
		Rocket::Core::SoundSystem::Instance().PlayOnce(audioPath);
	}

	ROCKET_API void PlayAudioRepeat(const std::string& audioPath, float volume)
	{
		if (Rocket::Core::SoundSystem::Instance().IsSoundPlaying(audioPath))
		{
			return;
		}

		Rocket::Core::SoundSystem::Instance().RepeatPlayback(audioPath);
	}

	ROCKET_API void PlayEnd(std::string soundPath)
	{
		Rocket::Core::SoundSystem::Instance().PlayEnd(soundPath);
	}

	ROCKET_API void PlayAllEnd()
	{
		Rocket::Core::SoundSystem::Instance().PlayAllEnd();
	}

	void DrawDebugText(Vector2 centerPos, std::string content, float size)
	{
		Rocket::Core::DebugSystem::Instance().DrawDebugText(centerPos, content, size);
	}

	void DrawDebugBox(Matrix worldTM, Vector3 widthHeightDepth /*= { 1.0f,1.0f,1.0f }*/, bool isWire /*= true*/, Vector4 color /*= { 1.0f,0.0f,0.0f,0.0f }*/)
	{
		Rocket::Core::DebugSystem::Instance().DrawDebugBox(worldTM, widthHeightDepth, isWire, color);
	}

	void DrawDebugLine(Vector3 beginPoint, Vector3 endPoint, Vector4 color /*= { 1.0f,0.0f,0.0f,1.0f }*/)
	{
		Rocket::Core::DebugSystem::Instance().DrawDebugLine(beginPoint, endPoint, color);
	}

	void DrawDebug2DBox(Vector2 LT, Vector2 RB, Vector4 color /*= { 1.0f,0.0f,0.0f,1.0f }*/)
	{
		Rocket::Core::DebugSystem::Instance().DrawDebug2DBox(LT, RB, color);
	}


}
