#pragma once
#include "..\\GraphicsInterface\\IFactory.h"

namespace Rocket::Core
{
	class ObjectManager;

	class ObjectFactory : public Rocket::Core::IFactory
	{
	public:
		ObjectFactory();

	public:
		virtual ITransform* CreateTransform() override;
		virtual ICamera* CreateCamera() override;
		virtual IMeshRenderer* CreateMeshRenderer() override;
		virtual ITextRenderer* CreateTextRenderer() override;
		virtual ISpriteRenderer* CreateSpriteRenderer() override;
		virtual ILineRenderer* CreateLineRenderer() override;

		// IFactory을(를) 통해 상속됨
		virtual ISkinnedMesh* CreateSkinnedMeshObject() override;
		virtual CubePrimitive* CreateCubePrimitive() override;
		virtual SpherePrimitive* CreateSpherePrimitive() override;
		virtual CylinderPrimitive* CreateCylinderPrimitive() override;

	private:
		ObjectManager& _objectManager;
	};
}

