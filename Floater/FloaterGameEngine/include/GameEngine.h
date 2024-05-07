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
		void Update();
		void Finalize();

		void SetScene(Scene* scene);

		IRenderer* GetRenderer();
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
	};
}
