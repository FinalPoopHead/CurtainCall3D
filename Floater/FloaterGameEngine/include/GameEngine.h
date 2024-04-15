#pragma once
#include "../../FloaterUtil/include/Timer.h"

namespace flt
{
	class Platform;
	class IRenderer;
	class Scene;

	class GameEngine
	{
		friend class Scene;
		friend class Renderer;

	private:
		GameEngine();
		~GameEngine();
	
	public:
		static GameEngine* Instance();


	public:
		void Init();
		void Update();

	protected:
		IRenderer* GetRenderer();

	private:
		static GameEngine* _instance;

	private:
		Platform* _platform;
		IRenderer* _renderer;

		Scene* _currentScene;
		Timer _timer;
	};
}
