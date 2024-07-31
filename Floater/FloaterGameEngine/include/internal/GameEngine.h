#pragma once
#include "../../../FloaterUtil/include/Timer.h"
#include "../../../FloaterMath/include/Vector2f.h"
#include "../../../FloaterPlatform/include/EnumType.h"
#include <unordered_map>
#include <memory>


namespace flt
{
	class Platform;
	class IRenderer;
	class Scene;
	class PhysicsEngine;
	class SoundEngine;

	class GameEngine
	{
		friend class Scene;
		friend class RendererComponent;
		friend class CameraComponent;

	private:
		GameEngine();
		~GameEngine();
	
	public:
		static GameEngine* Instance();
		static GameEngine* Instance(uint32 handle);

	public:
		void Initialize();
		void Initialize(uint32 handle);
		bool Update();
		void Finalize();

		Scene* GetScene(const std::wstring& sceneName);
		Scene* SetScene(Scene* scene);
		Scene* SetScene(const std::wstring& sceneName);
		bool AddScene(const std::wstring& sceneName, Scene* scene);
		Scene* GetCurrentScene();

		uint32 GetWindowHandle();
		Vector2f GetWindowSize();
		void SetWindowTitle(const std::wstring& title);
		void SetWindowSize(uint32 width = 0, uint32 height = 0, WindowMode mode = WindowMode::BORDERLESS);

		IRenderer* GetRenderer();
		PhysicsEngine* GetPhysicsEngine();
		SoundEngine* GetSoundEngine();
		Platform* GetPlatform();

		void ExitGame();

		float GetTimeScale() const;
		float SetTimeScale(float timeScale);

	private:
		void ChangeScene();
		bool UpdateImpl(Scene* scene);

	private:
		static GameEngine* s_instance;

	private:
		std::unique_ptr<Platform> _platform;
		std::unique_ptr<PhysicsEngine> _physicsEngine;
		std::unique_ptr<SoundEngine> _soundEngine;

		IRenderer* _renderer;

		Scene* _loadingScene;
		Scene* _nextScene;
		Scene* _currentScene;
		std::unordered_map<std::wstring, Scene*> _scenes;

		float _timeScale;
		Timer _timer;
		float _fixedUpdateElapsedSecond;
		//Timer _fixedUpdateTimer;
	};
}
