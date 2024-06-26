﻿#pragma once
#include "../../../FloaterUtil/include/Timer.h"
#include "../../../FloaterMath/include/Vector2f.h"
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

	public:
		void Initialize();
		bool Update();
		void Finalize();

		Scene* GetScene(const std::wstring& sceneName);
		Scene* SetScene(Scene* scene);
		Scene* SetScene(const std::wstring& sceneName);
		bool AddScene(const std::wstring& sceneName, Scene* scene);
		Scene* GetCurrentScene();

		Vector2f GetWindowSize();
		void SetWindowTitle(const std::wstring& title);

		IRenderer* GetRenderer();
		PhysicsEngine* GetPhysicsEngine();
		SoundEngine* GetSoundEngine();
		Platform* GetPlatform();

		void ExitGame();

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
		Timer _timer;

		Timer _fixedUpdateTimer;
		float _fixedUpdateElapsedSecond;
	};
}
