#pragma once
#include "../../../FloaterUtil/include/Timer.h"
#include <unordered_set>


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
		Platform* GetPlatform();

	protected:


	private:
		static GameEngine* _instance;

	private:
		Platform* _platform;
		IRenderer* _renderer;
		PhysicsEngine* _physicsEngine;
		SoundEngine* _soundEngine;


		Scene* _currentScene;
		std::unordered_set<Scene*> _scenes;
		Timer _timer;

		Timer _fixedUpdateTimer;
		float _fixedUpdateElapsedSecond;
	};
}
