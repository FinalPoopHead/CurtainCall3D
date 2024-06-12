#pragma once
#include "../../../FloaterUtil/include/Timer.h"
#include <unordered_set>
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

		Scene* SetScene(Scene* scene);
		void AddScene(Scene* scene);
		Scene* GetCurrentScene();

		IRenderer* GetRenderer();
		PhysicsEngine* GetPhysicsEngine();
		SoundEngine* GetSoundEngine();
		Platform* GetPlatform();

	private:
		void ChangeScene();

	private:
		static GameEngine* _instance;

	private:
		std::unique_ptr<Platform> _platform;
		std::unique_ptr<PhysicsEngine> _physicsEngine;
		std::unique_ptr<SoundEngine> _soundEngine;

		IRenderer* _renderer;
		//PhysicsEngine* _physicsEngine;
		//SoundEngine* _soundEngine;

		Scene* _nextScene;
		Scene* _currentScene;
		std::unordered_set<Scene*> _scenes;
		Timer _timer;

		Timer _fixedUpdateTimer;
		float _fixedUpdateElapsedSecond;
	};
}
