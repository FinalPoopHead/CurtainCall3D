#include "./include/internal/Scene.h"


flt::Scene::Scene() :
	_gameObjects(),
	_activeGameObjects(),
	_stagingActiveGameObjects(),
	_gameObjectsToCreate(),
	_gameObjectsToDestroy(),
	_componentsToEnable(),
	_componentsToDisable(),
	_collisionPairs(),
	_collisionFlag(false),
	_isActive(false)
{
}

flt::Scene::~Scene()
{
	
}

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
		int index = gameObject->_index;
		if (index != -1)
		{
			ASSERT(_gameObjects[index] == gameObject, "Not exist GameObject");
			isExist = true;
		}
		else
		{
			for (auto& object : _gameObjectsToCreate)
			{
				if (object == gameObject)
				{
					isExist = true;
					break;
				}
			}
		}

		ASSERT(isExist, "Not exist game object");
	}

	_stagingActiveGameObjects[gameObject] = isEnable;
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

void flt::Scene::TweenUpdate(float deltaSecond)
{
	for (auto iter = _tweens.begin(); iter != _tweens.end(); ++iter)
	{
		bool isFinished = (*iter)->Update(deltaSecond);

		if (isFinished)
		{
			_tweensToDelete.push_back(iter.GetIndex());
		}
	}

	for (auto& index : _tweensToDelete)
	{
		_tweens.Erase(index);
	}
	_tweensToDelete.clear();

	for(auto iter = _posTweens.begin(); iter != _posTweens.end();)
	{
		Vector4f pos = iter->first->step(deltaSecond);
		iter->second->SetPosition(pos);

		if(iter->first->isEnd())
		{
			iter = _posTweens.erase(iter);
		}
		else
		{
			++iter;
		}
	}

	for(auto iter = _scaleTweens.begin(); iter != _scaleTweens.end();)
	{
		Vector4f scale = iter->first->step(deltaSecond);
		iter->second->SetScale(scale);

		if(iter->first->isEnd())
		{
			iter = _scaleTweens.erase(iter);
		}
		else
		{
			++iter;
		}
	}

	for(auto iter = _rotTweens.begin(); iter != _rotTweens.end();)
	{
		Quaternion rot = iter->first->step(deltaSecond);
		iter->second->SetRotation(rot);

		if(iter->first->isEnd())
		{
			iter = _rotTweens.erase(iter);
		}
		else
		{
			++iter;
		}
	}
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

void flt::Scene::DestroyGameObjectRecursive(GameObject* gameObject)
{
	std::vector<GameObject*> children = gameObject->GetChildren();
	for (auto& child : children)
	{
		DestroyGameObjectRecursive(child);
	}

	for (auto& component : gameObject->_components)
	{
		component->OnDestroy();
	}
	gameObject->OnDestroy();

	gameObject->_index = -1;
	_gameObjectsToDelete.push_back(gameObject);
}

void flt::Scene::PrePhysicsUpdate()
{
	for (auto& object : _activeGameObjects)
	{
		//object->FixedUpdate();

		for (auto& component : object->_components)
		{
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
	for (auto& object : _activeGameObjects)
	{
		for (auto& component : object->_components)
		{
			if (!component->_isEnable)
			{
				continue;
			}

			component->PostPhysics();
		}
	}

	// FixedUpdate
	for (auto& object : _activeGameObjects)
	{
		for (auto& component : object->_components)
		{
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
	for (auto& object : _activeGameObjects)
	{
		object->PreUpdate(deltaSecond);

		for (auto& component : object->_components)
		{
			if (!component->_isEnable)
			{
				continue;
			}

			component->PreUpdate(deltaSecond);
		}
	}

	for (auto& object : _activeGameObjects)
	{
		object->Update(deltaSecond);

		for (auto& component : object->_components)
		{
			if (!component->_isEnable)
			{
				continue;
			}

			component->Update(deltaSecond);
		}
	}

	TweenUpdate(deltaSecond);

	for (auto& object : _activeGameObjects)
	{
		object->PostUpdate(deltaSecond);

		for (auto& component : object->_components)
		{
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
	for (auto& object : _activeGameObjects)
	{
		object->PreRender();

		for (auto& component : object->_components)
		{
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
	for (auto& object : _activeGameObjects)
	{
		object->PostRender();

		for (auto& component : object->_components)
		{
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

		auto iter = _gameObjects.EmplaceBack(object);
		object->_index = iter.GetIndex();

		for (auto& component : object->_components)
		{
			component->OnCreate();
		}
		object->OnCreate();

		if (object->_isEnable)
		{
			object->_isEnable = false;
			object->Enable();
		}
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

	for (int i = 0; i < _gameObjectsToDestroy.size(); ++i)
	{
		GameObject* object = _gameObjectsToDestroy[i];
		object->Disable();
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

		int index = object->_index;
		if(index == -1)
		{
			continue;
		}

		_gameObjects.Erase(index);
		object->_index = -1;
		DestroyGameObjectRecursive(object);
	}

	while (!_stagingActiveGameObjects.empty())
	{
		auto iter = _stagingActiveGameObjects.begin();
		GameObject* object = iter->first;
		bool isUpdate = iter->second;

		if (isUpdate)
		{
			// 아직 활성화 되어있지 않는 경우에만 처리
			if (object->_updateIndex == -1)
			{
				auto iter = _activeGameObjects.EmplaceBack(object);
				object->_updateIndex = iter.GetIndex();
			}
		}
		else
		{
			// 이미 활성화 되어있는 경우에만 처리
			if (object->_updateIndex != -1)
			{
				_activeGameObjects.Erase(object->_updateIndex);
				object->_updateIndex = -1;
			}
		}

		_stagingActiveGameObjects.erase(iter);
	}

	while(!_gameObjectsToDelete.empty())
	{
		GameObject* object = _gameObjectsToDelete.back();
		_gameObjectsToDelete.pop_back();

		ASSERT(object->_index == -1, "Destroy GameObject Error");
		for (auto& component : object->_components)
		{
			delete component;
		}
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
	for (auto& object : _activeGameObjects)
	{
		for (auto& component : object->_components)
		{
			if (component->_isEnable)
			{
				component->OnDisable();
			}
		}
		object->OnDisable();
	}

	for (auto& object : _gameObjects)
	{
		for (auto& component : object->_components)
		{
			component->OnDestroy();
		}
		object->OnDestroy();
	}

	while (!_gameObjects.Empty())
	{
		GameObject* object = _gameObjects.Back();

		for (auto& component : object->_components)
		{
			delete component;
		}
		delete object;
		//object = nullptr;

		_gameObjects.PopBack();
	}

	while (!_gameObjectsToCreate.empty())
	{
		GameObject* object = _gameObjectsToCreate.back();
		_gameObjectsToCreate.pop_back();

		for (auto& component : object->_components)
		{
			delete component;
		}
		delete object;
	}

	_activeGameObjects.Clear();
	_stagingActiveGameObjects.clear();
	_gameObjectsToDestroy.clear();
	_componentsToEnable.clear();
	_componentsToDisable.clear();
	_collisionPairs.clear();
}

void flt::Scene::AddTween(IFLTween* tween)
{
	_tweens.EmplaceBack(tween);
}

void flt::Scene::AddPosTween(FLTween<Vector4f>* tween, Transform* tr)
{
	_posTweens.emplace_back(tween, tr);
}

void flt::Scene::AddScaleTween(FLTween<Vector4f>* tween, Transform* tr)
{
	_scaleTweens.emplace_back(tween, tr);
}

void flt::Scene::AddRotTween(FLTween<Quaternion>* tween, Transform* tr)
{
	_rotTweens.emplace_back(tween, tr);
}
