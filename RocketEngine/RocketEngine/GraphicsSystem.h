#pragma once
#include "Singleton.h"
#include <windows.h>
#include <vector>
#include <memory>
#include <unordered_map>
#include "../GraphicsInterface/IDX11Renderer.h"
#include "../GraphicsInterface/IFactory.h"
#include "../GraphicsInterface/IGraphicsSystem.h"
#define GRAPHICSDLL_PATH (L"RocketDX11.dll")

namespace Rocket::Core
{
	class IRenderable;
	class ISketchable;
	class RenderConstantData;
}

namespace Rocket::Core
{
	class ModelRendererBase;
	class UIRenderer;
}

namespace Rocket
{
	class Light;
	class Component;
}

/// <summary>
/// 엔진에서 Graphics 모듈을 불러와 초기화시키고
/// 렌더링 컴포넌트를 총괄하여 그릴 수 있도록 관리하는 클래스.
/// 
/// 23.06.20 강석원 인재원.
/// </summary>
namespace Rocket::Core
{
	class GraphicsSystem : public Singleton<GraphicsSystem>, public IGraphicsSystem
	{
		friend Singleton;
	private:
		GraphicsSystem();		// 싱글턴이기 때문에 외부에서 생성할 수 없도록.

		/// 시스템 초기화 관련
	public:
		void Initialize(HWND hWnd, int screenWidth, int screenHeight, bool isEditor = false);
		void Finalize();

	private:
		bool _isEditor = false;

	public:
		void OnResize(int width, int height);

	public:
		void DestroyWindow();

		/// 렌더링 관련
	public:
		void DrawProcess();

 	private:
 		void UpdateRenderData();
// 		void UpdateConstantData(Rocket::Core::RenderConstantData& data);
// 		void DrawCurrentScene();
// 
// 	private:
// 		void DrawCurrentUI();

	public:
		int GetScreenWidth() const;
		int GetScreenHeight() const;
		IFactory* GetFactory() const;

		/// 기본 정보(윈도우 핸들, 윈도우 사이즈 등)
	private:
		HWND _hWnd;
		int _screenWidth;
		int _screenHeight;

		/// DLL 관련
	private:
		HMODULE hGraphicsModule;
		std::unique_ptr<Rocket::Core::IDX11Renderer> _rocketGraphics;
		std::unique_ptr<Rocket::Core::IFactory> _factory;

	public:
		void AddToList(Component* comp);

	private:
		std::vector<Component*> _renderableList;
	};
}
