#pragma once

/// <summary>
/// 팩토리 객체는 카메라, 메쉬, 라이트와 같은 렌더링에 필요한 객체들을 생성한다.
/// 2023.11.10 김민정
/// </summary>

namespace Rocket::Core
{
	class ICamera;
	class IMeshRenderer;
	class IDynamicModelRenderer;
	class ITextRenderer;
	class ISpriteRenderer;
	class ILineRenderer;
	class IResourceManager;
	struct CubePrimitive;
	struct SpherePrimitive;
	struct CylinderPrimitive;

	class IFactory
	{
	public:
		virtual ICamera* CreateCamera() = 0;
		virtual IMeshRenderer* CreateStaticModelRenderer() = 0;
		virtual IDynamicModelRenderer* CreateDynamicModelRenderer() = 0;
		virtual ITextRenderer* CreateTextRenderer() = 0;
		virtual ISpriteRenderer* CreateSpriteRenderer() = 0;
		virtual ILineRenderer* CreateLineRenderer() = 0;
		virtual CubePrimitive* CreateCubePrimitive() = 0;
		virtual SpherePrimitive* CreateSpherePrimitive() = 0;
		virtual CylinderPrimitive* CreateCylinderPrimitive() = 0;
	};

	/// dllexport 하는 오브젝트팩토리를 반환하는 팩토리 함수.

	extern "C" __declspec(dllexport) IFactory* CreateGraphicsObjectFactory();
	extern "C" __declspec(dllexport) void ReleaseFactory(IFactory* instance);
	extern "C" __declspec(dllexport) IResourceManager* GetResourceManager();
}
