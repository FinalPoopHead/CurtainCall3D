#include "./include/Scene.h"

flt::Scene::Scene()
{

}

flt::Scene::~Scene()
{

}

void flt::Scene::Init()
{
	_timer.Start();
}

flt::GameObject& flt::Scene::CreateGameObject()
{
	_gameObjects.emplace_back();
	return _gameObjects.back();
}

void flt::Scene::DestroyGameObject(GameObject& gameObject)
{
	ASSERT(false, "Not implemented yet");
}

void flt::Scene::Update(float deltaSecond)
{
	for (auto& object : _gameObjects)
	{
		if (!object._isEnable)
		{
			continue;
		}

		for (auto& component : object._components)
		{
			if (component == nullptr)
			{
				continue;
			}
			if (!component->_isEnable)
			{
				continue;
			}

			component->Update(deltaSecond);
		}
	}

	
}

void flt::Scene::EndRender()
{
	for (auto& object : _gameObjects)
	{
		if (!object._isEnable)
		{
			continue;
		}

		for (auto& component : object._components)
		{
			if (component == nullptr)
			{
				continue;
			}
			if (!component->_isEnable)
			{
				continue;
			}

			component->EndDraw();
		}
	}
}

void flt::Scene::EndFrame()
{
	for (auto& [object, isEnable] : _gameObjectsToEnable)
	{
		if (object->_isEnable != isEnable)
		{
			object->_isEnable = isEnable;
		}
	}

	for (auto& [component, isEnable] : _componentsToEnable)
	{
		if (component->_isEnable != isEnable)
		{
			component->_isEnable = isEnable;
			if (isEnable)
			{
				component->OnEnable();
			}
			else
			{
				component->OnDisable();
			}
		}
	}
}
