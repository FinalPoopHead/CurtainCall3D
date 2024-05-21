#pragma once
#include "../../FloaterUtil/include/Timer.h"

namespace flt
{
	class Platform;
	class IRenderer;
	class Scene;
	class PhysicsEngine;

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

		void SetScene(Scene* scene);

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

		Scene* _currentScene;
		Timer _timer;

		Timer _fixedUpdateTimer;
		float _fixedUpdateElapsedSecond;
	};
}
