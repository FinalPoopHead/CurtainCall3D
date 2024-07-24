#include "./include/internal/Scene.h"


flt::Scene::Scene()
	: _gameObjects()
	, _activeGameObjects()
	, _stagingActiveGameObjects()
	, _gameObjectsToCreate()
	, _gameObjectsToDestroy()
	//,_componentsToEnable()
	//,_componentsToDisable()
	, _collisionPairs()
	, _collisionFlag(false)
	, _isActive(false)
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

	// 게임오브젝트 활성화를 위한 추가
	_stagingActiveGameObjects[gameObject] = isEnable;
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
	for (uint32 denseIndex = 0; denseIndex < _tweens.Size();)
	{
		IFLTween* tween = _tweens.AtDense(denseIndex);

		// 업데이트 이후 이 트윈이 이미 _tweens에서 없을 수 있음.
		bool isFinished = tween->Update(deltaSecond);
		if (isFinished)
		{
			StopTween(tween);
			//_tweens.EraseDense(denseIndex);
		}
		else
		{
			++denseIndex;
		}
	}

	for (uint32 denseIndex = 0; denseIndex < _posTweens.Size();)
	{
		FLTween<Vector4f>* tween = _posTweens.AtDense(denseIndex).first;
		Transform* target = _posTweens.AtDense(denseIndex).second;

		// 업데이트 이후 이 트윈이 이미 _posTweens에서 없을 수 있음.
		Vector4f pos = tween->step(deltaSecond);

		target->SetPosition(pos);

		if (tween->IsFinished())
		{
			StopTween(tween);
			//_posTweens.EraseDense(denseIndex);
		}
		else
		{
			++denseIndex;
		}
	}

	for (uint32 denseIndex = 0; denseIndex < _scaleTweens.Size();)
	{
		FLTween<Vector4f>* tween = _scaleTweens.AtDense(denseIndex).first;
		Transform* target = _scaleTweens.AtDense(denseIndex).second;

		// 업데이트 이후 이 트윈이 이미 _scaleTweens에서 없을 수 있음.
		Vector4f scale = tween->step(deltaSecond);

		target->SetScale(scale);

		if (tween->IsFinished())
		{
			StopTween(tween);
			//_scaleTweens.EraseDense(denseIndex);
		}
		else
		{
			++denseIndex;
		}
	}

	for (uint32 denseIndex = 0; denseIndex < _rotTweens.Size();)
	{
		FLTween<Quaternion>*& tween = _rotTweens.AtDense(denseIndex).first;
		Transform*& target = _rotTweens.AtDense(denseIndex).second;

		// 업데이트 이후 이 트윈이 이미 _rotTweens에서 없을 수 있음.
		Quaternion rot = tween->step(deltaSecond);

		target->SetRotation(rot);

		if (tween->IsFinished())
		{
			StopTween(tween);
			//_rotTweens.EraseDense(denseIndex);
		}
		else
		{
			++denseIndex;
		}
	}
}

void flt::Scene::DeleteTweensDeferred()
{
	for (auto& tween : _tweensToDelete)
	{
		delete tween;
	}

	_tweensToDelete.clear();
}

void flt::Scene::ClearTweens()
{
	_tweens.Clear();
	_posTweens.Clear();
	_scaleTweens.Clear();
	_rotTweens.Clear();

	for (auto& [tween, TweenInfo] : _tweenMap)
	{
		delete tween;
	}
	_tweenMap.clear();
	
	DeleteTweensDeferred();
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
	for (auto& object : _gameObjectsToCreate)
	{
		auto iter = _gameObjects.EmplaceBack(object);
		object->_index = iter.GetIndex();

		for (auto& component : object->_components)
		{
			component->OnCreate();
		}
		object->OnCreate();
	}

	while (!_gameObjectsToCreate.empty())
	{
		GameObject* object = _gameObjectsToCreate.back();
		_gameObjectsToCreate.pop_back();

		if (object->_isEnable)
		{
			object->_isEnable = false;
			// 이 내부에서 _stagingActiveGameObjects에 추가됨(자식 Object들도 추가해야 하기 떄문에_
			object->Enable();
		}
	}

	for (int i = 0; i < _gameObjectsToDestroy.size(); ++i)
	{
		GameObject* object = _gameObjectsToDestroy[i];
		object->Disable();
	}

	// Destroy 처리
	while (!_gameObjectsToDestroy.empty())
	{
		GameObject* object = _gameObjectsToDestroy.back();
		_gameObjectsToDestroy.pop_back();

		int index = object->_index;
		if (index == -1)
		{
			continue;
		}

		_gameObjects.EraseSparse(index);
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
				_activeGameObjects.EraseSparse(object->_updateIndex);
				object->_updateIndex = -1;
			}
		}

		_stagingActiveGameObjects.erase(iter);
	}

	while (!_gameObjectsToDelete.empty())
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

	DeleteTweensDeferred();
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
	_collisionPairs.clear();

	ClearTweens();
}

