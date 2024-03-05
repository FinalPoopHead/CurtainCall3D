#include "ObjectFactory.h"
#include "ObjectManager.h"

#include "Transform.h"
#include "Camera.h"
#include "MeshRenderer.h"
#include "CubeMesh.h"
#include "TextRenderer.h"
#include "SpriteRenderer.h"
#include "LineRenderer.h"

namespace Rocket::Core
{
	IFactory* CreateGraphicsObjectFactory()
	{
		return new ObjectFactory();
	}

	void ReleaseFactory(IFactory* instance)
	{
		delete instance;
	}
}

namespace Rocket::Core
{
	ObjectFactory::ObjectFactory()
		: _objectManager(ObjectManager::Instance())
	{

	}

	ITransform* ObjectFactory::CreateTransform()
	{
		return new Transform();
	}

	ICamera* ObjectFactory::CreateCamera()
	{
		return _objectManager.CreateCamera();
	}

	IMeshRenderer* ObjectFactory::CreateMeshRenderer()
	{
		return _objectManager.CreateMeshRenderer();
	}

	ITextRenderer* ObjectFactory::CreateTextRenderer()
	{
		return _objectManager.CreateText();
	}

	ISpriteRenderer* ObjectFactory::CreateSpriteRenderer()
	{
		return _objectManager.CreateImage();
	}

	ILineRenderer* ObjectFactory::CreateLineRenderer()
	{
		return _objectManager.CreateLineRenderer();
	}

	ISkinnedMesh* ObjectFactory::CreateSkinnedMeshObject()
	{
		return nullptr;
	}

	CubePrimitive* ObjectFactory::CreateCubePrimitive()
	{
		return nullptr;
	}

	SpherePrimitive* ObjectFactory::CreateSpherePrimitive()
	{
		return nullptr;
	}

	CylinderPrimitive* ObjectFactory::CreateCylinderPrimitive()
	{
		return nullptr;
	}
}
