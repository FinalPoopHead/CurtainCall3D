﻿#include "./include/internal/Scene.h"


flt::Scene::Scene() :
	_gameObjects(),
	_gameObjectsToEnable(),
	_componentsToEnable()
{
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

//void flt::Scene::CreateGameObject(GameObject* gameObject)
//{
//	_gameObjects.emplace_back(gameObject);
//	if (gameObject->_isEnable)
//	{
//		_gameObjectsToEnable.emplace_back(gameObject, gameObject->_isEnable);
//
//		for (auto& component : gameObject->_components)
//		{
//			if (component == nullptr)
//			{
//				continue;
//			}
//
//			_componentsToEnable.emplace_back(component, true);
//		}
//	}
//}

std::vector<flt::GameObject*> flt::Scene::GetGameObjects(const std::wstring& name) const
{
	std::vector<GameObject*> result;
	for (const auto& object : _gameObjects)
	{
		if (object->name == name)
		{
			result.emplace_back(object);
		}
	}

	return result;
}

void flt::Scene::AddEnableGameObject(GameObject* gameObject, bool isEnable)
{
	{
		bool isExist = false;
		for (auto& object : _gameObjects)
		{
			if (object == gameObject)
			{
				isExist = true;
				break;
			}
		}
		for (auto& object : _gameObjectsToCreate)
		{
			if (object == gameObject)
			{
				isExist = true;
				break;
			}
		}
		ASSERT(isExist, "Not exist game object");
	}

	if (isEnable)
	{
		_gameObjectsToEnable.emplace_back(gameObject);
	}
	else
	{
		_gameObjectsToDisable.emplace_back(gameObject);
	}
}

void flt::Scene::AddEnableComponent(ComponentBase* component, bool isEnable)
{
	if (isEnable)
	{
		_componentsToEnable.emplace_back(component);
	}
	else
	{
		_componentsToDisable.emplace_back(component);
	}
}

void flt::Scene::AddDestroyGameObject(GameObject* gameObject)
{
	bool isExist = false;
	for (auto& object : _gameObjects)
	{
		if (object == gameObject)
		{
			isExist = true;
			break;
		}
	}
	ASSERT(isExist, "Not exist game object");

	_gameObjectsToDestroy.push_back(gameObject);
}

void flt::Scene::CallCollisionEvent()
{
	//bool nextFlag = !_collisionFlag;
	//for (auto& collisionPair : _collisionPairs)
	//{
	//	// TODO : 충돌한 페이를 구분하기 위한 키값을 만들어야 함.
	//	//uint64 key = collisionPair.obj1;
	//	auto iter = _collisionSet.find(collisionPair);
	//	if (iter == _collisionSet.end())
	//	{
	//		for (auto& component : collisionPair.obj1->_components)
	//		{
	//			if (component == nullptr)
	//			{
	//				continue;
	//			}
	//			if (!component->_isEnable)
	//			{
	//				continue;
	//			}
	//			component->OnCollisionEnter(collisionPair.collider2);
	//		}

	//		for (auto& component : collisionPair.obj2->_components)
	//		{
	//			if (component == nullptr)
	//			{
	//				continue;
	//			}
	//			if (!component->_isEnable)
	//			{
	//				continue;
	//			}
	//			component->OnCollisionEnter(collisionPair.collider1);
	//		}

	//		collisionPair.flag = nextFlag;
	//		_collisionSet.insert(collisionPair);
	//	}
	//}

	//for (auto& collisionPair : _collisionSet)
	//{
	//	if (collisionPair.flag == _collisionFlag)
	//	{
	//		for (auto& component : collisionPair.obj1->_components)
	//		{
	//			if (component == nullptr)
	//			{
	//				continue;
	//			}
	//			if (!component->_isEnable)
	//			{
	//				continue;
	//			}
	//			component->OnCollisionStay(collisionPair.collider2);
	//		}
	//		for (auto& component : collisionPair.obj2->_components)
	//		{
	//			if (component == nullptr)
	//			{
	//				continue;
	//			}
	//			if (!component->_isEnable)
	//			{
	//				continue;
	//			}
	//			component->OnCollisionStay(collisionPair.collider1);
	//		}
	//		_collisionSet.erase(collisionPair);
	//	}
	//	else
	//	{
	//		for (auto& component : collisionPair.obj1->_components)
	//		{
	//			if (component == nullptr)
	//			{
	//				continue;
	//			}
	//			if (!component->_isEnable)
	//			{
	//				continue;
	//			}
	//			component->OnCollisionExit(collisionPair.collider2);
	//		}
	//		for (auto& component : collisionPair.obj2->_components)
	//		{
	//			if (component == nullptr)
	//			{
	//				continue;
	//			}
	//			if (!component->_isEnable)
	//			{
	//				continue;
	//			}
	//			component->OnCollisionExit(collisionPair.collider1);
	//		}

	//		_collisionSet.erase(collisionPair);
	//	}
	//}

	//_collisionFlag = nextFlag;
	//_collisionPairs.clear();
}

void flt::Scene::PrePhysicsUpdate()
{
	for (auto& object : _gameObjects)
	{
		if (!object->_isEnable)
		{
			continue;
		}

		//object->FixedUpdate();

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

			component->PrePhysics();
		}
	}
}

