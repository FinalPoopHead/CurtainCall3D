#pragma once

namespace flt
{
	class Platform;
	class IRenderer;

	class GameEngine
	{
	private:
		GameEngine();
		~GameEngine();
	
	public:
		static GameEngine* Instance();
		static IRenderer* GetRenderer();

	public:
		void GetKey();

	private:
		static GameEngine* _instance;

	private:
		Platform* _platform;
		IRenderer* _renderer;
	};

}
