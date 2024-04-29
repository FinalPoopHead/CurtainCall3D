#include "./include/Scene.h"

flt::Scene::Scene()
{
	_timer.Start();
}

flt::Scene::~Scene()
{

}

void flt::Scene::Initialize()
{

}

void flt::Scene::Finalize()
{

}

flt::GameObject* flt::Scene::CreateGameObject()
{
	_gameObjects.emplace_back(new(std::nothrow) GameObject());
	return _gameObjects.back();
}

void flt::Scene::AddGameObject(GameObject* gameObject)
{
	_gameObjects.emplace_back(gameObject);
	if(gameObject->_isEnable)
	{
		_gameObjectsToEnable.emplace_back(gameObject, gameObject->_isEnable);

		for(auto& component : gameObject->_components)
		{
			if(component == nullptr)
			{
				continue;
			}

			_componentsToEnable.emplace_back(component, true);
		}
	}
}

void flt::Scene::DestroyGameObject(GameObject& gameObject)
{
	ASSERT(false, "Not implemented yet");
}

void flt::Scene::Update(float deltaSecond)
{
	for (auto& object : _gameObjects)
	{
		if (!object->_isEnable)
		{
			continue;
		}

		object->Update(deltaSecond);

		for (auto& component : object->_components)
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
		if (!object->_isEnable)
		{
			continue;
		}

		for (auto& component : object->_components)
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

void flt::Scene::StartFrame()
{
	for (auto& [object, isEnable] : _gameObjectsToEnable)
	{
		if (object->_isEnable != isEnable)
		{
			object->_isEnable = isEnable;
		}
	}

	_gameObjectsToEnable.clear();

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

	_componentsToEnable.clear();
}