void flt::Scene::PostPhysicsUpdate()
{
	// Collider Event
	CallCollisionEvent();

	// postPhysics
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

			component->PostPhysics();
		}
	}

	// FixedUpdate
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

			component->FixedUpdate();
		}
		object->FixedUpdate();
	}
}

void flt::Scene::Update(float deltaSecond)
{
	for (auto& object : _gameObjects)
	{
		if (!object->_isEnable)
		{
			continue;
		}

		object->PreUpdate(deltaSecond);

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

			component->PreUpdate(deltaSecond);
		}
	}

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

	for (auto& object : _gameObjects)
	{
		if (!object->_isEnable)
		{
			continue;
		}

		object->PostUpdate(deltaSecond);

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

			component->PostUpdate(deltaSecond);
		}
	}
}

void flt::Scene::PreRender()
{
	for (auto& object : _gameObjects)
	{
		if (!object->_isEnable)
		{
			continue;
		}

		object->PreRender();

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

			component->PreRender();
		}
	}
}

void flt::Scene::PostRender()
{
	for (auto& object : _gameObjects)
	{
		if (!object->_isEnable)
		{
			continue;
		}

		object->PostRender();

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

			component->PostRender();
		}
	}
}

void flt::Scene::StartFrame()
{
	while (!_gameObjectsToCreate.empty())
	{
		GameObject* object = _gameObjectsToCreate.back();
		_gameObjectsToCreate.pop_back();

		_gameObjects.emplace_back(object);

		for (auto& component : object->_components)
		{
			if (component == nullptr)
			{
				continue;
			}

			component->OnCreate();
		}
		object->OnCreate();

		if (object->_isEnable)
		{
			object->_isEnable = false;
			_gameObjectsToEnable.emplace_back(object);
		}
	}

	while (!_gameObjectsToEnable.empty())
	{
		GameObject* object = _gameObjectsToEnable.back();
		_gameObjectsToEnable.pop_back();

		if (object->_isEnable == true)
		{
			continue;
		}

		object->_isEnable = true;

		for (auto& component : object->_components)
		{
			if (component == nullptr)
			{
				continue;
			}

			if (component->_isEnable)
			{
				component->OnEnable();
			}
		}
		object->OnEnable();
	}

	while (!_componentsToEnable.empty())
	{
		ComponentBase* component = _componentsToEnable.back();
		_componentsToEnable.pop_back();

		if (component->_isEnable == true)
		{
			continue;
		}

		component->_isEnable = true;
		component->OnEnable();
	}

	// 비활성화 처리. 한 프레임 내에 enable, disable 이 여러번 불린다면 diable이 되어야 하며, update는 호출되지 않는다.
	while (!_gameObjectsToDisable.empty())
	{
		GameObject* object = _gameObjectsToDisable.back();
		_gameObjectsToDisable.pop_back();

		//이미 이 전 상태와 같다면 패스.
		if (object->_isEnable == false)
		{
			continue;
		}

		object->_isEnable = false;

		for (auto& component : object->_components)
		{
			if (component == nullptr)
			{
				continue;
			}

			if (component->_isEnable)
			{
				component->OnDisable();
			}
		}
		object->OnDisable();
	}

	for (int i = 0; i < _gameObjectsToDestroy.size(); ++i)
	{
		GameObject* object = _gameObjectsToDestroy[i];

		//이미 이 전 상태와 같다면 패스.
		if (object->_isEnable == false)
		{
			continue;
		}

		object->_isEnable = false;

		for (auto& component : object->_components)
		{
			if (component == nullptr)
			{
				continue;
			}

			if (component->_isEnable)
			{
				component->OnDisable();
			}
		}
		object->OnDisable();
	}

	while (!_componentsToDisable.empty())
	{
		ComponentBase* component = _componentsToDisable.back();
		_componentsToDisable.pop_back();

		if (component->_isEnable == false)
		{
			continue;
		}

		component->_isEnable = false;
		component->OnDisable();
	}

	// Destroy 처리
	while (!_gameObjectsToDestroy.empty())
	{
		GameObject* object = _gameObjectsToDestroy.back();
		_gameObjectsToDestroy.pop_back();

		for (auto& component : object->_components)
		{
			if (component == nullptr)
			{
				continue;
			}

			// Disable인 컴포넌트들도 Destroy해야하는가..?
			component->OnDestroy();
			delete component;
		}
		object->OnDestroy();

		_gameObjects.erase(std::remove(_gameObjects.begin(), _gameObjects.end(), object), _gameObjects.end());
		delete object;
	}
}


