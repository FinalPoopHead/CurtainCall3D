#include "ObjectFactory.h"
#include "ObjectManager.h"
#include "ResourceManager.h"

#include "Camera.h"
#include "MeshRenderer.h"
#include "DynamicModelRenderer.h"
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

	IResourceManager* GetResourceManager()
	{
		return &ResourceManager::Instance();
	}
}

namespace Rocket::Core
{
	ObjectFactory::ObjectFactory()
		: _objectManager(ObjectManager::Instance())
	{

	}

	ICamera* ObjectFactory::CreateCamera()
	{
		return _objectManager.CreateCamera();
	}

	IMeshRenderer* ObjectFactory::CreateStaticModelRenderer()
	{
		return _objectManager.CreateStaticModelRenderer();
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

	IDynamicModelRenderer* ObjectFactory::CreateDynamicModelRenderer()
	{
		return _objectManager.CreateDynamicModelRenderer();
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