void flt::Scene::AddTween(IFLTween* tween)
{
	//_tweens.emplace_back(tween);
	_tweenMap[tween] = { TweenType::none, UINT_MAX, nullptr };
}

void flt::Scene::AddPosTween(FLTween<Vector4f>* tween, Transform* tr)
{
	//_posTweens.emplace_back(tween, tr);
	_tweenMap[static_cast<IFLTween*>(tween)] = { TweenType::pos, UINT_MAX, tr };
}

void flt::Scene::AddScaleTween(FLTween<Vector4f>* tween, Transform* tr)
{
	//_scaleTweens.emplace_back(tween, tr);
	_tweenMap[static_cast<IFLTween*>(tween)] = { TweenType::scale, UINT_MAX, tr };
}

void flt::Scene::AddRotTween(FLTween<Quaternion>* tween, Transform* tr)
{
	//_rotTweens.emplace_back(tween, tr);
	_tweenMap[static_cast<IFLTween*>(tween)] = { TweenType::rot, UINT_MAX, tr };
}

void flt::Scene::StartTween(IFLTween* tween)
{
	auto iter = _tweenMap.find(tween);
	if (iter == _tweenMap.end())
	{
		//ASSERT(false, "Not exist tween");
		return;
	}

	if (iter->second.sparseIndex != UINT_MAX)
	{
		//ASSERT(false, "Already Activated");
		iter->first->ResetProgress();
		return;
	}

	uint32 sparseIndex = UINT_MAX;
	switch (iter->second.type)
	{
		case flt::Scene::TweenType::none:
		{
			sparseIndex = _tweens.EmplaceBack(tween).GetIndex();
		}
		break;
		case flt::Scene::TweenType::pos:
		{
			sparseIndex = _posTweens.EmplaceBack(static_cast<FLTween<Vector4f>*>(tween), iter->second.tr).GetIndex();
		}
		break;
		case flt::Scene::TweenType::scale:
		{
			sparseIndex = _scaleTweens.EmplaceBack(static_cast<FLTween<Vector4f>*>(tween), iter->second.tr).GetIndex();
		}
		break;
		case flt::Scene::TweenType::rot:
		{
			sparseIndex = _rotTweens.EmplaceBack(static_cast<FLTween<Quaternion>*>(tween), iter->second.tr).GetIndex();

		}
		break;
		default:
			break;
	}
	iter->first->ResetProgress();
	iter->second.sparseIndex = sparseIndex;
}

void flt::Scene::StopTween(IFLTween* tween)
{
	auto iter = _tweenMap.find(tween);
	if (iter == _tweenMap.end())
	{
		//ASSERT(false, "Not exist tween");
		return;
	}

	if (iter->second.sparseIndex == UINT_MAX)
	{
		//ASSERT(false, "Not Activated");
		return;
	}

	switch (iter->second.type)
	{
		case flt::Scene::TweenType::none:
		{
			_tweens.EraseSparse(iter->second.sparseIndex);
		}
		break;
		case flt::Scene::TweenType::pos:
		{
			_posTweens.EraseSparse(iter->second.sparseIndex);
		}
		break;
		case flt::Scene::TweenType::scale:
		{
			_scaleTweens.EraseSparse(iter->second.sparseIndex);
		}
		break;
		case flt::Scene::TweenType::rot:
		{
			_rotTweens.EraseSparse(iter->second.sparseIndex);
		}
		break;
		default:
			break;
	}

	iter->second.sparseIndex = UINT_MAX;
}

void flt::Scene::ReleaseTween(IFLTween* tween)
{
	auto iter = _tweenMap.find(tween);
	if (iter == _tweenMap.end())
	{
		//ASSERT(false, "Not exist tween");
		return;
	}
	StopTween(tween);
	_tweenMap.erase(iter);

	_tweensToDelete.push_back(tween);
}

bool flt::Scene::IsActiveTween(IFLTween* tween)
{
	auto iter = _tweenMap.find(tween);
	if (iter == _tweenMap.end())
	{
		//ASSERT(false, "Not exist tween");
		return false;
	}
	return iter->second.sparseIndex != UINT_MAX;
}