void flt::Scene::EndFrame()
{

}

void flt::Scene::StartScene()
{

}

void flt::Scene::EndScene()
{
	for (auto& object : _gameObjects)
	{
		if (object->_isEnable == false)
		{
			continue;
		}

		for (auto& component : object->_components)
		{
			if (component == nullptr)
			{
				continue;
			}

			if (component->_isEnable == false)
			{
				continue;
			}

			component->OnDisable();
		}
		object->OnDisable();
	}

	for (auto& object : _gameObjects)
	{
		for (auto& component : object->_components)
		{
			if (component == nullptr)
			{
				continue;
			}

			component->OnDestroy();
		}
		object->OnDestroy();
	}

	while (!_gameObjects.empty())
	{
		GameObject* object = _gameObjects.back();

		for (auto& component : object->_components)
		{
			if (component == nullptr)
			{
				continue;
			}
			delete component;
			//component = nullptr;
		}
		delete object;
		//object = nullptr;

		_gameObjects.pop_back();
	}

	while (!_gameObjectsToCreate.empty())
	{
		GameObject* object = _gameObjectsToCreate.back();
		_gameObjectsToCreate.pop_back();

		for (auto& component : object->_components)
		{
			if (component == nullptr)
			{
				continue;
			}
			delete component;
		}
		delete object;
	}


	_gameObjectsToEnable.clear();
	_gameObjectsToDisable.clear();
	_gameObjectsToDestroy.clear();
	_componentsToEnable.clear();
	_componentsToDisable.clear();
	_collisionPairs.clear();

	/*for (auto& object : _gameObjects)
	{
		if (object->_isEnable == false)
		{
			continue;
		}

		for (auto& component : object->_components)
		{
			if (component == nullptr)
			{
				continue;
			}

			if (component->_isEnable == false)
			{
				continue;
			}

			component->OnDisable();
		}
		object->OnDisable();
	}

	for (auto& object : _gameObjects)
	{
		for (auto& component : object->_components)
		{
			if (component == nullptr)
			{
				continue;
			}

			component->OnDestroy();
		}
		object->OnDestroy();
	}

	for (auto& object : _gameObjects)
	{
		for (auto& component : object->_components)
		{
			if (component == nullptr)
			{
				continue;
			}
			delete component;
		}
		delete object;
	}
	_gameObjects.clear();*/
}
